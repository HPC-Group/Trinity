#include "StdAfx.h"
#include "WebsocketClientFactory.h"
#include "WebsocketClient.h"



WebsocketClientFactory::WebsocketClientFactory(void)
{
}

WebsocketClientFactory::~WebsocketClientFactory(void)
{
}

int WebsocketClientFactory::onFirstRequest( IncomingPacket& _request, ConnectionContext* pConnectionContext, LogicalConnection*& lpClient, OutgoingPacket*& lpPacket )
{
	//received messages belong to a physical connection that still did not transform into a logical connection :
	//understand in which stage we are :
	WebsocketConnectionContext* pCxt = (WebsocketConnectionContext*) pConnectionContext;

	if (pCxt->GetStage() == WebsocketConnectionContext::HandshakeStage)
	{
		WebsocketHandshakeMessage& request = (WebsocketHandshakeMessage&) _request;
		if (!request.Parse())
		{
			return ClientFactory::RefuseAndClose;
		}

		WebsocketHandshakeMessage *pResponse = new WebsocketHandshakeMessage();
		if (WebsocketProtocol::ProcessHandshake(request, *pResponse))
		{
            lpPacket = pResponse;
			pCxt->SetStage(WebsocketConnectionContext::LoginStage);
		}
		return ClientFactory::RefuseRequest; // Will not close the connection, but we still wait for login message to create a logical client.
	}

	if (pCxt->GetStage() == WebsocketConnectionContext::LoginStage)
	{
		WebsocketDataMessage& request = (WebsocketDataMessage&) _request;

		WebsocketClient* pClient = new WebsocketClient(request.GetArg1());
		lpClient = pClient;

        WebsocketDataMessage *pResponse = new WebsocketDataMessage(LoginCommunication);
        pResponse->SetArguments("Welcome " + request.GetArg1());
        lpPacket = pResponse;

		pCxt->SetStage(WebsocketConnectionContext::ConnectedStage);

		return ClientFactory::CreateClient;
	}

	//Impossible to come here.
}

void WebsocketClientFactory::onClientDisconnected( LogicalConnection* pClient )
{
	Disconnect(pClient, false);
}

void WebsocketClientFactory::disposeClient( LogicalConnection* pClient )
{
	delete pClient;
}

OutgoingPacket* WebsocketClientFactory::onNewConnection( ConnectionContext*& pConnectionContext )
{
	pConnectionContext = new WebsocketConnectionContext();
	return NULL;
}
