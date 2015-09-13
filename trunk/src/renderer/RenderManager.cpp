#include "RenderManager.h"

#include <renderer/Context/ContextManager.h>
#include <IO/Service/IOLocal.h>
#include <IO/TransferFunction1D.h>
#include <renderer/OpenGL/GLGridLeaper.h>
#include <renderer/NetworkRenderer/NetRendererClient.h>


using namespace Tuvok::Renderer::OpenGL;
using namespace Tuvok;
using namespace Core::Math;
using namespace ghoul::logging;

using namespace Tuvok;
using namespace Tuvok::Renderer;


uint64_t useableVRAM(){
	uint64_t gpumemorysize = 500 * 1024 * 1024;

	std::string vendor = (char*)glGetString(GL_VENDOR);
	LINFOC("RenderServer", "gpu vendor: " << vendor);
	if (vendor == "NVIDIA Corporation"){

		int currentavailable = 0;
		// get the currently AVAILABLE!! free gpu memory
		glGetIntegerv(0x9049, &currentavailable);

		LINFOC("RenderServer", "available vram in kb: " << currentavailable);
		uint64_t willusemax = currentavailable - (200 * 1024);
		if (willusemax < 200 * 1024){
			LWARNINGC("RenderServer", "not enough free vram.");
			return 0;
		}
		LINFOC("RenderServer", "will use 200MB less vram: " << willusemax);
		LINFOC("RenderServer", "read errorcodes: " << glGetError());

		gpumemorysize = willusemax * 1024;
	}

	return gpumemorysize;
}


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

	std::shared_ptr<GLGridLeaper> renderer = std::make_shared<OpenGL::GLGridLeaper>(contextPtr,contextPtr->getContextResolution(), ERenderMode::RM_1DTRANS);

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

	uint64_t gpumemorysize = useableVRAM();

    LINFOC("RenderServer", "initialize renderer");
    if (!renderer->Initialize(gpumemorysize)){
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
