//
//  GLModel.cpp
//  TNG
//
//  Created by Andre Waschk on 13/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "GLModel.h"

using namespace Tuvok::Renderer::OpenGL::GLCore;

GLModel::GLModel():
m_bInitialized(false){
}
GLModel::~GLModel(){}



void GLModel::Initialize(float vertices[], int indices[], int m_vertexCount, int m_indexCount){
  
  m_gpuSize = m_vertexCount * sizeof(float)* 3 + m_indexCount* sizeof(unsigned int);

  m_ElementCount = m_indexCount;
  
  glGenVertexArrays(1, &m_VertexArrayObjectHandle);
  glBindVertexArray(m_VertexArrayObjectHandle);
  
  
  glGenBuffers(1, &m_VertexBufferObjectHandle);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectHandle);
  glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float)* 3, vertices, GL_STATIC_DRAW);
  
  // Enable vertex array attributes.
  glEnableVertexAttribArray(0);
  
  // Specify the location and format
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectHandle);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float)* 3, 0);
  
  
  glGenBuffers(1, &m_VertexIndexObjectHandle);
  
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndexObjectHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);
  
  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    printf("++Unknown error (%x) occurred while creating an vbo.\n",
           static_cast<unsigned int>(err));
  }
  
  m_bInitialized = true;
}

void GLModel::updateVertexData(float vertices[], int m_vertexCount){
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectHandle);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float)* 3, vertices, GL_STATIC_DRAW);
}

void GLModel::paint(GLenum mode){
  if(m_bInitialized){
    glBindVertexArray(m_VertexArrayObjectHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndexObjectHandle);

    glDrawElements(mode, m_ElementCount, GL_UNSIGNED_INT, 0);
  }
}

uint64_t GLModel::GetGPUSize() const{
	return m_gpuSize;
}