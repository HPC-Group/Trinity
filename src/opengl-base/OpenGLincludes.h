#pragma once 

#include <silverbullet/base/DetectEnv.h>

#ifdef DETECTED_OS_APPLE

# define __gl_h_ 1 // make sure the gl header file is not included
# define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED // disable warning

#include <OpenGL/gl3.h>
#include <OpenGL/OpenGL.h>

#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLCurrent.h>
#endif

#ifdef DETECTED_OS_WINDOWS
#define NOMINMAX
#include <iosfwd>
#include <windows.h>
#include <opengl-base/GL/glew.h>
#include <GL/gl.h>
#endif

#ifdef DETECTED_OS_LINUX
#include <opengl-base/GL/glew.h>
#include <EGL/egl.h>
#include <GL/gl.h>
#endif
