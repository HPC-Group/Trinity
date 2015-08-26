//
//  main.cpp
//  TNG
//
//  Created by Andre Waschk on 18/07/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

#include <renderer/OpenGL/OpenGLDefines.h>  // opengl
#include "GLFWDefines.h"


#include <core/Math/Vectors.h>      // resolution
#include <renderer/RenderEnums.h>   // visibility state etc

#include <renderer/RenderManager.h> //RenderManager!

#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

#include <renderer/NetworkRenderer/NetRendererServer.h>>

//some usings
using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;

using namespace mocca;
using namespace mocca::net;




int main(int argc, char* argv[]){

    //init logger
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    Log* html = new HTMLLog("RenderDemoServerLog.html");
    Log* text = new TextLog("RenderDemoServerLog.txt");
    Log* console = new ConsoleLog();
    LogMgr.addLog(html);
    LogMgr.addLog(text);
    LogMgr.addLog(console);

    LINFOC("RENDER DEMO - SERVER SIDE", "LOGGING STARTED");

	Tuvok::Renderer::NetRendererServer server;

	server.openServer(1234);
	while(true){
        server.acceptConnection();
        server.waitForMsg();
	}
}


