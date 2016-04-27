#pragma once

#include <opengl-base/OpenGLincludes.h>

void CheckOpenGLError(const char* stmt, GLenum e, const char* fname, int line);

#define MAX_GL_ERROR_COUNT 10

#ifndef NDEBUG
# define GL_CHECK_EXT()                                                \
  do {                                                                 \
    GLenum glerr;                                                      \
    unsigned int iCounter = 0;                                         \
    while((glerr = glGetError()) != GL_NO_ERROR) {                     \
      CheckOpenGLError("GL_CHECK", glerr, __FILE__, __LINE__);         \
      iCounter++;                                                      \
      if (iCounter > MAX_GL_ERROR_COUNT) break;                        \
    }                                                                  \
  } while(0)
#else
# define GL_CHECK_EXT()
#endif

#ifndef NDEBUG
# define GL_CHECK(stmt)                                                \
  do {                                                                 \
    GLenum glerr;                                                      \
    unsigned int iCounter = 0;                                         \
    while((glerr = glGetError()) != GL_NO_ERROR) {                     \
      CheckOpenGLError("before", glerr, __FILE__, __LINE__);           \
      iCounter++;                                                      \
      if (iCounter > MAX_GL_ERROR_COUNT) break;                        \
    }                                                                  \
    stmt;                                                              \
    iCounter = 0;                                                      \
    while((glerr = glGetError()) != GL_NO_ERROR) {                     \
      CheckOpenGLError(#stmt, glerr, __FILE__, __LINE__);              \
      iCounter++;                                                      \
      if (iCounter > MAX_GL_ERROR_COUNT) break;                        \
    }                                                                  \
  } while(0)
#else
# define GL_CHECK(stmt) do { stmt; } while(0)
#endif

