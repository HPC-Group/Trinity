#include "StdAfx.h"
#include "StreamedCommunicationService.h"
#include "WebsocketClient.h"
#include "WebsocketDataMessage.h"


StreamedCommunicationService::StreamedCommunicationService(void)
{
}


StreamedCommunicationService::~StreamedCommunicationService(void)
{
}

void StreamedCommunicationService::handle( LogicalConnection* pClient, IncomingPacket* pRequest )
{
	WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
	WebsocketClient& client = (WebsocketClient&) (*pClient);

	string opType = request.GetArg1();

	if (opType == "subscribe")
	{
		broadcastManager.SubscribeConnectionToQueue(client.getKey(), "streamingQueue");
	}

	if (opType == "unsubscribe")
	{
		broadcastManager.UnsubscribeConnectionFromQueue(client.getKey(), "streamingQueue");
	}
}
