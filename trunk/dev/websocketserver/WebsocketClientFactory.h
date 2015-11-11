#pragma once

class WebsocketClientFactory : public ClientFactory
{
public:
    WebsocketClientFactory(void);
    ~WebsocketClientFactory(void);
    virtual OutgoingPacket* onNewConnection(ConnectionContext*& lpConnectionContext);
    virtual void onClientDisconnected(LogicalConnection* pClient);
    virtual void onBeforeDisposeClient(LogicalConnection* pClient)
    {
        //
    }

    virtual void onClientReconnected(LogicalConnection* pClient)
    {
        //
    }
    virtual void onClientConnected(LogicalConnection* pClient)
    {
        //
    }
    virtual void disposeClient(LogicalConnection* pClient);
    virtual int onFirstRequest(IncomingPacket& request, ConnectionContext* pConnectionContext, LogicalConnection*& lpClient, OutgoingPacket*& lpPacket);

};
