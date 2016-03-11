#pragma once 

#include <silverbullet/base/DetectEnv.h>

#include <opengl-base/OpenGLincludes.h>

#include <string>

class OpenGlHeadlessContext {
public :
  OpenGlHeadlessContext();
  ~OpenGlHeadlessContext();

  bool isValid() const {return bIsValid;}
  void makeCurrent();

  float getVersion() const;
  std::string getVersionInfo() const {return strVersion;}
  std::string getRendererInfo() const {return strRenderer;}

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
#endif
  
  bool bIsValid;
  std::string strVersion;
  std::string strRenderer;

  void createContext();
  void destroyContext();

};
