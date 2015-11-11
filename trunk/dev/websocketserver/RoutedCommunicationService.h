#pragma once
class RoutedCommunicationService :
	public Service
{
public:
	RoutedCommunicationService(void);
	~RoutedCommunicationService(void);
	void handle( LogicalConnection* pClient, IncomingPacket* pRequest );

};

