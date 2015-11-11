// WebsocketProtocol.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "WebsocketProtocol.h"
#include "base64/base64.h"
#include "sha1/sha1.h"


#include "WebsocketHandshakeMessage.h"
#include "WebsocketDataMessage.h"
#include "WebsocketConnectionContext.h"




// This is the constructor of a class that has been exported.
// see WebsocketProtocol.h for the class definition
WebsocketProtocol::WebsocketProtocol()
{
	return;
}

int WebsocketProtocol::tryDeframeIncomingPacket( PushFramework::DataBuffer& buffer, PushFramework::IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* pContext )
{
    if (buffer.GetDataSize() == 0)
        return Protocol::eIncompletePacket;

	WebsocketConnectionContext* pCxt = (WebsocketConnectionContext*) pContext;

	if (pCxt->GetStage() == WebsocketConnectionContext::HandshakeStage)
	{
		WebsocketHandshakeMessage* pMessage = new WebsocketHandshakeMessage(buffer.GetBuffer(), buffer.GetDataSize());
		serviceId = 0;
		nExtractedBytes = buffer.GetDataSize();
		pPacket = pMessage;
		return Protocol::Success;
	}

	//In the other cases, we should expect a data message : 
    int nMinExpectedSize = 6;
    if (buffer.GetDataSize() < nMinExpectedSize)
        return Protocol::eIncompletePacket;

    BYTE payloadFlags = buffer.getAt(0);
    if (payloadFlags != 129)
        return Protocol::eUndefinedFailure;

    BYTE basicSize = buffer.getAt(1) & 0x7F;
    unsigned __int64 payloadSize;
    int masksOffset;

    if (basicSize <= 125)
    {
        payloadSize = basicSize;
        masksOffset = 2;
    }
    else if (basicSize == 126)
    {
        nMinExpectedSize += 2;
        if (buffer.GetDataSize() < nMinExpectedSize)
            return Protocol::eIncompletePacket;
        payloadSize = ntohs( *(u_short*) (buffer.GetBuffer() + 2) );
        masksOffset = 4;
    }
    else if (basicSize == 127)
    {
        nMinExpectedSize += 8;
        if (buffer.GetDataSize() < nMinExpectedSize)
            return Protocol::eIncompletePacket;
        payloadSize = ntohl( *(u_long*) (buffer.GetBuffer() + 2) );
        masksOffset = 10;
    }
    else
        return Protocol::eUndefinedFailure;

    nMinExpectedSize += payloadSize;
    if (buffer.GetDataSize() < nMinExpectedSize)
        return Protocol::eIncompletePacket;

    BYTE masks[4];
    memcpy(masks, buffer.GetBuffer() + masksOffset, 4);

    char* payload = new char[payloadSize + 1];
    memcpy(payload, buffer.GetBuffer() + masksOffset + 4, payloadSize);
    for (unsigned __int64 i = 0; i < payloadSize; i++) {
        payload[i] = (payload[i] ^ masks[i%4]);
    }
	payload[payloadSize] = '\0';
  
	WebsocketDataMessage* pMessage = new WebsocketDataMessage(payload);
	serviceId = 1;
	nExtractedBytes = nMinExpectedSize;
	pPacket = pMessage;

	delete payload;
	return Protocol::Success;
}

int WebsocketProtocol::frameOutgoingPacket( PushFramework::OutgoingPacket& packet, PushFramework::DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary )
{
	WebsocketMessage& message = (WebsocketMessage&) packet;

	if (message.GetType() == WebsocketMessage::Handshake)
	{
		WebsocketHandshakeMessage& handshake = (WebsocketHandshakeMessage&) message;

        string strRaw = handshake.Serialize();
        if(strRaw.size() > buffer.getRemainingSize())
            return Protocol::eInsufficientBuffer;

        buffer.Append((char*)strRaw.c_str(), strRaw.size());
        nWrittenBytes = strRaw.size();
        return Protocol::Success;
	}

	//https://tools.ietf.org/html/rfc6455
	if (message.GetType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage& dataMessage = (WebsocketDataMessage&) message;
        string strData = dataMessage.GetEncodedData();

		unsigned __int64 payloadSize = strData.size();

		if (binary)
			payloadSize++;

		int expectedSize = payloadSize + 1; //flags byte.
		if(payloadSize <= 125  && payloadSize <= 65535 )
			expectedSize += 1;
		else if(payloadSize > 125  && payloadSize <= 65535)
			expectedSize += 3;
		else
			expectedSize += 9;

		if (expectedSize > buffer.getRemainingSize())
			return Protocol::eInsufficientBuffer;

		//create the flags byte
		char payloadFlags = 129;

		if (binary)
			payloadFlags = 130;

		buffer.Append(&payloadFlags, 1);

		//create the length byte
		if (payloadSize <= 125)
		{
			char basicSize = payloadSize;
			buffer.Append(&basicSize, 1);
		}
		else if (payloadSize > 125 & payloadSize <= 65535)
		{
			char basicSize = 126;
			buffer.Append(&basicSize, 1);

			char len[2];
			len[0] = ( payloadSize >> 8 ) & 255;
			len[1] = ( payloadSize ) & 255;
			buffer.Append(len, 2);
		}
		else
		{
			char basicSize = 127;
			buffer.Append(&basicSize, 1);

			char len[8];
			len[0] = ( payloadSize >> 56 ) & 255;
			len[1] = ( payloadSize >> 48  ) & 255;
			len[2] = ( payloadSize >> 40 ) & 255;
			len[3] = ( payloadSize >> 32  ) & 255;
			len[4] = ( payloadSize >> 24 ) & 255;
			len[5] = ( payloadSize >> 16  ) & 255;
			len[6] = ( payloadSize >> 8 ) & 255;
			len[7] = ( payloadSize ) & 255;
			buffer.Append(len, 8);
		}

        buffer.Append((char*)strData.c_str(), strData.size());

		if (binary)
		{
			char nonAsci = 150;
			buffer.Append(&nonAsci, 1);
		}

        nWrittenBytes = expectedSize;
        return Protocol::Success;
	}
	
	return Protocol::eUndefinedFailure;
}

bool WebsocketProtocol::ProcessHandshake( WebsocketHandshakeMessage& request, WebsocketHandshakeMessage& response )
{
    std::string server_key = request.GetField("Sec-WebSocket-Key");
    server_key += "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

    SHA1        sha;
    unsigned int    message_digest[5];

    sha.Reset();
    sha << server_key.c_str();

    sha.Result(message_digest);
    // convert sha1 hash bytes to network byte order because this sha1
    //  library works on ints rather than bytes
    for (int i = 0; i < 5; i++) {
        message_digest[i] = htonl(message_digest[i]);
    }

    server_key = base64_encode(
        reinterpret_cast<const unsigned char*>
        (message_digest),20
        );


    response.SetField("Upgrade", "websocket");
    response.SetField("Connection", "Upgrade");
    response.SetField("Sec-WebSocket-Accept", server_key);

    return true;
}

int WebsocketProtocol::decodeIncomingPacket( PushFramework::IncomingPacket* pPacket, int& serviceId )
{
	WebsocketMessage* pMessage = (WebsocketMessage*) pPacket;
	if (pMessage->GetType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage* pDataMessage = (WebsocketDataMessage*) pMessage;
		if (!pDataMessage->Decode())
		{
			return Protocol::eDecodingFailure;
		}
		serviceId = pDataMessage->GetServiceId();
		return Protocol::Success;
	}
	return Protocol::Success;
}

int WebsocketProtocol::encodeOutgoingPacket( PushFramework::OutgoingPacket& packet )
{
	WebsocketMessage& message = (WebsocketMessage&) packet;
	if (message.GetType() == WebsocketMessage::DataMessage)
	{
		WebsocketDataMessage& dataMessage = (WebsocketDataMessage&) message;
		return dataMessage.Encode() ? Protocol::Success : Protocol::eEncodingFailure;
	}
	return Protocol::Success;
}




