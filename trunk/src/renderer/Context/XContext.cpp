#include "XContext.h"

#ifdef LINUXOS
using namespace Tuvok::Renderer::Context;
using namespace Tuvok::Renderer;

XserverContext::XserverContext(Core::Math::Vec2ui resolution,
                            uint8_t MajorVersion,
                            uint8_t MinorVersion):
Context("XRenderer",System::Linux,Visibility::hidden,resolution),
m_uiMajor(MajorVersion),
m_uiMinor(MinorVersion){}
XserverContext::~XserverContext(){}

void XserverContext::lockContext(){
    if(!glXMakeContextCurrent( display, pbuffer, pbuffer, openGLContext )){
        std::cout << "somehting is wrong" <<std::endl;
    }
}

void XserverContext::unlockContext(){

}
bool XserverContext::initContext() {
    LINFOC("XCONTEXT","checking for glx extensions");
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc) glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
    glXMakeContextCurrentARB   = (glXMakeContextCurrentARBProc)   glXGetProcAddressARB( (const GLubyte *) "glXMakeContextCurrent"      );

    LINFOC("XCONTEXT","finding displays");
    const char *displayName = NULL;
    display = XOpenDisplay( displayName );

    static int visualAttribs[] = { None };
    int numberOfFramebufferConfigurations = 0;
    GLXFBConfig* fbConfigs = glXChooseFBConfig( display, DefaultScreen(display), visualAttribs, &numberOfFramebufferConfigurations );

    LINFOC("XCONTEXT","define context");
    int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, m_uiMajor,
        GLX_CONTEXT_MINOR_VERSION_ARB, m_uiMinor,
        GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_DEBUG_BIT_ARB,
        GLX_CONTEXT_PROFILE_MASK_ARB, GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
        None
    };

    LINFOC("XCONTEXT","creating context");
    openGLContext = glXCreateContextAttribsARB( display, fbConfigs[0], 0, True, context_attribs);


    int pbufferAttribs[] = {
        GLX_PBUFFER_WIDTH,  (int)m_vResolution.x,
        GLX_PBUFFER_HEIGHT, (int)m_vResolution.y,
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

        LINFOC("XCONTEXT","OpenGL version: " << glGetString(GL_VERSION));
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

void XserverContext::ReadBackBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount){
 lockContext();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	width = viewport[2];
	height = viewport[3];
	componentCount = 3;

	pixels.resize(viewport[2] * viewport[3] * 3);
	//glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGB,
		GL_UNSIGNED_BYTE, &pixels[0]);

    unlockContext();
}
#endif
