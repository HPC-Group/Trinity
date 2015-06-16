#ifndef TUVOK_RENDERMANAGER
#define TUVOK_RENDERMANAGER

#include "AbstrRenderer.h"
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

        std::shared_ptr<AbstrRenderer> createRenderer(
                                                        Visibility visibility,
                                                        Core::Math::Vec2ui resolution,
                                                        std::string dataset,
                                                        std::string tf);

        std::shared_ptr<AbstrRenderer> createRenderer(  std::shared_ptr<Tuvok::Renderer::Context::Context> contextPtr,
                                                        std::string dataset,
                                                        std::string tf);
    private:
        RenderManager(){};
        ~RenderManager(){};
    };

    };
};

#endif //RENDER
