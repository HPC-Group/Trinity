#ifndef TUVOK_XCONTEXT
#define TUVOK_XCONTEXT


#include "Context.h"

#include "../OpenGL/OpenGLDefines.h"
#define LINUXOS
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
                XserverContext(Core::Math::Vec2ui resolution = Core::Math::Vec2ui(640,480));
                ~XserverContext();

                void lockContext() override;
                void unlockContext() override;
                bool initContext() override;
                bool deleteContext() override;

                void* getContextItem() override;

                void frameFinished() override;

                int storeFinalFrameToTNG(std::string name) override;

            private:
                Display* display;
                GLXPbuffer pbuffer;
                GLXContext openGLContext;

            };

        };
    };
};
#endif
#endif
