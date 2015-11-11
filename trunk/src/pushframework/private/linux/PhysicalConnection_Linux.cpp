/*
 * Channel_Win.cpp
 *
 *  Created on: May 27, 2011
 *      Author: sst
 */

#include "StdAfx.h"
#include "PhysicalConnection.h"

#include "ScopedLock.h"
#include "../include/ListenerOptions.h"
#include "ServerImpl.h"
#include "ServerStats.h"

#ifdef Plateform_Windows
#include "IOQueue_Win.h"
#else
#ifdef Plateform_Linux
#include "IOQueue_Linux.h"
#endif
#endif

namespace PushFramework
{

void PhysicalConnection::PostReceive()
{
    //Not needed In Linux situation.
}

int PhysicalConnection::ReadReceivedBytes(int& dwIoSize)
{
    if(status < PhysicalConnection::Connected)
    {
        return status;
    }

    dwIoSize = 0;

    int nMaxToPick = min(pListenerOption->uReadBufferSize, inBuffer.getRemainingSize());

    if (nMaxToPick == 0)
    {
        //Fatal scenario : data is stuck at intermediate buffer without getting de-serialized.
        return status;
    }

    int ret = recv(socket, inBuffer.GetBuffer() + inBuffer.GetDataSize(), nMaxToPick, 0);

    if (ret <= 0)
    {
        //either remote socket close or an error or EAGAIN
    }
    else
    {
        dwIoSize += ret;
        inBuffer.GrowSize(ret);
    }
    return status;
}

bool PhysicalConnection::WriteBytes()
{
    //We should call send, untill the return value is EAGAIN
    while (true)
    {
        int dwToPick = min(pListenerOption->uSendBufferSize, oBuffer.GetDataSize());
        if (dwToPick == 0)
            return true;

        int ret = send(socket, oBuffer.GetBuffer(), dwToPick, 0);

        if (ret > 0)
        {
            oBuffer.Pop(ret);
            bool reportSentData = !isObserverChannel();

            if (reportSentData)
            {
                stats.addToCumul(ServerStats::BandwidthOutbound, ret);
                stats.addToKeyedDuration(ServerStats::BandwidthOutboundPerConnection, (unsigned int) this, ret);
            }
        }
        else if (ret == EAGAIN)
        {
            bWriteInProgress = TRUE;
            this->incrementIoWorkersReferenceCounter();
            ioQueue.RearmSocketOutputEvents(getSocket(), this);
            return true;
        }
        else
        {
            return false;
        }
        //End of while
    }
}

int PhysicalConnection::OnSendCompleted(int dwIoSize/*Unused*/, bool& bIsBufferIdle)
{
    ScopedLock lock(csLock);

    bWriteInProgress = FALSE;

    if (status < WaitingForWrite)
    {
        return status;
    }

    WriteBytes();

    bIsBufferIdle = (oBuffer.GetDataSize() == 0 && !bWriteInProgress);
    if(bIsBufferIdle && status == WaitingForWrite)
        CloseSocket();

    return status;
}

void PhysicalConnection::Close(bool bWaitForSendsToComplete)
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
    close(socket);
    status = Disposable;
}
}
