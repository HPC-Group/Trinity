/********************************************************************
	File :			Protocol.cpp
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
#include "StdAfx.h"
#include "../include/Protocol.h"
#include "../include/ConnectionContext.h"

namespace PushFramework
{


Protocol::Protocol(void)
{
}

Protocol::~Protocol(void)
{
}

int Protocol::serializeOutgoingPacket( OutgoingPacket& packet, DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary)
{
    int nRet = encodeOutgoingPacket(packet);
    if(nRet != Success)
        return nRet;
    return frameOutgoingPacket(packet, buffer, nWrittenBytes, binary);
}

int Protocol::tryDeserializeIncomingPacket( DataBuffer& buffer, IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* pContext )
{
    int nRet = tryDeframeIncomingPacket(buffer, pPacket, serviceId, nExtractedBytes, pContext);
    if(nRet != Success)
        return nRet;

    return decodeIncomingPacket(pPacket, serviceId);
}

}

