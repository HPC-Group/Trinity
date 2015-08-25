//
//  GLVolume3DTex.cpp
//  TNG
//
//  Created by Andre Waschk on 22/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "GLVolume3DTex.h"
#include <renderer/OpenGL/GLCore/GLTexture3D.h>
#include <renderer/OpenGL/GLCore/GLProgram.h>

using namespace Tuvok::Renderer::OpenGL::GLVolume;
using namespace Tuvok::Renderer::OpenGL::GLCore;

GLVolume3DTex::GLVolume3DTex() :
GLVolume(0, 0, 0, 0, 0, 0,
         NULL, GL_NEAREST, GL_NEAREST,
         GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,
         GL_CLAMP_TO_EDGE),
m_pTexture(NULL)
{
}

GLVolume3DTex::GLVolume3DTex(uint32_t iSizeX, uint32_t iSizeY, uint32_t iSizeZ,
                             GLint internalformat, GLenum format, GLenum type,
                             const GLvoid *voxels,
                             GLint iMagFilter,
                             GLint iMinFilter,
                             GLint wrapX,
                             GLint wrapY,
                             GLint wrapZ) :
GLVolume(iSizeX, iSizeY, iSizeZ, internalformat, format, type,
         voxels, iMagFilter, iMinFilter,wrapX,
         wrapY, wrapZ),
m_pTexture(new GLTexture3D(iSizeX,  iSizeY,  iSizeZ,
                           internalformat,  format,  type,
                           voxels, iMagFilter,
                           iMinFilter, wrapX, wrapY, wrapZ))
{
}

GLVolume3DTex::~GLVolume3DTex() {
  FreeGLResources();
}

void GLVolume3DTex::Bind(uint32_t iUnit) {
  m_pTexture->Bind(iUnit);
}

void GLVolume3DTex::Bind(GLProgramPtr glslProgram, const std::string& name){
  glslProgram->SetTexture(name, m_pTexture);
}

void GLVolume3DTex::FreeGLResources() {
  if (m_pTexture) {
    m_pTexture->Delete();
    m_pTexture.reset();
  }
  m_pTexture = NULL;
}

void GLVolume3DTex::SetData(const void *voxels) {
  if (m_pTexture) m_pTexture->SetData(voxels);
}
std::shared_ptr<void> GLVolume3DTex::GetData() {
  if(m_pTexture) { return m_pTexture->GetData(); }
  return std::shared_ptr<void>();
}

uint64_t GLVolume3DTex::GetCPUSize() const {
  return (m_pTexture) ? m_pTexture->GetCPUSize() : 0;
}

uint64_t GLVolume3DTex::GetGPUSize() const {
  return (m_pTexture) ? m_pTexture->GetGPUSize() : 0;
}

void GLVolume3DTex::SetFilter(GLint iMagFilter, GLint iMinFilter) {
  GLVolume::SetFilter(iMagFilter, iMinFilter);

  if (m_pTexture) m_pTexture->SetFilter(m_iMagFilter, m_iMinFilter);
}

GLuint GLVolume3DTex::GetGLID(){
	return m_pTexture->GetGLID();
}
