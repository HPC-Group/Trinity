/********************************************************************
	File :			ClientFactoryImpl.cpp
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
#include "ClientFactoryImpl.h"


#include "../include/LogicalConnection.h"
#include "../include/ClientFactory.h"
#include "PhysicalConnection.h"
#include "ServerImpl.h"
#include "ServerStats.h"
#include "../include/BroadcastManager.h"
#include "BroadcastManagerImpl.h"
#include "Dispatcher.h"
#include "GarbageCollector.h"

#include "ScopedLock.h"


namespace PushFramework
{

PUSHFRAMEWORK_API void ReturnClient(LogicalConnection* pClient)
{
    clientFactoryImpl.ReturnClient(pClient);
}
PUSHFRAMEWORK_API LogicalConnection* FindClient(CLIENT_KEY clientName)
{
    return clientFactoryImpl.FindLogicalConnection(clientName);
}
PUSHFRAMEWORK_API void Disconnect( LogicalConnection* pLogicalConnection, bool waitForPendingPackets )
{
    clientFactoryImpl.Disconnect(pLogicalConnection, waitForPendingPackets, false);
}
PUSHFRAMEWORK_API void DisconnectAndReturn( LogicalConnection* pLogicalConnection, bool waitForPendingPackets )
{
    clientFactoryImpl.Disconnect(pLogicalConnection, waitForPendingPackets, true);
}

ClientFactoryImpl clientFactoryImpl;

ClientFactoryImpl::ClientFactoryImpl()
{
    nClientsCount = 0;
    ::InitializeCriticalSection(&cs);
}

ClientFactoryImpl::~ClientFactoryImpl(void)
{
    ::DeleteCriticalSection(&cs);
}

LogicalConnection* ClientFactoryImpl::AddLogicalConnection( LogicalConnection* pClient )
{
    std::string key(pClient->getKey());
    ScopedLock csLock(cs);
    //
    LogicalConnection* pRet = pClient;

    clientMapT::iterator it = clientMap.find(key);
    if(it!=clientMap.end())
    {
        pRet =  it->second;
    }
    else
    {
        clientMap[key] = pClient;
        nClientsCount++;
    }

    pRet->IncrementUsage();
    return pRet;
}




LogicalConnection* ClientFactoryImpl::FindLogicalConnection( const char* _key )
{
    std::string key(_key);
    //
    ScopedLock csLock(cs);

    clientMapT::iterator it = clientMap.find(key);
    if(it==clientMap.end())
        return NULL;

    LogicalConnection* pClient = it->second;

    pClient->IncrementUsage();
    return pClient;
}


Server* ClientFactoryImpl::getServer()
{
    return pServerImpl->getFacade();
}

void ClientFactoryImpl::SilentDisconnect( LogicalConnection* pLogicalConnection )
{
    if (!RemoveIfExisting(pLogicalConnection))
    {
        return;
    }
    pLogicalConnection->pPhysicalConnection->Close(false);

    //Send to garbage collector.
    garbageCollector.AddDisposableClient(pLogicalConnection);

}
void ClientFactoryImpl::Disconnect( LogicalConnection* pLogicalConnection, bool waitForPendingPackets , bool returnClient)
{
    if (!RemoveIfExisting(pLogicalConnection))
    {
        if (returnClient)
        {
            pLogicalConnection->DecrementUsage();
        }
        return;
    }
    pLogicalConnection->pPhysicalConnection->Close(waitForPendingPackets);

    //Now it is set to disconnected.

    pServerImpl->getClientFactory()->onBeforeDisposeClient(pLogicalConnection);

    broadcastManagerImpl.RemoveClient(pLogicalConnection->getKey());
    dispatcher.NotifyObserversClientOut(pLogicalConnection->getKey());


    //Statistics :
    stats.addToCumul(ServerStats::VisitorsHitsOut, 1);
    stats.addToCumul(ServerStats::VisitorsOnline, -1);
    stats.addToDuration(ServerStats::VisitorsDuration, pLogicalConnection->getVisitDuration());


    if (returnClient)
    {
        pLogicalConnection->DecrementUsage();
    }

    //Send to garbage collector.
    garbageCollector.AddDisposableClient(pLogicalConnection);
}

bool ClientFactoryImpl::RemoveIfExisting( LogicalConnection* pLogicalConnection )
{
    ScopedLock csLock(cs);

    clientMapT::iterator it = clientMap.find(pLogicalConnection->getKey());
    if (it == clientMap.end())
    {
        return false;
    }
    nClientsCount--;

    clientMap.erase(it);
    return true;
}

unsigned int ClientFactoryImpl::GetClientCount()
{
    return nClientsCount;
}

void ClientFactoryImpl::ReturnClient( LogicalConnection* pClient )
{
    pClient->DecrementUsage();
}




}

