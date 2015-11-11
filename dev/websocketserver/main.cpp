//
//  main.cpp
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "stdafx.h"

#include "WebsocketServer.h"


#include <windows.h>

bool terminated = false;

/*unsigned __stdcall threadProc(LPVOID WorkContext)
{
while (!terminated)
{
WebsocketDataMessage* message = new WebsocketDataMessage(StreamedCommunication);
message->SetArguments("stream message");
broadcastManager.PushPacket(message, "streamingQueue");
Sleep(1000);
}
return 0;
}*/

int _tmain(int argc, _TCHAR* argv[])
{
	WebsocketServer server;
	EchoService *test = new EchoService();
	server.registerService(EchoCommunication, test, "echo");

	/*server.registerService(Routedcommunication, new RoutedCommunicationService, "routed");
	server.registerService(GroupCommunication, new GroupCommunicationService, "grouped");
	server.registerService(StreamedCommunication, new StreamedCommunicationService, "streamed");*/



	server.setClientFactory(new WebsocketClientFactory);
	server.setMaxConnections(100);
	server.setLoginExpiryDuration(60);

	server.setProtocol(new WebsocketProtocol);

	server.createListener(81);


	//broadcastManager.CreateQueue("groupQueue", 100, true, 10, 10);
	//broadcastManager.CreateQueue("streamingQueue", 100, true, 10, 10);



	//Start streaming thread :
	//UINT  nThreadID;

	//_beginthreadex(NULL, 0, threadProc, NULL, 0, &nThreadID);


	try
	{
		server.start(true);
	}
	catch (std::exception& e)
	{
		cout << "Failed to start server. Exception : " << e.what() << std::endl;
		return 0;
	}



	// end testing fragments

	while (true)
	{
	}
	terminated = true;
	Sleep(1000);

	server.stop();

	return 0;
}
