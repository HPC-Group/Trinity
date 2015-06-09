#include "RenderServer.h"




#include "../Context/GLFWContext.h"
#ifdef LINUXOS
#include "../Context/XContext.h"
#endif
#include "../OpenGL/GLGridLeaper.h"


#include <IO/Service/IOLocal.h>
#include <IO/TransferFunction1D.h>

using namespace Tuvok;
using namespace Tuvok::Renderer;
using namespace Tuvok::Renderer::Service;
using namespace Tuvok::Renderer::Context;
using namespace Core::Math;
using namespace std;

uint64_t RenderServer::createNewRenderer(Visibility visibility, Core::Math::Vec2ui resolution,std::string dataset,std::string tf, std::string ioHost, uint16_t ioPort){
    //create a prox entry to get the final map identifier;
    ServiceEntry entry(++m_servieIdentifierSource, nullptr, nullptr);
    m_ServiceMap.insert({ entry.getIdentifier(), entry });

    IVDA_MESSAGE("creating new thread");
    m_runThreads.push_back(std::make_shared<std::thread>(&RenderServer::singleRenderThreadLoop, this,entry.getIdentifier(),visibility, resolution, dataset, tf, ioHost, ioPort));

    IVDA_MESSAGE("have to return the handle");
    return entry.getIdentifier();
}

//***** IMPORTANT  IMPORTANT  IMPORTANT  IMPORTANT  *****
//rewrite this init function after full functionality of networking is implemented
std::shared_ptr<Tuvok::Renderer::Context::Context> RenderServer::initNewContext(Visibility visibility, Core::Math::Vec2ui resolution){

	std::shared_ptr<Tuvok::Renderer::Context::Context> context;
    if(visibility == Visibility::hidden){
        if(m_eServerSystem == System::Linux){
#ifdef LINUXOS
            context = std::make_shared<Context::XserverContext>(resolution);
#endif
        }else{
            context = std::make_shared<Context::GLFWContext>("Window",m_eServerSystem,visibility,resolution);
        }
    }else{
        context = std::make_shared<Context::GLFWContext>("Window",m_eServerSystem,visibility,resolution);
    }

	if (context == nullptr) return nullptr;
    if(!context->initContext()) return nullptr;
    IVDA_MESSAGE("CREATED A NEW \"THREADSAFE\" OPENGL CONTEXT");


    return context;
}

std::shared_ptr<AbstrRenderer> RenderServer::initNewRenderer(std::shared_ptr<Tuvok::Renderer::Context::Context> context, Core::Math::Vec2ui resolution, std::string dataset, std::string tf, std::string ioHost, uint16_t ioPort){
   /* //create handle to IOProtocol
    std::shared_ptr<Tuvok::IOSender> ioProt;

    if(ioHost == "localhost" || "127.0.0.1"){
        IVDA_MESSAGE("WILL USE LOCAL IO");
        auto io = make_shared<Tuvok::IOService>();

        LocalRegistry& local = LocalRegistry::Instance();
        shared_ptr<Sender> ioSender = local.getSender(Tuvok::IOService::defaultName);

        ioProt = std::make_shared<Tuvok::IOSender>(ioSender);

    }else{
        IVDA_MESSAGE("TODO REMOTE IO!!");
        return nullptr;
    }

    //load dataset
	IVDA_MESSAGE("LOAD THE TEST DATASET");
	uint16_t handle = 0;
	handle = ioProt->openFile(dataset);*/

    //THIS IS PURE LOCAL FOR FIRST
	std::shared_ptr<IOLocal> ioLocal = std::make_shared<IOLocal>("LOCALIO");
	uint16_t handleLocal = ioLocal->openFile(dataset);

    context->activateContext();
	std::shared_ptr<OpenGL::GLGridLeaper> renderer = std::make_shared<OpenGL::GLGridLeaper>(resolution, ERenderMode::RM_1DTRANS);

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

    uint64_t gpumemorysize = 500 * 1024*1024;

    std::string vendor = (char*)glGetString(GL_VENDOR);
    IVDA_MESSAGE("GRAPHICS CARD VENDOR: " << vendor);
    if(vendor == "NVIDIA Corporation"){

        int currentavailable = 0;
        // get the currently AVAILABLE!! free gpu memory
        glGetIntegerv(0x9049, &currentavailable);

        IVDA_MESSAGE("AVAILABLE KB ON VRAM: "<< currentavailable);
        uint64_t willusemax = currentavailable - (200*1024);
        if(willusemax < 200*1024){
            IVDA_MESSAGE("NOT ENOUGH FREE VRAM TO CREATE A SUITABLE RENDERER");
            return nullptr;
        }
        IVDA_MESSAGE("WILL USE (200MB LESS): "<< willusemax);
        IVDA_MESSAGE("NO ERRORS ? " << glGetError());

        gpumemorysize = willusemax * 1024;
    }

    IVDA_MESSAGE("WILL NOW START THE AWESOME INIT !!!");
    //third: try init
    if (!renderer->Initialize(gpumemorysize)){
		IVDA_MESSAGE("ERROR IN GRIDLEAPER INIT");
		return nullptr;
    }

    //make it look fancy
    renderer->SetUseLighting(true);
    renderer->SwitchPagingStrategy(MissingBrickStrategy::OnlyNeeded);
    renderer->SetCameraZoom(1.5f);

    renderer->SetIsoValue(0.01f);
    //renderer->SetRenderMode(Tuvok::Renderer::ERenderMode::RM_ISOSURFACE);

    renderer->SetCompositeMode(Tuvok::Renderer::ECompositeDisplay::D_FINALCOLOR);
    renderer->setClearViewEnabled(true);

    std::shared_ptr<AbstrRenderer> absrend = renderer;

    return absrend;
}

void RenderServer::singleRenderThreadLoop(uint16_t serviceHandle, Visibility visibility, Core::Math::Vec2ui resolution,std::string dataset,std::string tf, std::string ioHost, uint16_t ioPort){
	std::shared_ptr<Tuvok::Renderer::Context::Context> context = initNewContext(visibility, resolution);
    if(context == nullptr) return;
    std::shared_ptr<AbstrRenderer> absrend = initNewRenderer(context, resolution, dataset, tf, ioHost, ioPort);
    if(absrend == nullptr) return;


    //set final entry
    ServiceEntry e(serviceHandle, absrend, context);
    e.setCanceled(false);

    auto it = m_ServiceMap.find(serviceHandle);
    if (it != m_ServiceMap.end()) it->second = e;

    //reload entry to be safe to use the right one!
    e = m_ServiceMap.find(serviceHandle)->second;


    int numb = 0;
    string name;
    Core::Time::Timer timer;

    timer.start();
    double startTime = timer.elapsed();
    double el = startTime;

    std::cout << timer.elapsed() << std::endl;

    while(!e.getIsCanceled()){
        {
            std::lock_guard<recursive_mutex> lock(m_rsMutex);


            e.getContext()->activateContext();
            e.getRenderer()->Paint();
            e.getContext()->frameFinished();

            if(e.getThreadCommands()->m_bShouldPick){
                e.getRenderer()->readVolumePosition(e.getThreadCommands()->m_cv_picking);
                e.getThreadCommands()->m_bShouldPick = false;
            }

            if(e.getThreadCommands()->m_bSetCamera){
                e.getRenderer()->SetCamera(e.getRenderState()->m_pCameraPtr);
                e.getThreadCommands()->m_bSetCamera = false;
            }

           /* if(visibility == Visibility::hidden){
                if((numb % 100) == 0){
                    name = "hidden_ser"+std::to_string(serviceHandle)+"_"+ std::to_string(numb);
                    e.getContext()->storeFinalFrameToTNG(name);
                }
                numb++;
            }*/
        }
       /* ++numb;
        if(numb % 10 == 0){
            el = timer.elapsed();
            std::cout <<  ((el-startTime)/10.0) << std::endl;
            startTime = el;
        }
*/

         std::this_thread::sleep_for(std::chrono::milliseconds(33));
         //std::this_thread::sleep_for(std::chrono::nanoseconds(500));
    }

    e.getRenderer()->Cleanup();
    e.getContext()->deleteContext();

    it = m_ServiceMap.find(serviceHandle);
    m_ServiceMap.erase(it);
}

std::shared_ptr<Tuvok::Renderer::Context::Context> RenderServer::getContextPtr(uint16_t handle){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    if(!e.getIsCanceled()){
        return e.getContext();
    }
    return nullptr;
}

std::shared_ptr<AbstrRenderer> RenderServer::getRendererPtr(uint16_t handle){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    if(!e.getIsCanceled()){
        return e.getRenderer();
    }
    return nullptr;
}

//! Below this line you find interaction methods with the renderer -------------------------------------------
uint8_t RenderServer::checkIfRenderThreadIsRunning(uint16_t handle){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    if(e.getIsCanceled()){
        return 0;
    }else{
        return 1;
    }
}
void RenderServer::stopRenderThread(uint16_t handle){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.setCanceled(true);
}

void RenderServer::rotateCamera(uint16_t handle, Core::Math::Vec3f rotate){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.getRenderState()->m_pCameraPtr.rotateCamera(rotate);
    e.getThreadCommands()->m_bSetCamera = true;
}
void RenderServer::moveCamera(uint16_t handle, Core::Math::Vec3f direction){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.getRenderState()->m_pCameraPtr.moveCamera(direction);
    e.getThreadCommands()->m_bSetCamera = true;
}
void RenderServer::toogleFirstPerson(uint16_t handle, bool toggle){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.getRenderer()->SetFirstPersonMode(toggle);
}
void RenderServer::zoomCamera(uint16_t handle, float zoom){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.getRenderState()->m_pCameraPtr.setZoom(zoom);
    e.getThreadCommands()->m_bSetCamera = true;
}

void RenderServer::setRenderMode(uint16_t handle, Tuvok::Renderer::ERenderMode mode){
    ServiceEntry e = m_ServiceMap.find(handle)->second;

    e.getRenderer()->SetRenderMode(mode);
}

void RenderServer::setCompositeMode(uint16_t handle, Tuvok::Renderer::ECompositeDisplay mode){
    ServiceEntry e = m_ServiceMap.find(handle)->second;

    e.getRenderer()->SetCompositeMode(mode);
}

void RenderServer::clearViewPicking(uint16_t handle, Core::Math::Vec2ui pixelposition){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.getThreadCommands()->m_bShouldPick = true;
    e.getThreadCommands()->m_cv_picking = pixelposition;
}

void RenderServer::setClearViewEnabled(uint16_t handle,bool b){
    ServiceEntry e = m_ServiceMap.find(handle)->second;
    e.getRenderer()->setClearViewEnabled(b);
}

void RenderServer::setClearViewRadius(uint16_t handle, float f){
	ServiceEntry e = m_ServiceMap.find(handle)->second;
	e.getRenderer()->setClearViewRadius(f);
}
