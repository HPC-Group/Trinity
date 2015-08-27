//
//  OpenGLDefines.h
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef TNG_OpenGLDefines_h
#define TNG_OpenGLDefines_h

#if defined __APPLE__
    #include <GL/glew.h>
    #include <OpenGL/gl3.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#elif defined _WIN32
    #include <windows.h>
    #include <GL/glew.h>
    #include <gl/GL.h>
    #include <stdint.h>
    #define DETECTED_OS_WINDOWS
#else
    #include <GL/glew.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glut.h>
    #include <GL/glx.h>
    #define LINUXOS
#endif

#endif
