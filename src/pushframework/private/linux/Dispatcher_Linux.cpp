
#include "StdAfx.h"
#include "Dispatcher.h"

#include "ServerImpl.h"
#include "ChannelFactory.h"
#include "ServerStats.h"
#include "StopWatch.h"
#include "../include/ClientFactory.h"
#include "PhysicalConnection.h"

#ifdef Plateform_Windows
#include "IOQueue_Win.h"
#else
#ifdef Plateform_Linux
#include "IOQueue_Linux.h"
#include "MonitorsBroadcastManager.h"
#endif
#endif

namespace PushFramework
{

void Dispatcher::OnWriteComplete(PhysicalConnection* pChannel, int dwIoSize/*UNUSED*/)
{
    // No stats collected here, rather at write-time operation.
    bool bIsBufferIdle;
    int status = pChannel->OnSendCompleted(-1, bIsBufferIdle);

    if (status == PhysicalConnection::Attached && bIsBufferIdle)
    {
        if (pChannel->isObserverChannel())
        {
            monitorBroadcastManager.HandleSingleMonitor(pChannel);
        }
    }

    pChannel->decrementIoWorkersReferenceCounter();
}

void Dispatcher::OnReceiveComplete(PhysicalConnection* pChannel, int dwIoSize)
{
    int status = pChannel->ReadReceivedBytes(dwIoSize);

    if (status < PhysicalConnection::Connected)
    {
        pChannel->decrementIoWorkersReferenceCounter();
        return;
    }

    bool isClient = !pChannel->isObserverChannel();

    //Channel is either connected or attached.
    if (dwIoSize == 0)//Peer wants to close the connection.
    {
        pChannel->Close(false);
        if (status == PhysicalConnection::Attached)
        {
            if (isClient)
            {
                pServerImpl->getClientFactory()->onClientDisconnected(pChannel->getLogicalConnection());
            }
            else
            {
                //TODO notify observer is out.
            }
        }
        pChannel->decrementIoWorkersReferenceCounter();
        return;
    }

    if (isClient)
    {
        stats.addToCumul(ServerStats::BandwidthInbound, dwIoSize);
        stats.addToKeyedDuration(ServerStats::BandwidthInboundPerConnection, (unsigned int) pChannel, dwIoSize);
    }

    //The Processing Loop.
    int uCommandID;
    IncomingPacket* pPacket;
    int iResult;
    unsigned int uExtractedBytes;
    Protocol* pProtocol = pChannel->getProtocol();
    DataBuffer& sourceBuffer = pChannel->GetReceiveBuffer();

    bool bProcessDataInQueue = true;
    while (bProcessDataInQueue)
    {

        iResult = pProtocol->tryDeserializeIncomingPacket(sourceBuffer, pPacket, uCommandID, uExtractedBytes, pChannel->GetConnectionContext());

        if (iResult == Protocol::Success)
        {
            pChannel->GetReceiveBuffer().Pop(uExtractedBytes);
            if (status == PhysicalConnection::Attached)
            {
                if (isClient)
                    dispatchRequest(uCommandID, pChannel->getLogicalConnection(), *pPacket, uExtractedBytes);
                else
                    HandleMonitorRequest(pChannel, *pPacket);
            }
            else if (status == PhysicalConnection::Connected)
            {
                if (isClient)
                    ProcessFirstPacket(pChannel, uCommandID, *pPacket, uExtractedBytes);
                else
                    ProcessMonitorFirstPacket(pChannel, *pPacket);
            }
            else
            {
                //Status changed by another thread eg ::disconnect.
                bProcessDataInQueue = false;
            }
            pProtocol->disposeIncomingPacket(pPacket);
        }
        else if (iResult == Protocol::eDecodingFailure)
        {
            pChannel->GetReceiveBuffer().Pop(uExtractedBytes);
        }
        else
            break;
    }
    //
    if (iResult == Protocol::eIncompletePacket &&
            pChannel->getStatus() >= PhysicalConnection::Connected)
    {
        ioQueue.RearmSocketInputEvents(pChannel->getSocket(), pChannel);
        pChannel->incrementIoWorkersReferenceCounter();
        //pChannel->PostReceive();
    }

    pChannel->decrementIoWorkersReferenceCounter();
}
}
