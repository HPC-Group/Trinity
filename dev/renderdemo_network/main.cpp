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

//some usings
using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;


typedef std::shared_ptr<IRenderer> RenderPtr;


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

    /*
        THIS IS JUST DEMO HOW TO CREATE A RENDERER !

        WHAT YOU SHOULD DO

        RenderPtr renderer; //for first single renderer!

        loop{
            WAIT FOR MESSAGE;
            PROCESS MESSAGE->
                switch(message){
                    init :  {
                            renderer = RenderManager::getInstance().createRenderer(Visibility::Windowed,Vec2ui(640,480),"walnut.uvf","none"); //later add file/tfname!
                            return handleToRenderer (need to store handles on serverside, map or so, will add this later)
                            break;
                            }
                    getFB : {
                            return renderer->readFrameBuffer(); // vector<uint8_t>
                            break;
                            }
                    rotate :{
                            renderer->RoateCamera(Vec3f(x,y,z));
                            return SOMETHING;
                            break;
                            }
                    ...
                }
        }
    */

    //Use the interface of the rendermanager to create a renderer !
    IRenderManager& manager = RenderManager::getInstance();
    //create a renderer
    //RenderPtr renderer = manager.createRenderer(Visibility::Windowed,Vec2ui(640,480),"walnut.uvf","none");
    RenderPtr renderer = manager.createRenderer(Visibility::Windowed,Vec2ui(1280,720),"WholeBody-SCANLINE-68-lz4.uvf","WholeBody.1dt");
    //at this point a gridleaper should run in a seperate thread and you can use the RenderPtr for interaction


	while (true){
        // do whatever you want ?
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // to reduce the incoming commands !

        renderer->RotateCamera(Vec3f(0,0.3,0));
	}
}
