//
//  main.cpp
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include <chrono>
#include <thread>


#include <iostream>
#include <fstream>

//because we use glfw here
#include <renderer/OpenGL/OpenGLDefines.h>
#include "GLFWDefines.h"


#include <core/Math/Vectors.h>


#include <communication/protocols/protocol_renderer.h>
#include <renderer/Service/RenderServer.h>

#include <renderer/RenderEnums.h>

#include <core/FileFinder.h>

//logger, use sweden logger later ! will remove debugouthandle
#include <tools/DebugOutHandler.h>



using namespace Tuvok::Renderer::Service;
using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;

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

//interaction with the renderer
void glfwHanldeKeyboard(GLFWwindow* window, uint16_t renderHandle){

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
      double x =0;
      double y = 0;
      glfwGetCursorPos(window,&x,&y);

      RenderServer::getInstance().clearViewPicking(renderHandle,Vec2ui((uint32_t)x,(uint32_t)y));
    }

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		RenderServer::getInstance().rotateCamera(renderHandle, Vec3f(0, 0.5f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		RenderServer::getInstance().rotateCamera(renderHandle, Vec3f(0, -0.5f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		RenderServer::getInstance().rotateCamera(renderHandle, Vec3f(0.5f, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		RenderServer::getInstance().rotateCamera(renderHandle, Vec3f(-0.5f, 0, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		RenderServer::getInstance().moveCamera(renderHandle, Vec3f(0, 0.01f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		RenderServer::getInstance().moveCamera(renderHandle, Vec3f(0, -0.01f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
		RenderServer::getInstance().moveCamera(renderHandle, Vec3f(0.01f, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		RenderServer::getInstance().moveCamera(renderHandle, Vec3f(-0.01f, 0, 0));
	}

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS){
		zoom += 0.01f;
		RenderServer::getInstance().zoomCamera(renderHandle, zoom);
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS){
		zoom -= 0.01f;
		RenderServer::getInstance().zoomCamera(renderHandle, zoom);
	}

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        RenderServer::getInstance().setRenderMode(renderHandle,Tuvok::Renderer::ERenderMode::RM_1DTRANS);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        RenderServer::getInstance().setRenderMode(renderHandle,Tuvok::Renderer::ERenderMode::RM_ISOSURFACE);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        RenderServer::getInstance().setCompositeMode(renderHandle, Tuvok::Renderer::ECompositeDisplay::D_FINALCOLOR);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        RenderServer::getInstance().setCompositeMode(renderHandle, Tuvok::Renderer::ECompositeDisplay::D_RESUMECOLOR);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        RenderServer::getInstance().setCompositeMode(renderHandle, Tuvok::Renderer::ECompositeDisplay::D_RESUMEPOSITION);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
        RenderServer::getInstance().setCompositeMode(renderHandle, Tuvok::Renderer::ECompositeDisplay::D_DEBUGVIEW);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        RenderServer::getInstance().setClearViewEnabled(renderHandle,true);
    }
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
		RenderServer::getInstance().setClearViewEnabled(renderHandle, false);
	}


	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS){
		f += 0.01f;
		RenderServer::getInstance().setClearViewRadius(renderHandle, f);
	}
	if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS){
		f -= 0.01f;
		RenderServer::getInstance().setClearViewRadius(renderHandle, f);
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		f -= 0.01f;
		RenderServer::getInstance().stopRenderThread(renderHandle);
	}
}

int main(int argc, char* argv[]){
    //DebugOut remove as soon as possible
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowErrors(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowMessages(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowWarnings(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowOther(true);


    std::string dataset;
    std::string transferfunction;

    selectDataSetAndTransferFunction(dataset,transferfunction);

    //Get RenderServer Instance
	Communication::ProtocolRenderer& r = RenderServer::getInstance();

    //create a new renderer and store the id of the renderer
    uint16_t renderHandle = r.createNewRenderer(Visibility::Windowed,Vec2ui(1280,720),dataset,transferfunction);



    bool keppRunning = true;
    while(keppRunning){
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        //if the renderer is still running
        if(r.checkIfRenderThreadIsRunning(renderHandle) != 0){
            //get pointer to the glfwWindow (only works if renderer is using glfw -> later we will just send the commands without glfw windows etc)
            GLFWwindow* w = static_cast<GLFWwindow*>(RenderServer::getInstance().getContextPtr(renderHandle)->getContextItem());
            if(w != nullptr){
                //Handle inputs
                glfwHanldeKeyboard(w,renderHandle);
            }
        }else{
			keppRunning = false;
		}
    }
}
