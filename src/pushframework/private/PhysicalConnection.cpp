/********************************************************************
	File :			PhysicalConnection.cpp
	Creation date :	2010/6/27

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
#include "StdAfx.h"
#include "PhysicalConnection.h"

#ifdef Plateform_Windows
#include "win\IOQueue_Win.h"
#else

#ifdef Plateform_Linux
#include "linux\IOQueue_Linux.h"
#endif
#endif

#include "ServerImpl.h"

#include "Dispatcher.h"
#include "ServerStats.h"

#include "ScopedLock.h"

#include "../include/ListenerOptions.h"
#include "../include/LogicalConnection.h"
#include "../include/ConnectionContext.h"


namespace PushFramework
{
PhysicalConnection::PhysicalConnection(SOCKET socket, SOCKADDR_IN address, bool bIsObserver, ListenerOptions* pListenerOption)
{
    this->socket = socket;

    rPeerPort = address.sin_port;
    rPeerIP = inet_ntoa(address.sin_addr);

    dtCreationTime = time(NULL);

    bWriteInProgress = FALSE;

    this->bIsObserver = bIsObserver;

    status = Connected;

    this->pListenerOption = pListenerOption;

    ioWorkersReferenceCounter = 0;

    oBuffer.Allocate(/*pListenerOption->uIntermediateSendBufferSize*/640 * 480 * 4);
    inBuffer.Allocate(pListenerOption->uIntermediateReceiveBufferSize);

#ifdef Plateform_Windows
    m_byInBuffer = new BYTE[pListenerOption->uReadBufferSize];
    m_byOutBuffer = new BYTE[pListenerOption->uSendBufferSize];

    m_wsaInBuffer.buf = (char*)m_byInBuffer;
    m_wsaInBuffer.len = pListenerOption->uReadBufferSize;
    m_wsaOutBuffer.buf = (char*)m_byOutBuffer;
    m_wsaOutBuffer.len = pListenerOption->uSendBufferSize;
    pReadOverlap = new OVERLAPPEDPLUS(IORead);
    pWriteOverlap = new OVERLAPPEDPLUS(IOWrite);
#endif

    broadcastCxt.nSentPackets = 0;
    broadcastCxt.pCurrentSinkChannel = NULL;

	pConnectionContext = NULL;

    ::InitializeCriticalSection(&csLock);
}

PhysicalConnection::~PhysicalConnection(void)
{
#ifdef Plateform_Windows
    delete [] m_byOutBuffer;
    delete [] m_byInBuffer;

    if(pReadOverlap)
        delete pReadOverlap;
    if(pWriteOverlap)
        delete pWriteOverlap;
#endif
	delete pConnectionContext;
    ::DeleteCriticalSection(&csLock);
}

int PhysicalConnection::getStatus()
{
    return status;
}

double PhysicalConnection::getLifeDuration()
{
    time_t current = time(NULL);
    return difftime(current, dtCreationTime);
}
SendResult PhysicalConnection::TryPushPacket( OutgoingPacket* pPacket )
{
    if (TryEnterCriticalSection(&csLock))
    {
        SendResult result = PushPacketCommon(pPacket);
        LeaveCriticalSection(&csLock);
        return result;
    }
    return SendResult_Retry;
}
SendResult PhysicalConnection::PushPacket( OutgoingPacket* pPacket )
{
    ScopedLock lock(csLock);
    return PushPacketCommon(pPacket);

}

const char* PhysicalConnection::getLogicalConnectionKey()
{
    if (isObserverChannel())
    {
        return clientKey.c_str();
    }
    else
        return pLogicalConnection->getKey();
}

void PhysicalConnection::SetBinaryFlag(bool binary)
{
	isBinary = binary;
}

void PhysicalConnection::SetConnectionContext( ConnectionContext* pConnectionContext )
{
    this->pConnectionContext = pConnectionContext;
}

ConnectionContext* PhysicalConnection::GetConnectionContext()
{
    return pConnectionContext;
}

DataBuffer& PhysicalConnection::GetReceiveBuffer()
{
    return inBuffer;
}

void PhysicalConnection::attachToClient( LogicalConnection* pLogicalConnection )
{
    this->pLogicalConnection = pLogicalConnection;
    status = Attached;
}

void PhysicalConnection::attachToClient( std::string clientKey )
{
    this->clientKey = clientKey;
    status = Attached;
}

bool PhysicalConnection::isObserverChannel()
{
    return bIsObserver;
}

UINT PhysicalConnection::getPeerPort()
{
    return rPeerPort;
}

std::string PhysicalConnection::getPeerIP()
{
    return rPeerIP;
}

Protocol* PhysicalConnection::getProtocol()
{
    return pListenerOption->pProtocol;
}

LogicalConnection* PhysicalConnection::getLogicalConnection()
{
    return pLogicalConnection;
}

void PhysicalConnection::incrementIoWorkersReferenceCounter()
{
    AtomicIncrement(ioWorkersReferenceCounter);
}

void PhysicalConnection::decrementIoWorkersReferenceCounter()
{
    AtomicDecrement(ioWorkersReferenceCounter);
}

int PhysicalConnection::getIoWorkersReferenceCounter() const
{
    return ioWorkersReferenceCounter;
}

SOCKET PhysicalConnection::getSocket()
{
    return socket;
}

bool PhysicalConnection::CheckIfUnusedByIOWorkers()
{
    return status == Disposable && ioWorkersReferenceCounter == 0;
}

PushFramework::SendResult PhysicalConnection::PushPacketCommon( OutgoingPacket* pPacket)
{
    if (status < Connected)
    {
        return SendResult_NotOK;
    }

    unsigned int uWrittenBytes = 0;

    Protocol* pProtocol = pListenerOption->pProtocol;

	int iResult = pProtocol->serializeOutgoingPacket(*pPacket, oBuffer, uWrittenBytes, isBinary);

    if (iResult!=Protocol::Success)
    {
        stats.addToCumul(ServerStats::BandwidthRejection, uWrittenBytes);
        return iResult==Protocol::eInsufficientBuffer ? SendResult_Retry : SendResult_NotOK;
    }

    stats.addToCumul(ServerStats::BandwidthOutstanding, uWrittenBytes);
    std::string serviceName;
    if (dispatcher.getCurrentService(serviceName))
    {
        stats.addToDistribution(ServerStats::BandwidthOutboundVolPerRequest, serviceName, uWrittenBytes);
    }

    //oBuffer.GrowSize(uWrittenBytes);

    if (!bWriteInProgress)
    {
        return WriteBytes() ? SendResult_OK : SendResult_NotOK;
    }
    return SendResult_OK;
}




}
