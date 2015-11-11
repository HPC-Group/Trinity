#pragma once

#include "WebsocketClientFactory.h"

#include "EchoService.h"
#include "RoutedCommunicationService.h"
#include "GroupCommunicationService.h"
#include "StreamedCommunicationService.h"
#include <process.h>

class WebsocketServer : public Server
{
public:
	WebsocketServer()
	{
		//
	}
	~WebsocketServer()
	{
		//
	}
protected:
	virtual void disposeOutgoingPacket(OutgoingPacket* pPacket)
	{
		delete pPacket;
	}
};
