
#pragma once
#include "Symbols.h"

class WEBSOCKETPROTOCOL_API WebsocketConnectionContext : public ConnectionContext
{
public:
	enum
	{
		HandshakeStage = 0,
		LoginStage,
		ConnectedStage
	};
	WebsocketConnectionContext();
	~WebsocketConnectionContext(void);

	int GetStage();
	void SetStage(int stage);

private:
	int stage;
};

