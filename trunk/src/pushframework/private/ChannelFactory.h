/********************************************************************
	File :			ChannelFactory.h
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
#ifndef ChannelFactory__INCLUDED
#define ChannelFactory__INCLUDED

#pragma once

#include "PhysicalConnection.h"


namespace PushFramework
{


class ServerImpl;
class Listener;


typedef struct IPRange
{
    IN_ADDR startIP;
    IN_ADDR endIP;
} IPRange;

class ChannelFactory
{
public:
    ChannelFactory();
    ~ChannelFactory(void);

    void setMaxAllowedConnections(unsigned int nMaxConnections);

    bool SetupClientConnection(SOCKET hSocket, SOCKADDR_IN address, Listener* pListener);
    bool SetupDashboardConnection(SOCKET hSocket, SOCKADDR_IN address, Listener* pListener);

    void setLoginExpiryDuration(unsigned int uLoginExpireDuration);

    void addIPRangeAccess(const char* ipStart, const char* ipStop, bool bPermit);

    void AddToConnectedList(PhysicalConnection* pChannel);
    bool RemoveFromUnattachedList(PhysicalConnection* pChannel);

    void CloseNonLogged();
private:
    //
    CRITICAL_SECTION csChannelMap;
    vectConnectionsT vectPendingPhysicalConnections;


    //
    unsigned int uLoginExpireDuration;

private:
    unsigned int nMaxConnections;

    bool SetupPhysicalConnection(SOCKET hSocket, ListenerOptions* pListenerOptions, SOCKADDR_IN address, bool isObserver);
    bool isAddressAllowed(IN_ADDR	SockAddr);

    typedef std::vector<IPRange> ipRangeListT;
    ipRangeListT blockedIPs;
    ipRangeListT permittedIPs;
    //
    bool isPermitOnly;
};
extern ChannelFactory channelFactory;
}

#endif // ChannelFactory__INCLUDED
