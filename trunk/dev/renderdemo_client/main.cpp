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
#include <renderer/Context/ContextManager.h>


#include <base/Error.h>
#include <base/BytePacket.h>
#include <net/TCPNetworkAddress.h>
#include <net/LoopbackNetworkService.h>
#include <net/LoopbackConnectionListener.h>
#include <net/TCPNetworkService.h>

#include <lz4/lz4.h>
#include <lz4/lz4hc.h>

//some usings
using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;


//typedef std::shared_ptr<IRenderer> RenderPtr;
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


    //CREATE A SIMPLE WINDOW TO DISPLAY THE RENDERER STUFF!
    std::shared_ptr<Tuvok::Renderer::Context::Context> FrontendWindow = Tuvok::Renderer::Context::ContextManager::getInstance().createContext(Visibility::Windowed, Vec2ui(1024,768),0);
    std::vector<uint8_t> pixels;
    pixels.resize(1024*768*3);

    // NOW YOU SHOULD USE YOUR NETWORK LAYER TO TALK TO THE RENDER SERVER
    // USE
    // pixels = renderer->ReadFramebuffer(); TO ACCESS THE LATEST FRAMEBUFFER
    // THE LOOP SHOWS HOW TO UPDATE THE CLIENT WINDOW !

    /* DO SOMETHING LIKE

        RenderPtr renderer = Network->CreateRenderer

        loop{
            pixles = renderer->ReadFramebuffer();

            // DO FRAMEBUFFER UPDATE

            HandleInputs();
        }

    */
    TCPNetworkService netService; // instantiate the network service
    std::unique_ptr<AbstractConnection>  clientConnection = netService.connect("localhost:1234"); // connect to the server
   /* BytePacket packet;
    packet << "The meaning of life is... "; // create some data to send
    clientConnection->send(packet.byteArray()); // send data to server
    */

	while (true){
        // do whatever you want ?
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // to reduce the incoming commands !
        BytePacket data = clientConnection->receive(ReceiveMode::NON_BLOCKING);
             if(data.byteArray().size() > 0){

                //LZ4_decompress_safe ((char*)data.byteArray().data(), (char*) &pixels[0], data.byteArray().size(), 1024*768*3);
                FrontendWindow->lockContext();
                //glDrawPixels(1024,768, GL_RGB, GL_UNSIGNED_BYTE,(void*) &(pixels[0])); // DISPLAY FRAMEBUFER!
                glDrawPixels(1024,768, GL_RGB, GL_UNSIGNED_BYTE,(void*) data.byteArray().data()); // DISPLAY FRAMEBUFER!
                FrontendWindow->frameFinished();
                FrontendWindow->unlockContext();
        }
	}
}
