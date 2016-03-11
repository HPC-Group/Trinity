#pragma once

#include <silverbullet/base/DetectEnv.h>
#include "OpenGLincludes.h"
/*
#ifdef DETECTED_OS_APPLE
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLCurrent.h>
#endif

#ifdef DETECTED_OS_WINDOWS
#include <iosfwd>
#include <windows.h>
#include <GL/glew.h>
#include <GL/gl.h>
#endif

#ifdef DETECTED_OS_LINUX
#include <GL/glew.h>
#include <EGL/egl.h>
#include <GL/gl.h>
#endif
*/

#include <string>

class OpenGlHeadlessContext {
public :
  OpenGlHeadlessContext();
  ~OpenGlHeadlessContext();

  bool isValid() const {return bIsValid;}

  float getVersion() const;
  std::string getVersionInfo() const {return strVersion;}
  std::string getRendererInfo() const {return strRenderer;}

  void makeCurrent();
private:
#ifdef DETECTED_OS_APPLE
  CGLContextObj context;
#endif
  
#ifdef DETECTED_OS_WINDOWS
  HGLRC context;
  HDC deviceContext;
  HWND window;
#endif

#ifdef DETECTED_OS_LINUX
  EGLDisplay eglDpy;
  EGLContext eglCtx;
#endif
  
  bool bIsValid;
  std::string strVersion;
  std::string strRenderer;

  void createContext();
  void destroyContext();

};

