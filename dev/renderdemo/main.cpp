//
//  main.cpp
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include <memory>
#include <chrono>
#include <thread>


#include <iostream>
#include <fstream>
#include <renderer/OpenGL/OpenGLDefines.h>
#include "GLFWDefines.h"

#include <renderer/OpenGL/GLGridLeaper.h>

#include <core/Math/Vectors.h>

#include <core/TuvokException.h>

#include <tools/DebugOutHandler.h>

#include <renderer/RenderEnums.h>

#include <renderer/Context/GLFWContext.h>
#include <renderer/Context/XContext.h>
#include <renderer/Service/RenderServer.h>

#include <core/FileFinder.h>

using namespace Tuvok::Renderer::OpenGL::GLCore;
using namespace Tuvok::Renderer::OpenGL;
using namespace Tuvok::Renderer::Service;
using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;

using std::cout;
using std::endl;
using std::shared_ptr;

static float f = 0.01;
static float zoom = 1.5f;

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
//-----MORPHABLE UI INIT -------------------------------------------------------------
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowErrors(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowMessages(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowWarnings(true);
	IVDA::DebugOutHandler::Instance().DebugOut()->SetShowOther(true);

    std::cout << "LIST DS, TF" << std::endl;
    std::vector<std::string> ds;
    std::vector<std::string> tf;
    listFiles(ds,tf);

    int dsid,tfid;
    std::cout << "Enter DS id" << std::endl;
    std::cin >> dsid;
    std::cout << "Enter TF id" << std::endl;
    std::cin >> tfid;


	RenderServer& r = RenderServer::getInstance();

	std::vector<uint16_t> renderHandles;
    renderHandles.push_back(r.createNewRenderer(Visibility::Windowed,Vec2ui(1280,720),ds[dsid],tf[tfid]));

    //renderHandles.push_back(r.createNewRenderer(Visibility::Windowed,Vec2ui(1280,720),"Mandelbulb1k-SCANLINE-132-lz4.uvf","Mandelbulb1k-SCANLINE-64-lz4_10.1dt"));
    //renderHandles.push_back(r.createNewRenderer(Visibility::Windowed, Vec2ui(1280, 720), "WholeBody-SCANLINE-68-lz4.uvf", "WholeBody-SCANLINE-68-lz4.1dt"));
    //renderHandles.push_back(r.createNewRenderer(Visibility::Windowed, Vec2ui(1280, 720), "RichtmyerMeshkov-SCANLINE-36-lz4.uvf", "RichtmyerMeshkov-SCANLINE-36-lz4.1dt"));
    //renderHandles.push_back(r.createNewRenderer(Visibility::Windowed, Vec2ui(1280, 720), "VisibleHumanFullColor-SCANLINE-36-lz4.uvf", "VisibleHumanFullColor-SCANLINE-36-lz4.1dt"));
    //renderHandles.push_back(r.createNewRenderer(Visibility::Windowed,Vec2ui(640,480),"walnut.uvf","none"));
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    //wait for renderer to load everything needed
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    bool keppRunning = true;
    while(keppRunning){
        //send each renderthread a rotate command
        for(int c = 0; c < renderHandles.size();++c){
			uint16_t i = renderHandles[c];
            if(r.checkIfRenderThreadIsRunning(i) != 0){
                std::this_thread::sleep_for(std::chrono::milliseconds(20));
                //RenderServer::getInstance().rotateCamera(i,Vec3f(0,0.1f*(i+1),0));
                GLFWwindow* w = static_cast<GLFWwindow*>(RenderServer::getInstance().getContextPtr(i)->getContextItem());
                if(w != nullptr){
                    glfwHanldeKeyboard(w,i);
                }
            }else{
                renderHandles.erase(renderHandles.begin()+c);
                c--;
            }
        }
		if (renderHandles.size() == 0){
			keppRunning = false;
		}
    }
}
