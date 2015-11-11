/********************************************************************
	File :			BroadcastManagerImplBase.cpp
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
#include "BroadcastManagerImplBase.h"

#include "../include/Protocol.h"
#include "../include/OutgoingPacket.h"
#include "../include/LogicalConnection.h"
#include "BroadcastQueue.h"
#include "BroadcastContext.h"

namespace PushFramework
{


BroadcastManagerImplBase::BroadcastManagerImplBase(void)
{
    pChainHead = NULL;
}

BroadcastManagerImplBase::~BroadcastManagerImplBase(void)
{
    delete pChainHead;
}

void BroadcastManagerImplBase::CreateBroadcastQueue( BROADCASTQUEUE_NAME channelKey, unsigned int maxPacket, bool requireSubscription, unsigned int uPriority, unsigned int uPacketQuota )
{
    BroadcastQueue* pQueue = new BroadcastQueue(this, channelKey, requireSubscription);
    pQueue->setMaxPackets(maxPacket);

    BroadcastQueueInfo* pPushInfo = insertChannelIntoOrderedChain(pQueue, uPriority, uPacketQuota);

    channelMap[channelKey] = pPushInfo;
}

void BroadcastManagerImplBase::RemoveBroadcastQueue( BROADCASTQUEUE_NAME channelKey )
{
    channelMapT::iterator it = channelMap.find(channelKey);
    if(it!=channelMap.end())
    {

        BroadcastQueueInfo* pPushInfo = it->second;
        delete pPushInfo;

        channelMap.erase(it);
    }
}

bool BroadcastManagerImplBase::SubscribeClient( CLIENT_KEY clientKey, BROADCASTQUEUE_NAME channelKey )
{
    channelMapT::iterator it = channelMap.find(channelKey);
    if(it==channelMap.end())
        return false;
    //
    BroadcastQueue* packetQueue = it->second->pChannel;
    packetQueue->subscribeClient(clientKey);
    return true;
}

bool BroadcastManagerImplBase::UnsubscribeClient( CLIENT_KEY clientKey, BROADCASTQUEUE_NAME channelKey )
{
    channelMapT::iterator it = channelMap.find(channelKey);
    if(it==channelMap.end())
        return false;
    //
    BroadcastQueue* packetQueue = it->second->pChannel;
    packetQueue->unsubscribeClient(clientKey);
    return true;
}

void BroadcastManagerImplBase::PushPacket( OutgoingPacket* pPacket, BROADCASTQUEUE_NAME channelName, BROADCASTPACKET_KEY killKey, int objectCategory )
{
    channelMapT::iterator it = channelMap.find(channelName);
    if(it==channelMap.end())
        return;

    PreEncodeOutgoingPacket(pPacket);
    ReportOnBeforePushPacket(channelName);
    //
    BroadcastQueue* packetQueue = it->second->pChannel;
    packetQueue->pushPacket(pPacket, killKey, objectCategory);

    ActivateSubscribers(channelName);
}

void BroadcastManagerImplBase::RemovePacket( BROADCASTPACKET_KEY killKey, int objectCategory, BROADCASTQUEUE_NAME channelKey )
{
    channelMapT::iterator it = channelMap.find(channelKey);
    if(it==channelMap.end())
        return;
    //
    BroadcastQueue* packetQueue = it->second->pChannel;
    packetQueue->removePacket(killKey, objectCategory);
}



void BroadcastManagerImplBase::RemoveClient( CLIENT_KEY clientKey )
{
    for (channelMapT::iterator it = channelMap.begin();
            it!=channelMap.end();
            it++)
    {
        BroadcastQueue* pChannel = it->second->pChannel;
        pChannel->unsubscribeClient(clientKey);
    }
}



BroadcastQueueInfo* BroadcastManagerImplBase::insertChannelIntoOrderedChain( BroadcastQueue* pQueue, unsigned int uPriority, unsigned int uPacketQuota )
{
    BroadcastQueueInfo* pChannelInfo = new BroadcastQueueInfo(pQueue, uPacketQuota);


    //if pChainHead is Null, create one and insert channel there.

    if (pChainHead == NULL)
    {
        pChainHead = new BroadcastQueueGroupInfo(uPriority);
        pChainHead->insertChannelInfo(pChannelInfo);
        return pChannelInfo;
    }

    //Search for group between

    BroadcastQueueGroupInfo* pBefore = NULL;
    BroadcastQueueGroupInfo* pAfter = pChainHead;

    while(pAfter!=NULL)
    {
        if (uPriority >= pAfter->uPriority)
        {
            break;
        }
        else
        {
            //Update before and next :
            pBefore = pAfter;
            pAfter = pAfter->pNextGroup;
        }
    }




    BroadcastQueueGroupInfo* pParentGroup = NULL;



    if (pAfter==NULL)
    {
        //
        BroadcastQueueGroupInfo* pNewGroup = new BroadcastQueueGroupInfo(uPriority);
        pBefore->pNextGroup = pNewGroup;

        pParentGroup = pNewGroup;
    }
    else if (uPriority == pAfter->uPriority)
    {
        pParentGroup = pAfter;
    }
    else
    {
        BroadcastQueueGroupInfo* pNewGroup = new BroadcastQueueGroupInfo(uPriority);
        pNewGroup->pNextGroup = pAfter;

        if (pBefore)
        {
            pBefore->pNextGroup = pNewGroup;
        }
        else
        {
            pChainHead = pNewGroup;
        }

        pParentGroup = pNewGroup;

    }


    pParentGroup->insertChannelInfo(pChannelInfo);
    return pChannelInfo;
}

OutgoingPacket* BroadcastManagerImplBase::getPacketFromGroupChain( BroadcastQueueInfo* pStartAt , CLIENT_KEY client, PacketInfo*& pPacketInfo, BroadcastQueueInfo*& pFoundInfo )
{
    OutgoingPacket* pPacket = NULL;

    BroadcastQueueInfo* pNextChannelInfo = pStartAt;

    while (pNextChannelInfo)
    {
        pFoundInfo = pNextChannelInfo;

        pPacket = pNextChannelInfo->pChannel->getNextPacket(client, pPacketInfo);

        if(pPacket!=NULL)
            return pPacket;

        pNextChannelInfo = pNextChannelInfo->pNext;
    }

    return NULL;
}

void BroadcastManagerImplBase::DisposeAllPackets()
{
    for (channelMapT::iterator it = channelMap.begin();
            it!=channelMap.end();
            it++)
    {
        BroadcastQueue* pChannel = it->second->pChannel;
        pChannel->disposeAllPackets();
    }
}

std::string BroadcastManagerImplBase::GetQueuesNames()
{
    std::stringstream ss;
    ss << std::noskipws;

    for (channelMapT::iterator it = channelMap.begin();
            it!=channelMap.end();
            it++)
    {
        ss << "<queue val=\"" << it->first << "\"/>";
    }

    return ss.str();
}


OutgoingPacket* BroadcastManagerImplBase::GetNextPacket(std::string subscriberKey, const BroadcastContext& broadcastCtxt , PacketInfo*& pPacketInfo, BroadcastQueueInfo*& pFoundInfo )
{
    OutgoingPacket* pPacket = NULL;

    if (broadcastCtxt.pCurrentSinkChannel)
    {
        pPacket = getPacketFromGroupChain(broadcastCtxt.pCurrentSinkChannel, subscriberKey.c_str(), pPacketInfo, pFoundInfo);

        if (pPacket)
            return pPacket;
    }

    //Nothing is found, so go up!

    BroadcastQueueGroupInfo* pGroup = pChainHead;
    while (pGroup)
    {
        pPacket = getPacketFromGroupChain(pGroup->pItemList, subscriberKey.c_str(), pPacketInfo, pFoundInfo);

        if (pPacket)
            return pPacket;

        pGroup = pGroup->pNextGroup;
    }

    return NULL;
}

void BroadcastManagerImplBase::DisposePacket(PacketInfo* pPacketInfo,std::string subscriberKey, BroadcastQueueInfo* pChannelInfo, BroadcastContext& broadcastCtxt, bool bSuccess )
{
    ReportOnAfterPacketIsSent(pChannelInfo->pChannel->getChannelName(), subscriberKey);
    //
    pChannelInfo->pChannel->disposePacket(pPacketInfo, subscriberKey, bSuccess);

    if (bSuccess)
    {
        //QoS
        if (broadcastCtxt.pCurrentSinkChannel == pChannelInfo)
        {
            broadcastCtxt.nSentPackets++;
            if (broadcastCtxt.nSentPackets == pChannelInfo->uPacketQuota)
            {
                if (pChannelInfo->pNext)
                {
                    broadcastCtxt.pCurrentSinkChannel = pChannelInfo->pNext;
                    broadcastCtxt.nSentPackets = 0;
                }
                else
                {
                    broadcastCtxt.pCurrentSinkChannel = NULL;
                    broadcastCtxt.nSentPackets = 0;
                }

            }
        }
        else
        {
            broadcastCtxt.nSentPackets = 1;
            broadcastCtxt.pCurrentSinkChannel = pChannelInfo;
        }

    }
}


}
