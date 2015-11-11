#include "StdAfx.h"
#include "WebsocketConnectionContext.h"


WebsocketConnectionContext::WebsocketConnectionContext()
{
	this->stage = HandshakeStage;
}


WebsocketConnectionContext::~WebsocketConnectionContext(void)
{
}

int WebsocketConnectionContext::GetStage()
{
	return stage;
}

void WebsocketConnectionContext::SetStage( int stage )
{
	this->stage = stage;
}
