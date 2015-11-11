/********************************************************************
File :			PhysicalConnection_Win.cpp
Creation date :	2012/01/29

License :			Copyright 2010 Ahmed Charfeddine, http://www.pushframework.com

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.


*********************************************************************/
#include "..\StdAfx.h"
#include "..\PhysicalConnection.h"

#include "..\ScopedLock.h"
#include "..\include\ListenerOptions.h"

namespace PushFramework
{

void PhysicalConnection::PostReceive()
{
    if (status < Connected)
    {
        return;
    }

    ULONG	ulFlags = MSG_PARTIAL;
    DWORD dwIoSize;

    UINT nRetVal = WSARecv(socket, &m_wsaInBuffer, 1, &dwIoSize, &ulFlags, &pReadOverlap->m_ol, NULL);

    if ( nRetVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        return;
    }
    incrementIoWorkersReferenceCounter();
}


int PhysicalConnection::ReadReceivedBytes( int& dwIoSize )
{
    if(status < Connected)
    {
        return status;
    }

    if(dwIoSize==0)
    {
        return status;
    }
	std::cout << dwIoSize << std::endl;
    inBuffer.Append((char*)m_byInBuffer, dwIoSize);

    return status;
}

bool PhysicalConnection::WriteBytes()
{
    DWORD dwSent;
    DWORD dwToPick=min(pListenerOption->uSendBufferSize,oBuffer.GetDataSize());

    CopyMemory(m_wsaOutBuffer.buf,oBuffer.GetBuffer(),dwToPick);

    m_wsaOutBuffer.len=dwToPick;
    ULONG	ulFlags = MSG_PARTIAL;

    UINT nRetVal = WSASend(socket, &m_wsaOutBuffer, 1, &dwSent, ulFlags, &pWriteOverlap->m_ol, NULL);
    if ( nRetVal == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
    {
        return false;
    }
    bWriteInProgress = TRUE;
    incrementIoWorkersReferenceCounter();
    return true;
}


int PhysicalConnection::OnSendCompleted( int dwIoSize, bool& bIsBufferIdle )
{
    ScopedLock lock(csLock);

    if(status < WaitingForWrite)
    {
        return status;
    }

    oBuffer.Pop(dwIoSize);

    bIsBufferIdle = oBuffer.GetDataSize()==0;
    if (bIsBufferIdle)
    {
        if (status==WaitingForWrite )
        {
            CloseSocket();
        }
        else
        {
            bWriteInProgress = FALSE;
        }
        return status;
    }

    WriteBytes();
    return status;
}

void PhysicalConnection::Close( bool bWaitForSendsToComplete )
{
    ScopedLock lock(csLock);

    if (status < Connected)
    {
        return;
    }

    //Either connected or attached :
    if (bWaitForSendsToComplete)
    {
        if (oBuffer.GetDataSize() == 0)
        {
            CloseSocket();
        }
        else
            status = WaitingForWrite;
    }
    else
    {
        CloseSocket();
    }
}

void PhysicalConnection::CloseSocket()
{
    closesocket( socket );
    status = Disposable;
}

}