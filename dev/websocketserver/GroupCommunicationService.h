#pragma once

class GroupCommunicationService :
	public Service
{
public:
	GroupCommunicationService(void);
	~GroupCommunicationService(void);
	void handle( LogicalConnection* pClient, IncomingPacket* pRequest );

};

