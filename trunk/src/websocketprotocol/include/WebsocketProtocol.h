#pragma once
#include "Symbols.h"

class WebsocketHandshakeMessage;
// This class is exported from the WebsocketProtocol.dll
class WEBSOCKETPROTOCOL_API WebsocketProtocol : public Protocol {
public:
	WebsocketProtocol(void);
	// TODO: add your methods here.
    virtual void disposeOutgoingPacket(PushFramework::OutgoingPacket* pPacket)
    {
        delete pPacket;
    }
    virtual void disposeIncomingPacket(PushFramework::IncomingPacket* pPacket)
    {
        delete pPacket;
    }


    virtual int encodeOutgoingPacket(PushFramework::OutgoingPacket& packet);
    virtual int frameOutgoingPacket(PushFramework::OutgoingPacket& packet, PushFramework::DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary);

    virtual int tryDeframeIncomingPacket(PushFramework::DataBuffer& buffer, PushFramework::IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* pContext);
    virtual int decodeIncomingPacket(PushFramework::IncomingPacket* pPacket, int& serviceId);

    static bool ProcessHandshake( WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response );
	

};

