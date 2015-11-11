#include "StdAfx.h"
#include "WebsocketHandshakeMessage.h"


WebsocketHandshakeMessage::WebsocketHandshakeMessage(char* pRaw, int nSize)
	:WebsocketMessage(Handshake)
{
	this->nSize = nSize;
	this->pRaw = new char[nSize+1];
	CopyMemory(this->pRaw, pRaw, nSize);
    this->pRaw[nSize] = '\0';
}

WebsocketHandshakeMessage::WebsocketHandshakeMessage()
	:WebsocketMessage(Handshake)
{
    //HTTP/1.1 101 Switching Protocols
   m_version = "HTTP/1.1";
    
	pRaw = NULL;
	nSize = 0;
}


WebsocketHandshakeMessage::~WebsocketHandshakeMessage(void)
{
	delete[] pRaw;
}

bool WebsocketHandshakeMessage::Parse()
{
    std::istringstream s(pRaw);
    //std::istream s(pRaw);

	std::string request;

	// get status line
	std::getline(s, request);

	if (request[request.size()-1] == '\r') {
		request.erase(request.end()-1);

		std::stringstream ss(request);
		std::string val;

		ss >> m_method;
		ss >> m_uri;
		ss >> m_version;
	} else {
		return false;
	}

	std::string header;
	std::string::size_type end;

	// get headers
	while (std::getline(s, header) && header != "\r") {
		if (header[header.size()-1] != '\r') {
			continue; // ignore malformed header lines?
		} else {
			header.erase(header.end()-1);
		}

		end = header.find(": ",0);

		if (end != std::string::npos) {
			string key = header.substr(0,end);
			string val = header.substr(end+2);

			fields[key] = val;
		}
	}

	return true;
}




std::string WebsocketHandshakeMessage::GetField( string key )
{
	return fields[key];
}

void WebsocketHandshakeMessage::SetField( string key, string val )
{
	fields[key] = val;
}


string WebsocketHandshakeMessage::Serialize( )
{
	std::stringstream raw;

	//raw << m_method << " " << m_uri << " " << m_version << "\r\n";
    raw << "HTTP/1.1 101 Switching Protocols\r\n";

	map<string, string>::iterator it;
	for (it = fields.begin(); it != fields.end(); it++) {
		raw << it->first << ": " << it->second << "\r\n";
	}

	raw << "\r\n";

    return raw.str(); 	
}
