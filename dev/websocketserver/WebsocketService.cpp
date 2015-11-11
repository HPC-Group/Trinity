#include "StdAfx.h"
#include "WebsocketService.h"

#include "WebsocketClient.h"

WebsocketService::WebsocketService(void)
{
}

WebsocketService::~WebsocketService(void)
{
}

void WebsocketService::handle( LogicalConnection* pClient, IncomingPacket* pRequest )
{
    WebsocketDataMessage& request = (WebsocketDataMessage&)(*pRequest);
    WebsocketClient& client = (WebsocketClient&) (*pClient);

    cout << "received data packet :" << endl;
    cout << request.Encode() << endl;


    WebsocketDataMessage response("reply from server");
    client.PushPacket(&response, false);
}
