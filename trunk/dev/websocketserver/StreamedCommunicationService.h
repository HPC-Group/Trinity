#pragma once
class StreamedCommunicationService : public Service
{
public:
	StreamedCommunicationService(void);
	~StreamedCommunicationService(void);
	void handle( LogicalConnection* pClient, IncomingPacket* pRequest );

};

