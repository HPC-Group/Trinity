/********************************************************************
	File :			Dispatcher.h
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
#ifndef Dispatcher__INCLUDED
#define Dispatcher__INCLUDED

#pragma once

#include "BroadcastQueue.h"

#include "Types.h"


namespace PushFramework
{

class Service;
class ServerImpl;
class PhysicalConnection;
class IncomingPacket;
class MonitorsBroadcastManager;
class LogicalConnection;

class Dispatcher
{
public:
    Dispatcher();
    ~Dispatcher(void);
    void OnWriteComplete(PhysicalConnection* pChannel,int dwIoSize);
    void OnReceiveComplete(PhysicalConnection* pChannel,int dwIoSize);
    void OnInitializeReady(PhysicalConnection* pChannel);
    void registerService(unsigned int uCommmand, Service* pService, std::string serviceName);
public:
    std::string getServiceNames();
    void setCurrentService(std::string serviceName);
    void UnsetCurrentService();
    bool getCurrentService(std::string& serviceName);
private:
    typedef struct ServiceInfo
    {
        Service* pService;
        std::string serviceName;
    } ServiceInfo;
    typedef std::map<unsigned int, ServiceInfo*> serviceMapT;
    typedef std::map<ThreadIDType, std::string> workerServiceMapT;

    //List of services :
    serviceMapT serviceMap;

    //Reference to dispatched services :
    workerServiceMapT workerServiceMap;
    CRITICAL_SECTION csSrvMap;

public:


    void NotifyObserversClientIN(const char* key, std::string peerIP, unsigned int peerPort);
    void NotifyObserversClientOut(const char* key);
private:
    void dispatchRequest(unsigned int uCommand,LogicalConnection* pClient,IncomingPacket& packet,unsigned int serviceBytes);
    void ProcessFirstPacket(PhysicalConnection* pChannel,unsigned int uCommand, IncomingPacket& packet,unsigned int serviceBytes);
    void HandleMonitorRequest(PhysicalConnection* pChannel, IncomingPacket& packet);
    void ProcessMonitorFirstPacket(PhysicalConnection* pChannel, IncomingPacket& packet);

};

extern Dispatcher dispatcher;

}

#endif // Dispatcher__INCLUDED
