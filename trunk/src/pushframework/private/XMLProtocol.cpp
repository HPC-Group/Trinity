/********************************************************************
	File :			XMLProtocol.cpp
	Creation date :	2012/01/29

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
#include "StdAfx.h"
#include "XMLProtocol.h"


#include "XMLPacket.h"

namespace PushFramework
{

XMLProtocol::XMLProtocol(void)
{
}

XMLProtocol::~XMLProtocol(void)
{
}

void XMLProtocol::disposeOutgoingPacket( OutgoingPacket* pPacket )
{
    delete pPacket;
}

void XMLProtocol::disposeIncomingPacket( IncomingPacket* pPacket )
{
    delete pPacket;
}

int XMLProtocol::encodeOutgoingPacket( OutgoingPacket& packet )
{
    XMLPacket& response = (XMLPacket&) packet;

    return response.Encode() ? Success : eEncodingFailure;
}

int XMLProtocol::frameOutgoingPacket( OutgoingPacket& packet, DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary )
{
    XMLPacket& response = (XMLPacket&) packet;

    nWrittenBytes = response.getData().length() + 1;
    if (nWrittenBytes > buffer.getRemainingSize())
        return eInsufficientBuffer;

    buffer.Append((char*) response.getData().c_str(), response.getData().length());

    char endChar = 0;

    buffer.Append(&endChar, 1);

    return Success;
}



int XMLProtocol::tryDeframeIncomingPacket( DataBuffer& buffer, IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* lpContext )
{
    serviceId = 0;

    for (int offset  = 0; offset < buffer.GetDataSize(); offset++)
    {
        char ch = buffer.getAt(offset);
        if (ch=='\0')
        {
            nExtractedBytes = offset +1;
            //It is ok
            XMLPacket* pRequest = new XMLPacket();
            if (!pRequest->Decode(buffer.GetBuffer()))
            {
                delete pRequest;
                return eDecodingFailure;
            }
            else
            {
                pPacket = (IncomingPacket*) pRequest;
                serviceId = pRequest->getTypeId();
                return Success;
            }
        }
    }
    return eIncompletePacket;
}



int XMLProtocol::decodeIncomingPacket( PushFramework::IncomingPacket* pPacket, int& serviceId )
{
    return Protocol::Success;
}


}
