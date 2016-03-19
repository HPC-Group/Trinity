#include "OpenGLError.h"
#include <iostream>
#include <string>

static std::string errToString(GLenum e) {
  switch (e) {
    case 0 : return "NO_ERROR";
    case 0x0500 : return "INVALID_ENUM";
    case 0x0501 : return "INVALID_VALUE";
    case 0x0502 : return "INVALID_OPERATION";
    case 0x0503 : return "STACK_OVERFLOW";
    case 0x0504 : return "STACK_UNDERFLOW";
    case 0x0505 : return "OUT_OF_MEMORY";
    case 0x0506 : return "INVALID_FRAMEBUFFER_OPERATION";
  }
  return "UNKNOWN GL-ERROR CODE";
}


void CheckOpenGLError(const char* stmt, GLenum e, const char* fname, int line) {
  std::printf("OpenGL error %s (%08x), at %s:%i - for %s\n",
              errToString(e).c_str(),
              e, fname, line, stmt);
}
