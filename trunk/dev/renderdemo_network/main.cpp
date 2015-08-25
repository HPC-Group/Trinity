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


typedef std::shared_ptr<IRenderer> RenderPtr;

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1)*33) ^ str[h];
}


RenderPtr initRenderer(Vec2ui resolution, std::string dataset, std::string tf){
    IRenderManager& manager = RenderManager::getInstance();
    RenderPtr renderer = manager.createRenderer(Visibility::Windowed,Vec2ui(640,480),"WholeBody-SCANLINE-68-lz4.uvf","WholeBody.1dt");
    return renderer;
}


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


    //Use the interface of the rendermanager to create a renderer !
    //IRenderManager& manager = RenderManager::getInstance();
    //create a renderer
    //RenderPtr renderer = manager.createRenderer(Visibility::Windowed,Vec2ui(640,480),"walnut.uvf","none");
    //RenderPtr renderer = manager.createRenderer(Visibility::Windowed,Vec2ui(1024,768),"WholeBody-SCANLINE-68-lz4.uvf","WholeBody.1dt");
    //at this point a gridleaper should run in a seperate thread and you can use the RenderPtr for interaction

   /* TCPNetworkService netService; // instantiate the network service
    auto connectionListener = netService.bind("1234"); // listen for connections

    int connectionCounter = 0;
    std::unique_ptr<AbstractConnection> serverConnection  = nullptr;
    std::vector<uint8_t> pixels;
    std::vector<uint8_t> pixelsCompressed;
    pixelsCompressed.resize(640*480*3);
    int compressedSize = 0;
    RenderPtr renderer = nullptr;
	while (true){
        // do whatever you want ?
        //std::this_thread::sleep_for(std::chrono::milliseconds(5)); // to reduce the incoming commands !


        if(connectionListener->numConnections() > connectionCounter){
           serverConnection = connectionListener->getConnection(); // accept the next pending connection
           std::cout << "something connected!"<< std::endl;
           connectionCounter = connectionListener->numConnections();
        }
        if(serverConnection != nullptr){
            if(renderer != nullptr){
                pixels = renderer->ReadFrameBuffer();
                //compressedSize = LZ4_compress((char*)&pixels[0],(char*)&pixelsCompressed[0],pixels.size());
                //pixelsCompressed.resize(compressedSize);
                ByteArray p = ByteArray::create();
                //p.append(&(pixelsCompressed[0]),pixels.size());
                p.append(&(pixels[0]),pixels.size());
                serverConnection->send(p);
                //pixelsCompressed.resize(1920*1080*3);

                renderer->RotateCamera(Vec3f(0,0.2,0.4));
            }


            BytePacket data = serverConnection->receive(ReceiveMode::NON_BLOCKING);
            if(data.byteArray().size() > 0){
            std::string message = data.get<std::string>();

            std::cout << message << std::endl;

                switch(str2int(message.c_str())){
                    case str2int("INIT") :
                        renderer = initRenderer(Vec2ui(640,480),"stuff","test");
                        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
                        break;
                    case str2int("ROTATE") : break;
                    default: break;
                }
            }


        }
	}*/

	Tuvok::Renderer::NetRendererServer server;

	server.openServer(1234);
	while(true){
        server.acceptConnection();
        server.waitForMsg();
	}
}


