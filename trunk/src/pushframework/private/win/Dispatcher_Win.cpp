/********************************************************************
	File :			Dispatcher_Win.cpp
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
#include "..\Dispatcher.h"

#include "..\ServerImpl.h"
#include "..\ChannelFactory.h"
#include "..\ServerStats.h"
#include "..\StopWatch.h"
#include "../include/ClientFactory.h"
#include "..\MonitorsBroadcastManager.h"


namespace PushFramework
{
void Dispatcher::OnWriteComplete( PhysicalConnection* pChannel, int dwIoSize )
{
    bool reportSentData  = !pChannel->isObserverChannel();

    if (reportSentData)
    {
        stats.addToCumul(ServerStats::BandwidthOutbound, dwIoSize);
        stats.addToKeyedDuration(ServerStats::BandwidthOutboundPerConnection, (unsigned int) pChannel, dwIoSize);
    }

    bool bIsBufferIdle;
    int status = pChannel->OnSendCompleted(dwIoSize, bIsBufferIdle);

    if (status == PhysicalConnection::Attached && bIsBufferIdle)
    {
        if (pChannel->isObserverChannel())
        {
            monitorBroadcastManager.HandleSingleMonitor(pChannel);
        }
    }

    pChannel->decrementIoWorkersReferenceCounter();
}

void Dispatcher::OnReceiveComplete( PhysicalConnection* pChannel, int dwIoSize )
{
    bool isClient = !pChannel->isObserverChannel();

    if (isClient)
    {
        stats.addToCumul(ServerStats::BandwidthInbound, dwIoSize);
        stats.addToKeyedDuration(ServerStats::BandwidthInboundPerConnection, (unsigned int) pChannel, dwIoSize);
    }

    int status = pChannel->ReadReceivedBytes(dwIoSize);

    if(status < PhysicalConnection::Connected)
    {
        pChannel->decrementIoWorkersReferenceCounter();
        return;
    }

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
        //watch.GetElapsedTime(false);
        iResult = pProtocol->tryDeserializeIncomingPacket(sourceBuffer, pPacket, uCommandID, uExtractedBytes, pChannel->GetConnectionContext());
        if (iResult == Protocol::Success)
        {
            pChannel->GetReceiveBuffer().Pop(uExtractedBytes);
            if (status == PhysicalConnection::Attached)
            {
                if(isClient)
                    dispatchRequest(uCommandID, pChannel->getLogicalConnection(), *pPacket, uExtractedBytes);
                else
                    HandleMonitorRequest(pChannel, *pPacket);
            }
            else if(status == PhysicalConnection::Connected)
            {
                if(isClient)
                    ProcessFirstPacket(pChannel, uCommandID, *pPacket,uExtractedBytes);
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
    if (iResult == Protocol::eIncompletePacket)
    {
        pChannel->PostReceive();
    }

    pChannel->decrementIoWorkersReferenceCounter();
}
}