#include "RenderManager.h"

#include <renderer/Context/ContextManager.h>
#include <IO/Service/IOLocal.h>
#include <IO/TransferFunction1D.h>
#include <renderer/OpenGL/GLGridLeaper.h>
#include <renderer/NetworkRenderer/NetRendererClient.h>


//using namespace Tuvok::Renderer::OpenGL;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;

using namespace Tuvok;
using namespace Tuvok::Renderer;


std::shared_ptr<IRenderer> RenderManager::createRenderer(   Visibility visibility,
                                                            Core::Math::Vec2ui resolution,
                                                            std::string dataset,
                                                            std::string tf,
                                                            bool startThread){

    LINFOC("RenderServer", "initialize renderer");
    std::shared_ptr<Context::Context> context = Context::ContextManager::getInstance().createContext(visibility, resolution);

	return createRenderer(context,dataset,tf,startThread);
}


std::shared_ptr<IRenderer> RenderManager::createRenderer(   std::shared_ptr<Tuvok::Renderer::Context::Context> contextPtr,
                                                            std::string dataset,
                                                            std::string tf,
                                                            bool startThread){

    std::shared_ptr<IOLocal> ioLocal = std::make_shared<IOLocal>("LOCALIO");
	uint16_t handleLocal = ioLocal->openFile(dataset);

	std::shared_ptr<OpenGL::GLGridLeaper> renderer = std::make_shared<OpenGL::GLGridLeaper>(contextPtr, contextPtr->getContextResolution(), ERenderMode::RM_1DTRANS);
	//std::shared_ptr<GLGridLeaper> renderer = std::make_shared<OpenGL::GLGridLeaper>(contextPtr,contextPtr->getContextResolution(), ERenderMode::RM_1DTRANS);

    //first: set the transferfunction
    DataIO::TransferFunction1D stdtf(255);
    if(tf == "none"){
        stdtf.SetStdFunction(0.3f,0.6f);
    }else{
        stdtf.Load(tf);
    }
    renderer->Set1DTransferFunction( *(stdtf.GetColorData().get()));

    //second: set the dataset
    renderer->SetDataset(ioLocal);

    LINFOC("RenderServer", "initialize renderer");
    if (!renderer->Initialize()){
        LWARNINGC("RenderServer", "ERROR IN RENDER INIT");
		return 0;
    }

    if(startThread)
        renderer->startRenderThread();

    return renderer;
}


 std::shared_ptr<IRenderer> RenderManager::createRenderer(      std::string ip,
                                                                int port,
                                                                Visibility visibility,
                                                                Core::Math::Vec2ui resolution,
                                                                std::string dataset,
                                                                std::string tf){

    std::shared_ptr<NetRendererClient> renderer = std::make_shared<NetRendererClient>(ip,port,visibility,resolution,dataset,tf);
    return renderer;
}
