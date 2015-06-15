#ifndef TUVOK_GLFWCONTEXT
#define TUVOK_GLFWCONTEXT


#include "Context.h"

#include "../OpenGL/OpenGLDefines.h"

#include <GLFW/glfw3.h>

namespace Tuvok{
    namespace Renderer{
        namespace Context{

            class GLFWContext: public Context{
            public:
                GLFWContext(std::string windowname = "renderer",
                            System system = System::Windows,
                            Visibility visibility = Visibility::Windowed,
                            Core::Math::Vec2ui resolution = Core::Math::Vec2ui(640,480));
                ~GLFWContext();

                void lockContext() override;
                void unlockContext() override;
                bool initContext() override;
                bool deleteContext() override;

                void* getContextItem() override;

                void frameFinished() override;

                int storeFinalFrameToTNG(std::string name) override;

            private:
                GLFWwindow*             m_windowContext;
            };

        };
    };
};

#endif
