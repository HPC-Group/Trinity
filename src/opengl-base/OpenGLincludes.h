#pragma once 

#include <silverbullet/base/DetectEnv.h>

#ifdef DETECTED_OS_APPLE
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl3.h>

#include <OpenGL/CGLTypes.h>
#include <OpenGL/CGLCurrent.h>
#endif

#ifdef DETECTED_OS_WINDOWS
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
