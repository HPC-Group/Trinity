#include "XContext.h"
#ifdef LINUXOS
using namespace Tuvok::Renderer::Context;
using namespace Tuvok::Renderer;

XserverContext::XserverContext(Core::Math::Vec2ui resolution):
Context("XRenderer",System::Linux,Visibility::hidden,resolution){}
XserverContext::~XserverContext(){}

void XserverContext::activateContext(){
    if(!glXMakeContextCurrent( display, pbuffer, pbuffer, openGLContext )){
        std::cout << "somehting is wrong" <<std::endl;
    }
}
bool XserverContext::initContext() {
    IVDA_MESSAGE("checking for glx extensions");
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
    glXMakeContextCurrentARB   = (glXMakeContextCurrentARBProc)   glXGetProcAddressARB( (const GLubyte *) "glXMakeContextCurrent"      );

    IVDA_MESSAGE("finding displays");
    const char *displayName = NULL;
    display = XOpenDisplay( displayName );

    IVDA_MESSAGE("something");
    static int visualAttribs[] = { None };
    int numberOfFramebufferConfigurations = 0;
    GLXFBConfig* fbConfigs = glXChooseFBConfig( display, DefaultScreen(display), visualAttribs, &numberOfFramebufferConfigurations );

    IVDA_MESSAGE("define context");
    int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 3,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    IVDA_MESSAGE("creating context");
    openGLContext = glXCreateContextAttribsARB( display, fbConfigs[0], 0, True, context_attribs);


    int pbufferAttribs[] = {
        GLX_PBUFFER_WIDTH,  640,
        GLX_PBUFFER_HEIGHT, 480,
        None
    };
    pbuffer = glXCreatePbuffer( display, fbConfigs[0], pbufferAttribs );

    // clean up:
    XFree( fbConfigs );
    XSync( display, False );

    if ( !glXMakeContextCurrent( display, pbuffer, pbuffer, openGLContext ) )
    {
    return false;
        // something went wrong
    }else{
         // ---------------- AFTER CONTEXT CREATION INITS ------------------------------------

        //Windows needs glew
        if(m_eSystem == System::Windows){
            //Initialize GLEW
            GLenum err = glewInit();

            //If GLEW hasn't initialized
            if (err != GLEW_OK)
            {
                fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
                return false;

            }
        //Linux glew hack experimental
        }else if(m_eSystem == System::Linux){
            //Initialize GLEW
            glewExperimental = GL_TRUE; //dirty cheating
            GLenum err = glewInit();

            //If GLEW hasn't initialized
            if (err != GLEW_OK)
            {
                fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
                return false;

            }
        }else{

        }


        std::cout << "context ready"<<std::endl;
        std::cout << "OPENGL VERSION " << glGetString(GL_VERSION)<< std::endl;
    }
    glGetError();
    return true;
}

bool XserverContext::deleteContext(){
    glXDestroyContext(display,openGLContext);

    return true;
}

void* XserverContext::getContextItem(){
    return &openGLContext;
}

void XserverContext::frameFinished(){
}

int XserverContext::storeFinalFrameToTNG(std::string name){
    unsigned char *pixels;
	FILE *image;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	pixels = new unsigned char[viewport[2] * viewport[3] * 3];

	glReadPixels(0, 0, viewport[2], viewport[3], GL_BGR,
		GL_UNSIGNED_BYTE, pixels);

	char tempstring[50];
	sprintf(tempstring, "screens/screenshot_%s_%i.tga",name.c_str(), 0);
	if ((image = fopen(tempstring, "wb")) == NULL) return 1;

	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	unsigned char header[6] = { static_cast<unsigned char>(((int)(viewport[2] % 256))),
		static_cast<unsigned char>(((int)(viewport[2] / 256))),
		static_cast<unsigned char>(((int)(viewport[3] % 256))),
		static_cast<unsigned char>(((int)(viewport[3] / 256))), 24, 0 };

	// TGA header schreiben
	fwrite(TGAheader, sizeof(unsigned char), 12, image);
	// Header schreiben
	fwrite(header, sizeof(unsigned char), 6, image);

	fwrite(pixels, sizeof(unsigned char),
		viewport[2] * viewport[3] * 3, image);

	fclose(image);
	delete[] pixels;

	return 0;
}
#endif
