//
//  main.cpp
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
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

//because we use glfw here
#include <renderer/OpenGL/OpenGLDefines.h>
#include "GLFWDefines.h"


#include <core/Math/Vectors.h>

#include <renderer/Service/RenderServer.h>

#include <renderer/RenderEnums.h>

#include <core/FileFinder.h>

//logger, use sweden logger later ! will remove debugouthandle
#include <tools/DebugOutHandler.h>


#include <communication/protocols/protocol_renderer.h>
#include <communication/protocols/LocalRenderer.h>

//NEW STUFF
#include <renderer/Context/ContextManager.h>

#include <IO/Service/IOLocal.h>
#include <IO/TransferFunction1D.h>
#include <renderer/OpenGL/GLGridLeaper_v2.h>

using namespace Tuvok::Renderer::Service;
using namespace Tuvok::Renderer;
using namespace Tuvok::Renderer::OpenGL;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;


using std::cout;
using std::endl;
using std::shared_ptr;

static float f = 0.01;
static float zoom = 1.5f;

//This function lists all datasets and transferfunction in the folders <root>, /DataSets, /datasets
void listFiles(std::vector<std::string> &ds, std::vector<std::string> &tf){
    Core::FileFinder::getInstance().readFilesWithEnding("DataSets/",ds,".uvf");
    Core::FileFinder::getInstance().readFilesWithEnding("",ds,".uvf");
    Core::FileFinder::getInstance().readFilesWithEnding("datasets/",ds,".uvf");

    Core::FileFinder::getInstance().readFilesWithEnding("DataSets/",tf,".1dt");
    Core::FileFinder::getInstance().readFilesWithEnding("",tf,".1dt");
    Core::FileFinder::getInstance().readFilesWithEnding("datasets/",tf,".1dt");
    tf.push_back("none");
    std::cout << "Datasets found : "<< ds.size()<<std::endl;
    for(int i = 0; i < ds.size();++i){
        std::cout << i <<" - " << ds[i] << std::endl;
    }
    std::cout << "Transferfunctions found : "<< tf.size()<<std::endl;
    for(int i = 0; i < tf.size();++i){
        std::cout << i <<" - " << tf[i] << std::endl;
    }
}

void selectDataSetAndTransferFunction(std::string& sDataSet, std::string& sTF){
    //Select DataSet and TransferFunction
    std::cout << "LIST DS, TF" << std::endl;
    std::vector<std::string> ds;
    std::vector<std::string> tf;
    listFiles(ds,tf);

    int dsid,tfid;
    std::cout << "Enter DS id" << std::endl;
    std::cin >> dsid;
    std::cout << "Enter TF id" << std::endl;
    std::cin >> tfid;

    sDataSet = ds[dsid];
    sTF = tf[tfid];
}

bool endAll = false;
//interaction with the renderer
void glfwHanldeKeyboard(GLFWwindow* window, Communication::ProtocolRenderer* renderer){

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
      double x =0;
      double y = 0;
      glfwGetCursorPos(window,&x,&y);

      renderer->clearViewPicking(Vec2ui((uint32_t)x,(uint32_t)y));
    }

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		renderer->rotateCamera(Vec3f(0, 0.5f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		renderer->rotateCamera(Vec3f(0, -0.5f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		renderer->rotateCamera(Vec3f(0.5f, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		renderer->rotateCamera(Vec3f(-0.5f, 0, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		renderer->moveCamera(Vec3f(0, 0.01f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		renderer->moveCamera(Vec3f(0, -0.01f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
		renderer->moveCamera(Vec3f(0.01f, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		renderer->moveCamera(Vec3f(-0.01f, 0, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
		zoom += 0.01f;
		renderer->zoomCamera(zoom);
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
		zoom -= 0.01f;
		renderer->zoomCamera(zoom);
	}

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        renderer->setRenderMode(Tuvok::Renderer::ERenderMode::RM_1DTRANS);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        renderer->setRenderMode(Tuvok::Renderer::ERenderMode::RM_ISOSURFACE);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        renderer->setCompositeMode(Tuvok::Renderer::ECompositeDisplay::D_FINALCOLOR);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        renderer->setCompositeMode(Tuvok::Renderer::ECompositeDisplay::D_RESUMECOLOR);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        renderer->setCompositeMode(Tuvok::Renderer::ECompositeDisplay::D_RESUMEPOSITION);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        renderer->setCompositeMode(Tuvok::Renderer::ECompositeDisplay::D_DEBUGVIEW);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        renderer->setClearViewEnabled(true);
    }
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
		renderer->setClearViewEnabled(false);
	}


	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS){
		f += 0.01f;
		renderer->setClearViewRadius(f);
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS){
		f -= 0.01f;
		renderer->setClearViewRadius(f);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        std::cout << "escape pressed"<< std::endl;
		renderer->stopRenderThread();
		endAll = true;
	}
}

int main(int argc, char* argv[]){
    //DebugOut remove as soon as possible
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowErrors(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowMessages(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowWarnings(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowOther(true);


    LogManager::initialize(LogManager::LogLevel::Debug, true);

    Log* html = new HTMLLog("RenderDemoLog.html");
    Log* text = new TextLog("RenderDemoLog.txt");
    Log* console = new ConsoleLog();
    LogMgr.addLog(html);
    LogMgr.addLog(text);
    LogMgr.addLog(console);

    LINFOC("bla log", "blub");


    std::string dataset;
    std::string transferfunction;

    selectDataSetAndTransferFunction(dataset,transferfunction);
/*
    Communication::ProtocolRenderer* renderer = new Communication::LocalRenderer();
    renderer->createNewRenderer(Visibility::Windowed,Vec2ui(640,480),dataset,transferfunction);

    while(!endAll && renderer->checkIfRenderThreadIsRunning()){
        //THIS IS ONLY FOR OUR LOCAL GLFW TEST! DO NOT DO THIS LATER!!
        GLFWwindow* w = static_cast<GLFWwindow*>(RenderServer::getInstance().getContextPtr(renderer->getRenderHandle())->getContextItem());
        if(w != nullptr){
                //Handle inputs
                glfwHanldeKeyboard(w,renderer);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }

    //wait for every other thread to detach
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));*/

    //NEW STUFF TESTING MAKE NICE LATER!!

//CONTEXT CREATION
    std::shared_ptr<Context::Context> context = Context::ContextManager::getInstance().createContext(Visibility::Windowed, Vec2ui(640,480));

//RENDER INIT!!
//THIS IS PURE LOCAL FOR FIRST
	std::shared_ptr<IOLocal> ioLocal = std::make_shared<IOLocal>("LOCALIO");
	uint16_t handleLocal = ioLocal->openFile(dataset);

	std::shared_ptr<GLGridLeaper2> renderer = std::make_shared<OpenGL::GLGridLeaper2>(context,Vec2ui(640,480), ERenderMode::RM_1DTRANS);

    //first: set the transferfunction
    DataIO::TransferFunction1D stdtf(255);
    if(transferfunction == "none"){
        stdtf.SetStdFunction(0.3f,0.6f);
    }else{
        stdtf.Load(transferfunction);
    }
    renderer->Set1DTransferFunction( *(stdtf.GetColorData().get()));

    //second: set the dataset
    renderer->SetDataset(ioLocal);

    uint64_t gpumemorysize = 500 * 1024*1024;

    std::string vendor = (char*)glGetString(GL_VENDOR);
    LINFOC("RenderServer", "gpu vendor: " << vendor);
    if(vendor == "NVIDIA Corporation"){

        int currentavailable = 0;
        // get the currently AVAILABLE!! free gpu memory
        glGetIntegerv(0x9049, &currentavailable);

        LINFOC("RenderServer", "available vram in kb: "<< currentavailable);
        uint64_t willusemax = currentavailable - (200*1024);
        if(willusemax < 200*1024){
            LWARNINGC("RenderServer", "not enough free vram.");
            return 0;
        }
        LINFOC("RenderServer", "will use 200MB less vram: "<< willusemax);
        LINFOC("RenderServer", "read errorcodes: " << glGetError());

        gpumemorysize = willusemax * 1024;
    }

    LINFOC("RenderServer", "initialize renderer");
    if (!renderer->Initialize(gpumemorysize)){
        LWARNINGC("RenderServer", "ERROR IN RENDER INIT");
		return 0;
    }



    while(true){
    }
}
