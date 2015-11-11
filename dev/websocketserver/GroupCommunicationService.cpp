#include "StdAfx.h"
#include "GroupCommunicationService.h"
#include "WebsocketClient.h"
#include "WebsocketDataMessage.h"

GroupCommunicationService::GroupCommunicationService(void)
{
}


GroupCommunicationService::~GroupCommunicationService(void)
{
}

void GroupCommunicationService::handle( LogicalConnection* pClient, IncomingPacket* pRequest )
{
	WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
	WebsocketClient& client = (WebsocketClient&) (*pClient);

	string opType = request.GetArg1();

	if (opType == "subscribe")
	{
		broadcastManager.SubscribeConnectionToQueue(client.getKey(), "groupQueue");
	}

	if (opType == "unsubscribe")
	{
		broadcastManager.UnsubscribeConnectionFromQueue(client.getKey(), "groupQueue");
	}

	if (opType == "publish")
	{
		WebsocketDataMessage* message = new WebsocketDataMessage(GroupCommunication);
		message->SetArguments(client.getKey(), request.GetArg2());
		broadcastManager.PushPacket(message, "groupQueue");
	}
}
