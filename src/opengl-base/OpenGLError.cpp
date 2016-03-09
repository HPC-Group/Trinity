#pragma once 

#include "OpenGLError.h"
#include <iostream>

void CheckOpenGLError(const char* stmt, const char* fname, int line) {
  GLenum err = glGetError();
  unsigned int c = 0;
  while (err != GL_NO_ERROR && c++ < 10)
  {
    std::printf("OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt);
    err = glGetError();
	while (true);
  }
}
