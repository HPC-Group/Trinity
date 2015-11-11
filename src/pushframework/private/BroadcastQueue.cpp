/********************************************************************
	File :			BroadcastQueue.cpp
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
#include "BroadcastQueue.h"

#include "ScopedLock.h"
#include "../include/Protocol.h"
#include "../include/OutgoingPacket.h"

#include "BroadcastManagerImplBase.h"

namespace PushFramework
{


BroadcastQueue::BroadcastQueue(BroadcastManagerImplBase* pManager, std::string channelName, bool requireRegistration)
{
    this->pManager = pManager;
    this->channelName = channelName;
    this->requireRegistration = requireRegistration;

    pTail = NULL;
    pHead = NULL;
    ::InitializeCriticalSection(&cs);
    nPackets = 0;
}

BroadcastQueue::~BroadcastQueue(void)
{
    ::DeleteCriticalSection(&cs);
}

void BroadcastQueue::setMaxPackets( unsigned int maxPacket )
{
    this->maxPacket = maxPacket;
}


void BroadcastQueue::pushPacket( OutgoingPacket* pPacket, BROADCASTPACKET_KEY killKey/*=""*/, int objectCategory /*=0*/ )
{
    ScopedLock csLock(cs);

    PacketInfo* pPacketInfo = new PacketInfo(pPacket, killKey, objectCategory, pTail);

    pTail = pPacketInfo;
    if (pHead == NULL)
    {
        pHead = pPacketInfo;
    }

    nPackets++;
    if (nPackets > maxPacket)
    {
        InternalRemove(pHead);
    }
}

void BroadcastQueue::removePacket( BROADCASTPACKET_KEY killKey/*=""*/, int objectCategory /*=0*/ )
{
    ScopedLock csLock(cs);


    PacketInfo* pPacketInfo = pHead;
    while (pPacketInfo && pPacketInfo->killKey!= killKey && pPacketInfo->objectCategory!=objectCategory)
    {
        pPacketInfo = pPacketInfo->pNext;
    }

    if (pPacketInfo)
    {
        InternalRemove(pPacketInfo);
    }
}

void BroadcastQueue::subscribeClient( std::string clientKey )
{
    ScopedLock csLock(cs);

    clientMap[clientKey] = 0;
}

void BroadcastQueue::unsubscribeClient( std::string clientKey )
{
    ScopedLock csLock(cs);

    clientMapT::iterator it = clientMap.find(clientKey);
    if (it!=clientMap.end())
        clientMap.erase(it);
}

void BroadcastQueue::disposePacket( PacketInfo* pPacketInfo, std::string clientKey, bool bSuccess /*= true*/ )
{
    ScopedLock csLock(cs);

    if(bSuccess)
        clientMap[clientKey] = pPacketInfo->queuePosition;

    //In all cases dec refCount :
    pPacketInfo->refCount--;


    //Check if item is in internal garbage list :
    if (pPacketInfo->bWaitingForRemoval && pPacketInfo->refCount == 0)
    {
        std::vector<PacketInfo*>::iterator it = removedList.begin();
        while (it!=removedList.end())
        {
            if (pPacketInfo == (*it))
            {
                delete pPacketInfo;
                it = removedList.erase(it);
            }
            else
                it++;
        }
    }
}

OutgoingPacket* BroadcastQueue::getNextPacket( std::string clientKey, PacketInfo*& pPacketInfo )
{
    ScopedLock csLock(cs);

    clientMapT::iterator cIt = clientMap.find(clientKey);
    if(cIt == clientMap.end() && requireRegistration)
        return NULL;//user not subscribed to this channel.


    int previouslyInsertedId = 0;
    if (cIt != clientMap.end())
    {
        previouslyInsertedId = cIt->second;
    }


    PacketInfo* pSearchItem = pHead;
    while (pSearchItem && pSearchItem->queuePosition <= previouslyInsertedId)
    {
        pSearchItem = pSearchItem->pNext;
    }

    if (pSearchItem)
    {
        pSearchItem->refCount++;
        pPacketInfo = pSearchItem;
        return pSearchItem->pPacket;
    }
    return NULL;
}


void BroadcastQueue::disposeAllPackets()
{
    //
}

std::string BroadcastQueue::getChannelName()
{
    return channelName;
}

void BroadcastQueue::InternalRemove( PacketInfo* pPacketInfo )
{
    if (pPacketInfo->pNext)
    {
        pPacketInfo->pNext->pPrevious = pPacketInfo->pPrevious;
    }
    if (pPacketInfo->pPrevious)
    {
        pPacketInfo->pPrevious->pNext = pPacketInfo->pNext;
    }

    if (pPacketInfo == pHead)
    {
        pHead = pPacketInfo->pNext;
    }
    if (pPacketInfo == pTail)
    {
        pTail = pPacketInfo->pPrevious;
    }

    nPackets --;


    if(pPacketInfo->refCount==0)
    {
        pManager->DeleteOutgoingPacket(pPacketInfo->pPacket);

        delete pPacketInfo;
    }
    else
    {
        removedList.push_back(pPacketInfo);
        pPacketInfo->bWaitingForRemoval = true;
    }
}

}

