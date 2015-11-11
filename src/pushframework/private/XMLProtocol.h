/********************************************************************
	File :			XMLProtocol.h
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
#ifndef XMLProtocol__INCLUDED
#define XMLProtocol__INCLUDED



#include "../include/Protocol.h"

namespace PushFramework
{
class XMLProtocol : public Protocol
{
public:
    XMLProtocol(void);
    ~XMLProtocol(void);

    virtual void disposeOutgoingPacket(PushFramework::OutgoingPacket* pPacket);
    virtual void disposeIncomingPacket(PushFramework::IncomingPacket* pPacket);

    virtual int encodeOutgoingPacket(PushFramework::OutgoingPacket& packet);
    virtual int frameOutgoingPacket(PushFramework::OutgoingPacket& packet, PushFramework::DataBuffer& buffer, unsigned int& nWrittenBytes, bool binary);
    //////////////////
    virtual int tryDeframeIncomingPacket(PushFramework::DataBuffer& buffer, PushFramework::IncomingPacket*& pPacket, int& serviceId, unsigned int& nExtractedBytes, ConnectionContext* lpContext);
    virtual int decodeIncomingPacket(PushFramework::IncomingPacket* pPacket, int& serviceId);

};
}



#endif // XMLProtocol__INCLUDED
