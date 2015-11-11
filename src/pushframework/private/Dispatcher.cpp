/********************************************************************
        File :			Dispatcher.cpp
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
#include "Dispatcher.h"



#include "ScopedLock.h"
#include "PhysicalConnection.h"
#include "ServerImpl.h"
#include "ServerStats.h"

#include "../include/Protocol.h"
#include "XMLPacket.h"

#include "../include/ClientFactory.h"
#include "../include/LogicalConnection.h"
#include "../include/Service.h"
#include "../include/BroadcastManager.h"
#include "../include/Server.h"
#include "ChannelFactory.h"
#include "ClientFactoryImpl.h"

#include "BroadcastManagerImpl.h"

#include "MonitorsBroadcastManager.h"
#include "BroadcastStreamerManager.h"

#include "BroadcastQueue.h"
#include "StopWatch.h"
#include "Utilities.h"



namespace PushFramework
{

Dispatcher dispatcher;

Dispatcher::Dispatcher()
{

    ::InitializeCriticalSection(&csSrvMap);


}

Dispatcher::~Dispatcher(void)
{
    ::DeleteCriticalSection(&csSrvMap);


}

std::string Dispatcher::getServiceNames()
{
    std::stringstream ss;
    ss << std::noskipws;

    for (serviceMapT::iterator it = serviceMap.begin();
            it != serviceMap.end();
            it++)
    {
        ss << "<service val=\"" << it->second->serviceName << "\"/>";
    }

    return ss.str();
}

void Dispatcher::setCurrentService(std::string serviceName)
{
    ScopedLock lock(csSrvMap);
    ThreadIDType dwThread = Utilities::getCurrentThreadID();

    workerServiceMap[dwThread] = serviceName;
}

void Dispatcher::UnsetCurrentService()
{
    ScopedLock lock(csSrvMap);
    ThreadIDType dwThread = Utilities::getCurrentThreadID();

    workerServiceMapT::iterator it = workerServiceMap.find(dwThread);
    if (it != workerServiceMap.end())
        workerServiceMap.erase(it);
}

bool Dispatcher::getCurrentService(std::string& serviceName)
{
    ScopedLock lock(csSrvMap);
    ThreadIDType dwThread = Utilities::getCurrentThreadID();

    workerServiceMapT::iterator it = workerServiceMap.find(dwThread);
    if (it == workerServiceMap.end())
        return false;

    serviceName = it->second;
    return true;
}

void Dispatcher::NotifyObserversClientIN(const char* key, std::string peerIP, unsigned int peerPort)
{
    std::string timestamp = Utilities::getCurrentTime();

    XMLPacket *response = new XMLPacket(AnalyticsProtocol::VisitorInResponse);
    response->setArgumentAsText("time", timestamp.c_str());
    response->setArgumentAsText("name", key);
    response->setArgumentAsText("ip", peerIP.c_str());
    response->setArgumentAsInt("port", peerPort);

    monitorBroadcastManager.PushPacket(response, "clientsIn", key, 0);
}

void Dispatcher::NotifyObserversClientOut(const char* key)
{

    XMLPacket *response = new XMLPacket(AnalyticsProtocol::VisitorOutResponse);
    response->setArgumentAsText("name", key);

    monitorBroadcastManager.PushPacket(response, "clientsOut");

    //Remove client from the other broadcast group :
    monitorBroadcastManager.RemovePacket(key, 0, "clientsIn");
}

void Dispatcher::OnInitializeReady(PhysicalConnection* pChannel)
{
    bool isClient = !pChannel->isObserverChannel();

    if (isClient)
    {
        ConnectionContext* lpContext = NULL;
        OutgoingPacket* pPacket = NULL;
        pPacket = pServerImpl->getClientFactory()->onNewConnection(lpContext);
        if (lpContext)
            pChannel->SetConnectionContext(lpContext);
        if (pPacket)
        {         
            pChannel->PushPacket(pPacket);
            pServerImpl->getFacade()->disposeOutgoingPacket(pPacket);
        }
    }

    pChannel->PostReceive();
    //
}



void Dispatcher::registerService(unsigned int uCommmand, Service* pService, std::string serviceName)
{
    ServiceInfo* pServiceInfo = new ServiceInfo;

    pServiceInfo->pService = pService;
    pServiceInfo->serviceName = serviceName;

    serviceMap[uCommmand] = pServiceInfo;
}

void Dispatcher::dispatchRequest( unsigned int uCommand,PushFramework::LogicalConnection* pClient,IncomingPacket& packet,unsigned int serviceBytes )
{
    //StopWatch dispatchWatch(m_QPFrequency);

    serviceMapT::iterator it = serviceMap.find(uCommand);
    if (it == serviceMap.end())
        return;
    //
    Service* pHandler = it->second->pService;

    //wcout << L"Locating Service : " << dispatchWatch.GetElapsedTime(false) << std::endl;

    //Mark dispatched service :

    setCurrentService(it->second->serviceName);

    StopWatch watch;
    pHandler->handle(pClient, &packet);


    double duration = watch.GetElapsedTime();
    /*	wcout << L"Service Time : " << watch.GetElapsedTime() << std::endl;
     */


    //StopWatch statsClock(m_QPFrequency);
    stats.addToDistribution(ServerStats::PerformanceProcessingTimePerService, it->second->serviceName, duration);
    //wcout << L"Stat 1 : " << statsClock.GetElapsedTime(false) << std::endl;

    stats.addToDuration(ServerStats::PerformanceProcessingTime, duration);
    //wcout << L"Stat 2 : " << statsClock.GetElapsedTime(false) << std::endl;

    UnsetCurrentService();

    //Stats. :

    stats.addToDistribution(ServerStats::BandwidthInboundVolPerRequest, it->second->serviceName, serviceBytes);
    //wcout << L"Stat 3 : " << statsClock.GetElapsedTime(false) << std::endl;


    stats.addToDistribution(ServerStats::PerformanceRequestVolPerRequest, it->second->serviceName, 1);
    //wcout << L"Stat 4 : " << statsClock.GetElapsedTime(false) << std::endl;


    //wcout << L"Dispatch Time : " << dispatchWatch.GetElapsedTime() << std::endl;
}

void Dispatcher::ProcessFirstPacket(PhysicalConnection* pChannel, unsigned int uCommand, IncomingPacket& packet, unsigned int serviceBytes)
{
    OutgoingPacket* pOutPacket = NULL;
    //
    LogicalConnection* pClient;
    int iResult = pServerImpl->getClientFactory()->onFirstRequest(packet, pChannel->GetConnectionContext(), pClient, pOutPacket);

    //packet and lpContext are not good.
    if (iResult == ClientFactory::RefuseAndClose)
    {
        pChannel->PushPacket(pOutPacket);
        pChannel->Close(false);
        return;
    }
    //
    if (iResult == ClientFactory::RefuseRequest)
    {
        if (pOutPacket)
        {
            pChannel->PushPacket(pOutPacket);
            pServerImpl->getFacade()->disposeOutgoingPacket(pOutPacket);
        }
        return;
    }

    std::string clientKey = pClient->getKey();


    //iResult >= CClientFactory::CreateClient
    if (!channelFactory.RemoveFromUnattachedList(pChannel))
    {
        delete pClient;
        pChannel->decrementIoWorkersReferenceCounter();
        return;
    }


    //Bind the two structures.
    pClient->pPhysicalConnection = pChannel;
    pChannel->attachToClient(pClient); //status is attached.

    stats.addToCumul(ServerStats::VisitorsHitsIn, 1);

    LogicalConnection* pRet = clientFactoryImpl.AddLogicalConnection(pClient);
    if (pRet != pClient)
    {
        //Copy data from existing to new instance.
        pServerImpl->getClientFactory()->CopyLogicalConnection(pRet, pClient);

        //Silent disconnect :
        clientFactoryImpl.SilentDisconnect(pRet);

        //Now add ours :
        clientFactoryImpl.AddLogicalConnection(pClient);

        //Fire Reconnect event.
        pServerImpl->getClientFactory()->onClientReconnected(pClient);

        //Add client to a streamer :
        broadcastStreamerManager.AddClient(pClient);

        pClient->DecrementUsage();
        return;
    }

    //Fire connect event.
    pServerImpl->getClientFactory()->onClientConnected(pClient);
    //Add client to a streamer :
    broadcastStreamerManager.AddClient(pClient);

    //Statistics :
    NotifyObserversClientIN(clientKey.c_str(), pChannel->getPeerIP(), pChannel->getPeerPort());
    stats.addToCumul(ServerStats::VisitorsOnline, 1);


    if (iResult == ClientFactory::CreateAndRouteRequest)
    {
        dispatchRequest(uCommand, pClient, packet, serviceBytes);
    }
    else
    {
        if (pOutPacket)
        {
            pChannel->PushPacket(pOutPacket);
            pServerImpl->getFacade()->disposeOutgoingPacket(pOutPacket);
        }
    }

    pClient->DecrementUsage();
}

void Dispatcher::HandleMonitorRequest(PhysicalConnection* pChannel, IncomingPacket& packet)
{
    XMLPacket& requestPacket = (XMLPacket&) packet;


    int typeId = requestPacket.getTypeId();

    if (typeId == AnalyticsProtocol::LiveSubscriptionRequest)
    {
        bool bSubscribe = requestPacket.getArgumentAsBool("resume");
        if (bSubscribe)
        {
            OutgoingPacket* pInitPacket = stats.getInitializationPacket();
            pChannel->PushPacket(pInitPacket);
            pServerImpl->getFacade()->disposeOutgoingPacket(pInitPacket);

            //
            monitorBroadcastManager.SubscribeClient(pChannel->getLogicalConnectionKey(), "stats");
            monitorBroadcastManager.SubscribeClient(pChannel->getLogicalConnectionKey(), "clientsIn");
            monitorBroadcastManager.SubscribeClient(pChannel->getLogicalConnectionKey(), "clientsOut");
        }
        else
        {
            monitorBroadcastManager.RemoveClient(pChannel->getLogicalConnectionKey());
        }
    }


    if (typeId == AnalyticsProtocol::LogoutRequest)
    {
        pChannel->Close(false);
    }

    if (typeId == AnalyticsProtocol::ConsoleCommandRequest)
    {
        std::string command = requestPacket.getArgumentAsText("command");

        XMLPacket response(AnalyticsProtocol::ConsoleCommandResponse);

        response.setArgumentAsText("client", requestPacket.getArgumentAsText("client"));

        if (command == "about")
        {
            std::string str = pServerImpl->getServerInfos() + "\nBased on PushFramework version 1.0";
            response.setArgumentAsText("console", str.c_str());
        }

        if (command == "profiling enable")
        {
            std::string str;
            if (pServerImpl->getProfilingStatus() == true)
            {
                str = "Profiling is already enabled.";
            }
            else
            {
                pServerImpl->enableProfiling(-1);
                str = "Profiling was enabled.";
            }
            response.setArgumentAsText("console", str.c_str());
        }

        if (command == "profiling disable")
        {
            std::string str;
            if (pServerImpl->getProfilingStatus() == false)
            {
                str = "Profiling is already disabled.";
            }
            else
            {
                pServerImpl->disableProfiling();
                str = "Profiling was disabled.";
            }
            response.setArgumentAsText("console", str.c_str());
        }

        if (command == "profiling status")
        {
            response.setArgumentAsText("console", pServerImpl->getProfilingStatus() == false ? "Profiling was found to be disabled." :
                                       "Profiling was found to be enabled.");
        }

        char pOut[256];

        bool bRet = pServerImpl->getFacade()->handleMonitorRequest(command.c_str(), pOut);

        if (bRet)
        {
            response.setArgumentAsText("console", pOut);
        }

        pChannel->PushPacket(&response);
    }

}

void Dispatcher::ProcessMonitorFirstPacket(PhysicalConnection* pChannel, IncomingPacket& packet)
{
    XMLPacket& requestPacket = (XMLPacket&) packet;

    //
    XMLPacket response(AnalyticsProtocol::LoginResponse);

    std::string password = requestPacket.getArgumentAsText("password");

    if (password == pServerImpl->getMonitorPassword())
    {

        static int observerId = 1;
        observerId++;
        std::stringstream ss;
        ss << observerId;



        if (channelFactory.RemoveFromUnattachedList(pChannel))
        {
            pChannel->attachToClient(ss.str().c_str());
            //
            monitorBroadcastManager.AddMonitor(pChannel);
            //
            response.setArgumentAsText("result", "ok");
            pChannel->PushPacket(&response);
        }
        else
        {
            //Structure was removed by closeUnlogged.
        }
    }
    else
    {
        //Reply
        XMLPacket response(AnalyticsProtocol::LoginResponse);
        response.setArgumentAsText("result", "pass");
        pChannel->PushPacket(&response);
        //pChannel->Close(true);
    }
}

}

