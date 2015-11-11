/********************************************************************
	File :			BroadcastManager.cpp
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
#include "../include/BroadcastManager.h"

#include "BroadcastManagerImpl.h"

namespace PushFramework
{

PUSHFRAMEWORK_API BroadcastManager broadcastManager;

BroadcastManager::BroadcastManager(void)
{
}

BroadcastManager::~BroadcastManager(void)
{
}

void BroadcastManager::CreateQueue( BROADCASTQUEUE_NAME channelKey, unsigned int maxPacket, bool requireSubscription, unsigned int uPriority, unsigned int uPacketQuota )
{
    broadcastManagerImpl.CreateBroadcastQueue(channelKey, maxPacket, requireSubscription, uPriority, uPacketQuota);
}

void BroadcastManager::RemoveQueue( BROADCASTQUEUE_NAME channelKey )
{
    broadcastManagerImpl.RemoveBroadcastQueue(channelKey);
}

bool BroadcastManager::SubscribeConnectionToQueue( CLIENT_KEY clientKey, BROADCASTQUEUE_NAME channelKey )
{
    return broadcastManagerImpl.SubscribeClient(clientKey, channelKey);
}

bool BroadcastManager::UnsubscribeConnectionFromQueue( CLIENT_KEY clientKey, BROADCASTQUEUE_NAME channelKey )
{
    return broadcastManagerImpl.UnsubscribeClient(clientKey, channelKey);
}

void BroadcastManager::PushPacket( OutgoingPacket* pPacket, BROADCASTQUEUE_NAME channelName, BROADCASTPACKET_KEY killKey, int objectCategory )
{
    broadcastManagerImpl.PushPacket(pPacket, channelName, killKey, objectCategory);
}

void BroadcastManager::PushPacket( OutgoingPacket* pPacket, BROADCASTQUEUE_NAME channelName )
{
    broadcastManagerImpl.PushPacket(pPacket, channelName, "", 0);
}

void BroadcastManager::RemovePacket( BROADCASTPACKET_KEY killKey, int objectCategory, BROADCASTQUEUE_NAME channelKey )
{
    broadcastManagerImpl.RemovePacket(killKey, objectCategory, channelKey);
}

void BroadcastManager::RemoveConnection( CLIENT_KEY clientKey )
{
    broadcastManagerImpl.RemoveClient(clientKey);
}
}

