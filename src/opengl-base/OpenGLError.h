#pragma once 

#include <opengl-base/OpenGLincludes.h>

void CheckOpenGLError(const char* stmt, const char* fname, int line);

#ifndef NDEBUG
  #define GL_CHECK(stmt) do { \
      stmt; \
      CheckOpenGLError(#stmt, __FILE__, __LINE__); \
    } while (0)
  #else
    #define GL_CHECK(stmt) stmt
#endif

