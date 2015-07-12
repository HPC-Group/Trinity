#ifndef TUVOK_RENDERMANAGER
#define TUVOK_RENDERMANAGER

#include "IRenderer.h"
#include "RenderEnums.h"
#include "Context/Context.h"
#include <memory>

namespace Tuvok{
    namespace Renderer{

    class RenderManager{
    public:
        static RenderManager& getInstance(){
             static RenderManager    instance;
             return instance;
        }

        std::shared_ptr<IRenderer> createRenderer(
                                                        Visibility visibility,
                                                        Core::Math::Vec2ui resolution,
                                                        std::string dataset,
                                                        std::string tf);

        std::shared_ptr<IRenderer> createRenderer(  std::shared_ptr<Tuvok::Renderer::Context::Context> contextPtr,
                                                        std::string dataset,
                                                        std::string tf);
    private:
        RenderManager(){};
        ~RenderManager(){};
    };

    };
};

#endif //RENDER
