/********************************************************************
	File :			MonitorsBroadcastManager.cpp
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
#include "StdAfx.h"
#include "MonitorsBroadcastManager.h"

#include "ChannelFactory.h"
#include "XMLPacket.h"

#include "PhysicalConnection.h"
#include "BroadcastContext.h"

#include "ScopedLock.h"

namespace PushFramework
{

MonitorsBroadcastManager monitorBroadcastManager;

MonitorsBroadcastManager::MonitorsBroadcastManager(  )
{
    CreateBroadcastQueue("stats", 100, true, 10, 10);
    CreateBroadcastQueue("clientsIn", 1000, true, 5, 5);
    CreateBroadcastQueue("clientsOut", 50, true, 5, 5);

    InitializeCriticalSection(&cs);
}

MonitorsBroadcastManager::~MonitorsBroadcastManager( void )
{
    DeleteCriticalSection(&cs);
}

void MonitorsBroadcastManager::ActivateSubscribers( std::string channelName )
{
    HandleAllMonitors();
}

void MonitorsBroadcastManager::PreEncodeOutgoingPacket( OutgoingPacket* pPacket )
{
    //Nothing to do, packet is already encoded.
}

void MonitorsBroadcastManager::DeleteOutgoingPacket( OutgoingPacket* pPacket )
{
    XMLPacket* pResponse = (XMLPacket*) pPacket;
    delete pResponse;
}

bool MonitorsBroadcastManager::HandleSingleMonitor( PhysicalConnection* pMonitorChannel )
{
    if (pMonitorChannel->CheckIfUnusedByIOWorkers())
        return true;

    BroadcastQueueInfo* pFoundInfo = NULL;
    PacketInfo* pPacketInfo = NULL;

    std::string subscriberKey = pMonitorChannel->getLogicalConnectionKey();
    BroadcastContext& broadcastCtxt = pMonitorChannel->broadcastCxt;

    while (true)
    {
        OutgoingPacket* pPacket = GetNextPacket(subscriberKey, broadcastCtxt, pPacketInfo, pFoundInfo);
        if (!pPacket)
            return false;

        SendResult result = pMonitorChannel->PushPacket(pPacket);
        DisposePacket(pPacketInfo, subscriberKey, pFoundInfo, broadcastCtxt, result == SendResult_OK);

        if (result == SendResult_NotOK)
        {
            return false;
        }
    }
}

void MonitorsBroadcastManager::HandleAllMonitors()
{
    ScopedLock lock(cs);
    //
    vectConnectionsT::iterator it = vectMonitorsConnections.begin();
    while (it != vectMonitorsConnections.end())
    {
        PhysicalConnection* pMonitor = *it;
        if (HandleSingleMonitor(pMonitor))
        {
            it = vectMonitorsConnections.erase(it);
            delete pMonitor;
        }
        else
            it++;
    }
}

void MonitorsBroadcastManager::AddMonitor( PhysicalConnection* pMonitor )
{
    ScopedLock lock(cs);
    //
    vectMonitorsConnections.push_back(pMonitor);
    //
}

}

