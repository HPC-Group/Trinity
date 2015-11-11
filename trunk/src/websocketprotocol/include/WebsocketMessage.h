

#pragma once
#include "Symbols.h"
// This class is exported from the WebsocketProtocol.dll
class WEBSOCKETPROTOCOL_API WebsocketMessage : public IncomingPacket, public OutgoingPacket {
public:
	enum
	{
		Handshake = 0,
		DataMessage,
	};
	WebsocketMessage(int type)
	{
		this->type = type;
	}
	~WebsocketMessage(void)
	{
		//
	}
	int GetType()
	{
		return type;
	}
private:
	int type;
};
