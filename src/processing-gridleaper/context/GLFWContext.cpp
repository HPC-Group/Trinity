#include "GLFWContext.h"

using namespace Tuvok::Renderer::Context;
using namespace Tuvok::Renderer;

GLFWContext::GLFWContext(std::string windowname,
                            System system,
                            Visibility visibility,
                            Core::Math::Vec2ui resolution,
                            uint8_t MajorVersion,
                            uint8_t MinorVersion):
Context(windowname,system,visibility,resolution),
m_uiMajor(MajorVersion),
m_uiMinor(MinorVersion){}
GLFWContext::~GLFWContext(){}

void GLFWContext::lockContext(){
    ContextMutex::getInstance().lockContext();
    glfwMakeContextCurrent(m_windowContext);
}

void GLFWContext::unlockContext(){
    glfwMakeContextCurrent(NULL);
    ContextMutex::getInstance().unlockContext();
}
bool GLFWContext::initContext() {
        // ## Init a GLFW window to get a opengl context

        glfwInit();

        //Set some window hints for linux and osx, not needed on windows
        if((m_eSystem == System::Linux || m_eSystem == System::OSX) && m_uiMajor != 0){
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_uiMajor);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_uiMinor);
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }else{

        }

        // Open a window and create its OpenGL context
        if (m_eVisibility == Visibility::Fullscreen){
			m_windowContext = glfwCreateWindow(m_vResolution.x, m_vResolution.y, m_sName.c_str(), glfwGetPrimaryMonitor(), NULL);
		}
		else{
			m_windowContext = glfwCreateWindow(m_vResolution.x, m_vResolution.y, m_sName.c_str(), NULL, NULL);
		}


        if( m_windowContext == NULL ){
            glfwTerminate();
            return false;
        }
        if(m_eVisibility == Visibility::hidden){
            glfwHideWindow(m_windowContext);
        }
        glfwMakeContextCurrent(m_windowContext);

        glfwSetInputMode(m_windowContext, GLFW_STICKY_KEYS, GL_TRUE);

        LINFOC("GLFWContext","OpenGL version: "<< glGetString(GL_VERSION));
        // ## Finished GLFW Init, got a working context at this point. Now Tuvok.

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
        //catch random glew errors and clean the error
        glGetError();
        return true;
}

bool GLFWContext::deleteContext(){
    glfwDestroyWindow(m_windowContext);
    return true;
}

void* GLFWContext::getContextItem(){
    return m_windowContext;
}

void GLFWContext::frameFinished(){
        glfwSwapBuffers(m_windowContext);
		glfwPollEvents();
}

int GLFWContext::storeFinalFrameToTNG(std::string name){
    unsigned char *pixels;
	FILE *image;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	pixels = new unsigned char[viewport[2] * viewport[3] * 3];

	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGB,
		GL_UNSIGNED_BYTE, pixels);

	char tempstring[50];
	sprintf(tempstring, "screenshot_%s_%i.tga",name.c_str(), 0);
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

void GLFWContext::ReadBackBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount){
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
