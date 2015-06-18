#ifndef TUVOK_XCONTEXT
#define TUVOK_XCONTEXT


#include "Context.h"

#include "../OpenGL/OpenGLDefines.h"

#ifdef LINUXOS

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);
typedef Bool (*glXMakeContextCurrentARBProc)(Display*, GLXDrawable, GLXDrawable, GLXContext);
static glXCreateContextAttribsARBProc glXCreateContextAttribsARB = NULL;
static glXMakeContextCurrentARBProc   glXMakeContextCurrentARB   = NULL;

namespace Tuvok{
    namespace Renderer{
        namespace Context{

            class XserverContext: public Context{
            public:
                XserverContext(Core::Math::Vec2ui resolution = Core::Math::Vec2ui(640,480),
                            uint8_t MajorVersion = 4,
                            uint8_t MinorVersion = 3);
                ~XserverContext();

                void lockContext() override;
                void unlockContext() override;
                bool initContext() override;
                bool deleteContext() override;

                void* getContextItem() override;

                void frameFinished() override;

                int storeFinalFrameToTNG(std::string name) override;

                void ReadBackBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount) override;

            private:
                Display* display;
                GLXPbuffer pbuffer;
                GLXContext openGLContext;

                uint8_t                 m_uiMajor;
                uint8_t                 m_uiMinor;

            };

        };
    };
};
#endif
#endif
