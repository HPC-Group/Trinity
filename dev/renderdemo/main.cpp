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


#include <renderer/RenderEnums.h>

#include <core/FileFinder.h>

//logger, use sweden logger later ! will remove debugouthandle
#include <tools/DebugOutHandler.h>

//NEW STUFF
#include <renderer/Context/ContextManager.h>
#include <renderer/RenderManager.h>

#include <IO/Service/IOLocal.h>
#include <IO/TransferFunction1D.h>
#include <renderer/OpenGL/GLGridLeaper.h>

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
void glfwHanldeKeyboard(GLFWwindow* window, std::shared_ptr<IRenderer> renderer){

  /*  if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
      double x =0;
      double y = 0;
      glfwGetCursorPos(window,&x,&y);

      renderer->clearViewPicking(Vec2ui((uint32_t)x,(uint32_t)y));
    }
  */
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS){
		cout << "left pressed" << std::endl;
		renderer->RotateCamera(Vec3f(0, 0.5f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS){
		renderer->RotateCamera(Vec3f(0, -0.5f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
		renderer->RotateCamera(Vec3f(0.5f, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
		renderer->RotateCamera(Vec3f(-0.5f, 0, 0));
	}
  /*
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		renderer->MoveCamera(Vec3f(0, 0.01f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		renderer->MoveCamera(Vec3f(0, -0.01f, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
		renderer->MoveCamera(Vec3f(0.01f, 0, 0));
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		renderer->MoveCamera(Vec3f(-0.01f, 0, 0));
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
	}*/
}

int test(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount){
    FILE *image;

    char tempstring[50];
	sprintf(tempstring, "screenshot_%i.tga", 1);
	if ((image = fopen(tempstring, "wb")) == NULL) return 1;

	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	unsigned char header[6] = { static_cast<unsigned char>(((int)(width % 256))),
		static_cast<unsigned char>(((int)(width / 256))),
		static_cast<unsigned char>(((int)(height % 256))),
		static_cast<unsigned char>(((int)(height / 256))), 24, 0 };

	// TGA header schreiben
	fwrite(TGAheader, sizeof(unsigned char), 12, image);
	// Header schreiben
	fwrite(header, sizeof(unsigned char), 6, image);

	fwrite(&(pixels[0]), sizeof(unsigned char),
		width * height * 3, image);

	fclose(image);
}

int main(int argc, char* argv[]){

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
    //NEW STUFF TESTING MAKE NICE LATER!!

	//CONTEXT CREATION
	std::shared_ptr<Tuvok::Renderer::Context::Context> context = Tuvok::Renderer::Context::ContextManager::getInstance().createContext(Visibility::Windowed, Vec2ui(1280, 720));

	GLFWwindow* window = (GLFWwindow*)context->getContextItem();

	//RENDER INIT!!
	std::shared_ptr<IRenderer> renderer = RenderManager::getInstance().createRenderer(context, dataset, transferfunction);

	//sleep and wait for renderer to initiate
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	renderer->SetCameraZoom(0.5f);
	while (true){
		renderer->RotateCamera(Vec3f(0, 3.1f, 0));
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		glfwHanldeKeyboard(window, renderer);
	}
    renderer->stopRenderThread(); // <- never reached right now!
}
