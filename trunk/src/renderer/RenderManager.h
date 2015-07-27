#ifndef TUVOK_RENDERMANAGER
#define TUVOK_RENDERMANAGER

#include "IRenderManager.h"

namespace Tuvok{
    namespace Renderer{

    class RenderManager : public IRenderManager{
    public:
        static RenderManager& getInstance(){
             static RenderManager    instance;
             return instance;
        }

        std::shared_ptr<IRenderer> createRenderer(
                                                        Visibility visibility,
                                                        Core::Math::Vec2ui resolution,
                                                        std::string dataset,
                                                        std::string tf) ;

        std::shared_ptr<IRenderer> createRenderer(  std::shared_ptr<Tuvok::Renderer::Context::Context> contextPtr,
                                                        std::string dataset,
                                                        std::string tf) ;
    private:
        RenderManager(){};
        ~RenderManager(){};
    };

    };
};

#endif //RENDER
