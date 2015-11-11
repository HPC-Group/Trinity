/********************************************************************
	File :			BroadcastManager.h
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
#ifndef BroadcastManager__INCLUDED
#define BroadcastManager__INCLUDED

#pragma once

#include "PushFramework.h"

namespace PushFramework
{

class BroadcastManagerImpl;
class ClientFactoryImpl;
class Dispatcher;
class OutgoingPacket;
class ServerImpl;

/*!
    The BroadcastManager class helps manage broadcasting queues and the subscription of connected clients to them.
*/
class PUSHFRAMEWORK_API BroadcastManager
{
    friend class ClientFactoryImpl;
    friend class Dispatcher;
    friend class ServerImpl;
public:
    BroadcastManager(void);
    ~BroadcastManager(void);

    /**  @name Queues Management  **/
    //@{
    /**
    \param channelKey the name of the queue to create.
    \param maxPacket the maximum number of packets this queue can store.
    \param requireSubscription specifies whether clients should be explicitly subscribed to the queue.
    \param uPriority queue priority over other queues.
    \param uPacketQuota packets quota.

    Call CreateQueue to create a new queue.
     */
    void CreateQueue(BROADCASTQUEUE_NAME channelKey, unsigned int maxPacket, bool requireSubscription, unsigned int uPriority, unsigned int uPacketQuota);
    /**
    \param channelKey the name of the queue to delete.

    Call RemoveQueue to delete a new queue.
     */
    void RemoveQueue(BROADCASTQUEUE_NAME channelKey);

    /**  @name Subscription Management  **/
    //@{
    /**
    \param clientKey the client key.
    \param channelKey the queue key.

    Call SubscribeConnectionToQueue to subscribe a connected client to an existing queue.
     */
    bool SubscribeConnectionToQueue(CLIENT_KEY clientKey, BROADCASTQUEUE_NAME channelKey);
    /**
    \param clientKey the client key.
    \param channelKey the queue key.

    Call UnsubscribeConnectionFromQueue to unsubscribe a connected client from an existing queue.
     */
    bool UnsubscribeConnectionFromQueue(CLIENT_KEY clientKey, BROADCASTQUEUE_NAME channelKey);
    /**
    \param clientKey the client key.
 
    Call RemoveConnection to unsubscribe a connected client from all queues.
     */
    void RemoveConnection(CLIENT_KEY clientKey);

    /**  @name Data Management  **/
    //@{
    /**
    \param pPacket the packet to send.
    \param channelKey the queue key.

    Call PushPacket to push a packet through a broadcasting queue.
     */
    void PushPacket(OutgoingPacket* pPacket, BROADCASTQUEUE_NAME channelName);
     /**
     \param pPacket the packet to send.
     \param channelKey the queue key.
     \param killKey used to reference the packet for subsequent removal.
     \param objectCategory used to reference the packet for subsequent removal.

    Call PushPacket to push a packet through a broadcasting queue.
     */
    void PushPacket(OutgoingPacket* pPacket, BROADCASTQUEUE_NAME channelName, BROADCASTPACKET_KEY killKey, int objectCategory);
     /**
     \param killKey the killKey for the packet to be removed.
     \param objectCategory the objectCategory for the packet to be removed.
     \param channelKey the queue key.

    Call PushPacket to push a packet through a broadcasting queue.
     */
    void RemovePacket(BROADCASTPACKET_KEY killKey, int objectCategory, BROADCASTQUEUE_NAME channelKey);

    
};

extern PUSHFRAMEWORK_API BroadcastManager broadcastManager;
}
#endif // BroadcastManager__INCLUDED

