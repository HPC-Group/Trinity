#ifndef TUVOK_IRENDERMANAGER
#define TUVOK_IRENDERMANAGER

#include "IRenderer.h"
#include "RenderEnums.h"
#include "Context/Context.h"
#include <memory>

namespace Tuvok{
    namespace Renderer{

    class IRenderManager{
    public:
        virtual std::shared_ptr<IRenderer> createRenderer(
                                                        Visibility visibility,
                                                        Core::Math::Vec2ui resolution,
                                                        std::string dataset,
                                                        std::string tf) = 0;

        virtual std::shared_ptr<IRenderer> createRenderer(  std::shared_ptr<Tuvok::Renderer::Context::Context> contextPtr,
                                                        std::string dataset,
                                                        std::string tf) = 0;

        virtual std::shared_ptr<IRenderer> createRenderer(      std::string ip,
                                                        int port,
                                                        Visibility visibility,
                                                        Core::Math::Vec2ui resolution,
                                                        std::string dataset,
                                                        std::string tf) = 0;
    };

    };
};

#endif //RENDER
