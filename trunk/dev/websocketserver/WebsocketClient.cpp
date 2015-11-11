#include "StdAfx.h"
#include "WebsocketClient.h"

WebsocketClient::WebsocketClient(std::string pseudo)
{
    this->pseudo =  pseudo;
}

WebsocketClient::~WebsocketClient(void)
{
}

PushFramework::CLIENT_KEY WebsocketClient::getKey()
{
    return pseudo.c_str();
}
