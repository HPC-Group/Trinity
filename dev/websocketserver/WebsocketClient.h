#pragma once

class WebsocketClient : public LogicalConnection
{
public:
    WebsocketClient(std::string pseudo);
    ~WebsocketClient(void);
    CLIENT_KEY getKey();
private:
    std::string pseudo;
};
