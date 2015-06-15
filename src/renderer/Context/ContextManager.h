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

            std::shared_ptr<Tuvok::Renderer::Context::Context> createContext(Tuvok::Renderer::Visibility visibility, Core::Math::Vec2ui resolution);

        private:
            ContextManager(){};
            ~ContextManager(){};
       };

        };
    };
};

#endif
