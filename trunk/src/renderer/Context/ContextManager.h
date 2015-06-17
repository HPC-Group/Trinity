#ifndef TUVOK_CONTEXTMANAGER
#define TUVOK_CONTEXTMANAGER

#include "logging/logmanager.h"

#include <core/Math/Vectors.h>
#include <memory>

#include "Context.h"
#include <renderer/RenderEnums.h>

namespace Tuvok{
    namespace Renderer{
        namespace Context{

        class ContextManager{
        public:
            static ContextManager& getInstance(){
             static ContextManager    instance;
             return instance;
            }

            std::shared_ptr<Tuvok::Renderer::Context::Context> createContext(Tuvok::Renderer::Visibility visibility, Core::Math::Vec2ui resolution, uint8_t major = 4, uint8_t minor = 3);

        private:
            ContextManager(){};
            ~ContextManager(){};
       };

        };
    };
};

#endif
