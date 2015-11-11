#pragma once

class WebsocketService : public Service
{
public:
    WebsocketService(void);
    ~WebsocketService(void);

    virtual void handle(LogicalConnection* pClient, IncomingPacket* pRequest);
};
