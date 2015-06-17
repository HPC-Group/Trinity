#include "ContextManager.h"

#include "GLFWContext.h"
#include "XContext.h"

#include <renderer/OpenGL/OpenGLDefines.h>

using namespace Tuvok::Renderer::Context;
using namespace Tuvok::Renderer;
using namespace Tuvok;
using std::shared_ptr;

std::shared_ptr<Tuvok::Renderer::Context::Context> ContextManager::createContext(Tuvok::Renderer::Visibility visibility, Core::Math::Vec2ui resolution, uint8_t major, uint8_t minor){

 std::shared_ptr<Tuvok::Renderer::Context::Context> context;
 System m_eServerSystem;

#if defined __APPLE__
    m_eServerSystem = System::OSX;
#elif defined _WIN32
    m_eServerSystem = System::Windows;
#else
    m_eServerSystem = System::Linux;
#endif

    if(visibility == Visibility::hidden){
        if(m_eServerSystem == System::Linux){
#ifdef LINUXOS
            context = std::make_shared<Tuvok::Renderer::Context::XserverContext>(resolution,major,minor);
#endif
        }else{
            context = std::make_shared<Tuvok::Renderer::Context::GLFWContext>("Window",m_eServerSystem,visibility,resolution,major,minor);
        }
    }else{
        context = std::make_shared<Tuvok::Renderer::Context::GLFWContext>("Window",m_eServerSystem,visibility,resolution,major,minor);
    }

	if (context == nullptr) return 0;
    if(!context->initContext()) return 0;
    LINFOC("RenderServer", "created a new context for this thread");

    return context;
}
