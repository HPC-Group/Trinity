#pragma once
#include "Symbols.h"
#include "WebsocketMessage.h"

class WEBSOCKETPROTOCOL_API WebsocketHandshakeMessage : public WebsocketMessage {
public:
	WebsocketHandshakeMessage(char* pRaw, int nSize);
	WebsocketHandshakeMessage();
	~WebsocketHandshakeMessage(void);

	bool Parse();
	
	string GetField(string key);
	void SetField(string key, string val);


	string Serialize();

private:
	char* pRaw;
	int nSize;
	string m_method;
	string m_uri;
	string m_version;
	map<string, string> fields;
};


