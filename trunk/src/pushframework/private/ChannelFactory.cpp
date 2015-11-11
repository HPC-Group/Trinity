/********************************************************************
	File :			ChannelFactory.cpp
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
#include "ChannelFactory.h"

#include "ServerImpl.h"

#ifdef Plateform_Windows
#include "win\IOQueue_Win.h"
#else
#ifdef Plateform_Linux
#include "linux\IOQueue_Linux.h"
#endif
#endif

#include "Dispatcher.h"
#include "Listener.h"
#include "ScopedLock.h"
#include "ClientFactoryImpl.h"


namespace PushFramework
{

ChannelFactory channelFactory;

ChannelFactory::ChannelFactory()
{
    ::InitializeCriticalSection(&csChannelMap);

    uLoginExpireDuration = 40;
    nMaxConnections = 100;

    isPermitOnly = false;
}

ChannelFactory::~ChannelFactory(void)
{
    ::DeleteCriticalSection(&csChannelMap);
}

void ChannelFactory::setLoginExpiryDuration( unsigned int uLoginExpireDuration )
{
    this->uLoginExpireDuration = uLoginExpireDuration;
}

void ChannelFactory::setMaxAllowedConnections( unsigned int nMaxConnections )
{
    this->nMaxConnections = nMaxConnections;
}



bool ChannelFactory::SetupClientConnection( SOCKET hSocket, SOCKADDR_IN address, Listener* pListener )
{
    if (clientFactoryImpl.GetClientCount() >= nMaxConnections)
    {
        return false;
    }

    //Check if address is not blocked :
    if (!isAddressAllowed(address.sin_addr))
    {
        return false;
    }

    //
    return SetupPhysicalConnection(hSocket, &pListener->getOptions(), address, false);
}

bool ChannelFactory::SetupDashboardConnection( SOCKET hSocket, SOCKADDR_IN address, Listener* pListener )
{
    return SetupPhysicalConnection(hSocket, &pListener->getOptions(), address, true);
}

bool ChannelFactory::isAddressAllowed( IN_ADDR SockAddr )
{
    long ip = SockAddr.s_addr;

    bool bAllow = false;

    ipRangeListT& list = isPermitOnly ? permittedIPs : blockedIPs;
    bool bIsInList = false;

    for (int i=0; i<list.size(); i++)
    {
        IPRange& ipRange = list.at(i);

        long start = ipRange.startIP.s_addr;
        long stop = ipRange.endIP.s_addr;


        if (ip <= stop && ip >= start)
        {
            bIsInList = true;
            break;
        }
    }
    return isPermitOnly ? bIsInList : !bIsInList;
}

void ChannelFactory::addIPRangeAccess( const char* ipStart, const char* ipStop, bool bPermit )
{
    isPermitOnly = bPermit;

    IPRange ipRange;

    ipRange.startIP.s_addr = 0;
    ipRange.startIP.s_addr = inet_addr(ipStart);

    ipRange.endIP.s_addr = 0;
    ipRange.endIP.s_addr = inet_addr(ipStop);

    ipRangeListT& list = bPermit ? permittedIPs : blockedIPs;

    list.push_back(ipRange);
}

void ChannelFactory::AddToConnectedList( PhysicalConnection* pChannel )
{
    ScopedLock lock(csChannelMap);
    vectPendingPhysicalConnections.push_back(pChannel);
}

bool ChannelFactory::RemoveFromUnattachedList( PhysicalConnection* pChannel )
{
    ScopedLock lock(csChannelMap);

    vectConnectionsT::iterator it = vectPendingPhysicalConnections.begin();

    while (it != vectPendingPhysicalConnections.end())
    {
        if ((*it) == pChannel)
        {
            vectPendingPhysicalConnections.erase(it);
            return true;
        }
        it ++;
    }

    return false;
}

void ChannelFactory::CloseNonLogged()
{
    ScopedLock lock(csChannelMap);

    vectConnectionsT::iterator it = vectPendingPhysicalConnections.begin();

    while (it != vectPendingPhysicalConnections.end())
    {
        PhysicalConnection* pChannel = *it;

        int nMaxDuration = pChannel->isObserverChannel() ? 40 : uLoginExpireDuration;
        if (pChannel->getLifeDuration() > nMaxDuration)
        {
            pChannel->Close(false);
        }
        it++;
    }
}

}
