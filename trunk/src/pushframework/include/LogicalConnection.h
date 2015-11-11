/********************************************************************
	File :			LogicalConnection.h
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
#ifndef LogicalConnection__INCLUDED
#define LogicalConnection__INCLUDED

#pragma once

#include "PushFramework.h"

#include "Protocol.h"

namespace PushFramework
{

class OutgoingPacket;
class Dispatcher;
class ClientFactoryImpl;
class PhysicalConnection;
class GarbageCollector;
class BroadcastQueueInfo;
class BroadcastStreamer;


/**
	Represent a client logged into the server. A client is  .
	*/
class PUSHFRAMEWORK_API LogicalConnection
{
    friend class Dispatcher;
    friend class ClientFactoryImpl;
    friend class GarbageCollector;
    friend class BroadcastStreamer;
public:
    LogicalConnection(void);
    ~LogicalConnection(void);

    /**
    	\param pPacket a reference to the packet to be sent.

    	Sends a packet through the connection attached to this Client instance. If buffer is full, packet is rejected and function returns false.
    */
    SendResult PushPacket(OutgoingPacket* pPacket, bool binary);
    SendResult TryPushPacket(OutgoingPacket* pPacket);
    /**
    	Returns Client identifier.
    */
    virtual const char* getKey() = 0;
    /**
    	\param ip Client IP address.
    	\param ip Client port numer.

    	Retrieves the current client address.
    */
    bool getChannelInfos(std::string& ip, unsigned int& port);

    double getVisitDuration();

    void Return();

private:
    void IncrementUsage();
    void DecrementUsage();
    bool CanDelete();
private:
    PhysicalConnection* pPhysicalConnection;
    time_t dtConnectTime;
    int nRefrenceCounter;
    //
};

}

#endif // LogicalConnection__INCLUDED
