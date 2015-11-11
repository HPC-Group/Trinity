#pragma once
#include "Symbols.h"
#include "WebsocketMessage.h"

class WEBSOCKETPROTOCOL_API WebsocketDataMessage : public WebsocketMessage
{
public:
	WebsocketDataMessage(const char* buffer);
	WebsocketDataMessage(int serviceId);
	~WebsocketDataMessage(void);
	
	int GetServiceId();
	void SetServiceId(int serviceId);
	void SetArguments(string arg1, string arg2 = "");
	string GetArg1();
	string GetArg2();
    bool Encode();
	bool Decode();
	string GetEncodedData();
private:
	int serviceId;
	string arg1;
	string arg2;

	string raw;

};

