/********************************************************************
	File :			Protocol.h
	Creation date :	2010/6/27

	License :			Copyright 2010 Ahmed Charfeddine, http://www.pushframework.com

				   Licensed under the Apache License, Version 2.0 (the "License");
				   you may not use this file except in compliance with the License.
				   You may obtain a copy of the License at

					   http://www.apache.org/licenses/LICENSE-2.0

				   Unless required by applicable law or agreed to in writing, software
				   distributed under the License is distributed on an "AS IS" BASIS,
				   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
				   See the License for the specific language governing permissions and
				   limitations under the License.


*********************************************************************/
#ifndef Protocol__INCLUDED
#define Protocol__INCLUDED

#pragma once

#include "PushFramework.h"
#include "DataBuffer.h"
namespace PushFramework
{

class OutgoingPacket;
class IncomingPacket;
class ConnectionContext;

enum SendResult
{
    SendResult_OK,
    SendResult_Retry,
    SendResult_NotOK
};

/**
	A subclass of this class defines a communication protocol between Push Framework and remote clients (or a category of remote
	clients if you are using multiple protocols). Server business code is expected to send responses in form of OutgoingPacket
	instances. The framework requests the protocol instance to serialize these instances by triggering encoding and framing jobs.
	Incoming data is handed to the de-serialization functions which in turns calls on user-defined deframing and decoding in order
	to construct the IncomingPacket instance and its accompanying service id.
	*/
class PUSHFRAMEWORK_API Protocol
{
public:
    enum Result
    {
        Success = 0,
        eInsufficientBuffer,
        eIncompletePacket,
        eCorruptPacket,
        eEncodingFailure,
        eDecodingFailure,
        eUndefinedFailure,
        ePacketizingError,
    };

    /** @name TORs **/
    //@{
    Protocol(void);
    ~Protocol(void);
    //@}

    /** @name Consumed by Push Framework internal code. **/
    int serializeOutgoingPacket(OutgoingPacket& packet, DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary);
    int tryDeserializeIncomingPacket(DataBuffer& buffer, IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* pContext);
    //@}
public:
    /** @name Serialization. **/
    /**
    	\param packet OutgoingPacket instance to encode.

    	Override to encode the OutgoingPacket instance. Save the result into the same input.
    */
    virtual int encodeOutgoingPacket(OutgoingPacket& packet) =  0;
    /**
    \param packet OutgoingPacket instance to encode.
    \param buffer Buffer to write the result to.
    \param nWrittenBytes Report the total bytes written.

    	Override to write the encoded packet into the intermediate sending buffer.
    */
    virtual int frameOutgoingPacket(OutgoingPacket& packet, DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary) = 0;
    //@}
    /** @name De-serialization. **/
    /**
    \param buffer to read data from.
    \param pPacket Reference to the output pointer that should store the address of the potentially created instance.
    \param serviceId Service id value used to route the created instance.
    \param nExtractedBytes Number of bytes extracted from the buffer.

    Override to deframe incoming packets from within the received bytes.
    */
    virtual int tryDeframeIncomingPacket(DataBuffer& buffer, IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* pContext) = 0;
    /**
    \param pPacket The previously deframed instance.
    \param serviceId Service id value used to route the created instance.

    Override to decode the created instance.
    */
    virtual int decodeIncomingPacket(IncomingPacket* pPacket, int& serviceId) = 0;
    /**
    \param pPacket IncomingPacket instance to delete.

    Incoming requests are created by the Protocol at time of de-serialization. When the servicing job that treats
    a requests finishes, the instance is handed back to its creator for deletion. Developer can call on delete or
    any other way of releasing the instance if a pool of requests is organized.
    */
    virtual void disposeIncomingPacket(IncomingPacket* pPacket) = 0;
    //@}

};

}

#endif // Protocol__INCLUDED
