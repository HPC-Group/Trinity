//
//  GLVolume.cpp
//  TNG
//
//  Created by Andre Waschk on 22/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "GLVolume.h"

using namespace Tuvok::Renderer::OpenGL::GLVolume;

GLVolume::GLVolume(uint32_t iSizeX, uint32_t iSizeY, uint32_t iSizeZ,
                   GLint , GLenum , GLenum ,
                   const GLvoid *,
                   GLint iMagFilter,
                   GLint iMinFilter,
                   GLint ,
                   GLint ,
                   GLint ) :
m_iMagFilter(iMagFilter),
m_iMinFilter(iMinFilter)
{
}

GLVolume::GLVolume() {
}

GLVolume::~GLVolume() {
}

void GLVolume::SetFilter(GLint iMagFilter, GLint iMinFilter) {
  m_iMagFilter = iMagFilter;
  m_iMinFilter = iMinFilter;
}
