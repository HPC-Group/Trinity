#include "OpenGlHeadlessContext.h"

#include <stdlib.h>     /* atof */

#include <iostream>

OpenGlHeadlessContext::OpenGlHeadlessContext() :
#ifdef DETECTED_OS_APPLE
context(0),
#endif
#ifdef DETECTED_OS_WINDOWS
context(0),
deviceContext(0),
window(0),
#endif
bIsValid(false),
strRenderer(""),
strVersion("")
{
  createContext();
}

OpenGlHeadlessContext::~OpenGlHeadlessContext() {
  destroyContext();
}

void OpenGlHeadlessContext::createContext() {
#ifdef DETECTED_OS_APPLE
  CGLPixelFormatAttribute attributes[4] = {
    kCGLPFAAccelerated,   // no software rendering
    kCGLPFAOpenGLProfile, // core profile with the version stated below
    (CGLPixelFormatAttribute) kCGLOGLPVersion_3_2_Core,
    (CGLPixelFormatAttribute) 0
  };

  CGLPixelFormatObj pix;
  CGLError errorCode;
  GLint num; // stores the number of possible pixel formats
  errorCode = CGLChoosePixelFormat( attributes, &pix, &num );
  if (errorCode != kCGLNoError) {bIsValid = false; return;}

  errorCode = CGLCreateContext( pix, NULL, &context ); // second parameter can be another context for object sharing
  if (errorCode != kCGLNoError) {bIsValid = false; return;}

  CGLDestroyPixelFormat( pix );

  errorCode = CGLSetCurrentContext( context );
  if (errorCode != kCGLNoError) {bIsValid = false; return;}

#endif
  
#ifdef DETECTED_OS_WINDOWS
  window = CreateWindowExW(WS_EX_TOOLWINDOW, L"Static", L"OpenGLContextWindow",
                           WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                           0, 0, 1, 1, 0, 0, GetModuleHandle(NULL), 0);
  if (!window) {bIsValid = false; return;}
  ShowWindow(window, SW_HIDE);
  
  deviceContext = GetDC(window);
  if (!deviceContext) {bIsValid = false; return;}

  PIXELFORMATDESCRIPTOR pfd;
  pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
  pfd.nVersion = 1;
  pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
  pfd.iPixelType = PFD_TYPE_RGBA;
  pfd.cColorBits = 32;
  pfd.cRedBits = 0;
  pfd.cRedShift = 0;
  pfd.cGreenBits = 0;
  pfd.cGreenShift = 0;
  pfd.cBlueBits = 0;
  pfd.cBlueShift = 0;
  pfd.cAlphaBits = 0;
  pfd.cAlphaShift = 0;
  pfd.cAccumBits = 0;
  pfd.cAccumRedBits = 0;
  pfd.cAccumGreenBits = 0;
  pfd.cAccumBlueBits = 0;
  pfd.cAccumAlphaBits = 0;
  pfd.cDepthBits = 24;
  pfd.cStencilBits = 8;
  pfd.cAuxBuffers = 0;
  pfd.iLayerType = PFD_MAIN_PLANE;
  pfd.bReserved = 0;
  pfd.dwLayerMask = 0;
  pfd.dwVisibleMask = 0;
  pfd.dwDamageMask = 0;
  
  int pixelFormat = ChoosePixelFormat(deviceContext, &pfd);
  if (!pixelFormat) {bIsValid = false; return;}
  
  PIXELFORMATDESCRIPTOR pfdResult;
  DescribePixelFormat(deviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfdResult);
  
  if (!(pfdResult.dwFlags & PFD_SUPPORT_OPENGL)) {bIsValid = false; return;}
  
  if (!SetPixelFormat(deviceContext, pixelFormat, &pfd)) {bIsValid = false; return;}
  
  context = wglCreateContext(deviceContext);
  if (!context) {bIsValid = false; return;}

  if(!wglMakeCurrent(deviceContext, context)) { bIsValid = false; return; }

  GLenum err = glewInit();
  if (GLEW_OK != err) {bIsValid = false; return;}
  
#endif

#ifdef DETECTED_OS_LINUX
  static const EGLint configAttribs[] = {
    EGL_SURFACE_TYPE,
    EGL_BLUE_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_RED_SIZE, 8,
    EGL_DEPTH_SIZE, 8,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
    EGL_NONE
  };

  
  // Initialize EGL
  eglDpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);

  EGLint major=0, minor=0;
  if(eglInitialize(eglDpy, &major, &minor) != EGL_TRUE) { bIsValid = false; return; }
  
  // Select an appropriate configuration
  EGLint numConfigs;
  EGLConfig eglCfg;
  
  if (eglChooseConfig(eglDpy, configAttribs, &eglCfg, 1, &numConfigs) != EGL_TRUE) { bIsValid = false; return; }
  
  // Bind the API
  if (eglBindAPI(EGL_OPENGL_API) != EGL_TRUE) { bIsValid = false; return; }
  
  // Create a context and make it current
  EGLContext eglCtx = eglCreateContext(eglDpy, eglCfg, EGL_NO_CONTEXT,
                                       NULL);
  
  if (eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx) != EGL_TRUE) { bIsValid = false; return; }
#endif
  
  
  const char* r = (const char*)glGetString(GL_RENDERER);
  const char* v = (const char*)glGetString(GL_VERSION);

  if (r) strRenderer = std::string(r);
  if (v) strVersion = std::string(v);
  
  bIsValid = true;
}


void OpenGlHeadlessContext::destroyContext() {
#ifdef DETECTED_OS_APPLE
  CGLSetCurrentContext( NULL );
  CGLDestroyContext( context );
#endif
  
#ifdef DETECTED_OS_WINDOWS
  wglMakeCurrent(0,0);
  wglDeleteContext(context);
  ReleaseDC(window, deviceContext);
  DestroyWindow(window);
#endif
  
#ifdef DETECTED_OS_LINUX
  eglTerminate(eglDpy);
#endif
  
}

float OpenGlHeadlessContext::getVersion() const {
  if(bIsValid && strVersion.find_first_of(" ") != std::string::npos) {
    const std::string num = strVersion.substr(0,strVersion.find_first_of(" "));
    return float(atof(num.c_str()));
  } else {
    return 0;
  }
}

void OpenGlHeadlessContext::makeCurrent(){
#ifdef DETECTED_OS_APPLE
    CGLError errorCode = CGLSetCurrentContext( context );
    if (errorCode != kCGLNoError) {bIsValid = false; return;}
#endif

#ifdef DETECTED_OS_WINDOWS
    if(!wglMakeCurrent(deviceContext, context)) { bIsValid = false; return; }
#endif

#ifdef DETECTED_OS_LINUX
    //if (eglMakeCurrent(eglDpy, EGL_NO_SURFACE, EGL_NO_SURFACE, eglCtx) != EGL_TRUE) { bIsValid = false; return; }
#endif
}




