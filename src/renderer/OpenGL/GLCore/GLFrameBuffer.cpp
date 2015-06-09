//
//  GLFrameBuffer.cpp
//  TNG
//
//  Created by Andre Waschk on 09/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//


#include "GLFrameBuffer.h"

#include <iostream>

using namespace Tuvok::Renderer::OpenGL::GLCore;
using namespace Core::Math;


GLFrameBuffer::GLFrameBuffer():
m_hFBOhandle(GL_INVALID_VALUE),
m_hDepthhandle(GL_INVALID_VALUE){}
GLFrameBuffer::~GLFrameBuffer(void){}

void GLFrameBuffer::Initialize(Vec2ui resolution, uint32_t format, int buffercount){
    
    // store information about the framebuffer
    m_iFrameBufferCount = buffercount;
    m_vFrameBufferResolution = resolution;

    // generate a framebuffer and depth buffer on the  gpu
    glGenFramebuffers(1, &m_hFBOhandle);
    glGenRenderbuffers(1, &m_hDepthhandle);

    // bind the created depth stencil
    glBindFramebuffer(GL_FRAMEBUFFER, m_hDepthhandle);

    //create a description for the multiple render targets
    m_DrawBuffers = new GLenum[m_iFrameBufferCount];

    //create the amount of needed render targets and attach them to the framebuffer
    for (int i = 0; i < m_iFrameBufferCount; ++i){
        m_DrawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
        uint32_t current;
        
        // create the render target texture
        glGenTextures(1, &current);
        glBindTexture(GL_TEXTURE_2D, current);
      
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, m_vFrameBufferResolution.x, m_vFrameBufferResolution.y, 0, GL_RGBA, GL_FLOAT, NULL);
        
        //attach the texture to the framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, current, 0);
        //store the handle to the texture
        m_vFrameBufferTextureHandles.push_back(current);
    }
    
    glBindRenderbuffer(GL_RENDERBUFFER, m_hDepthhandle);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_vFrameBufferResolution.x, m_vFrameBufferResolution.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_hDepthhandle);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout << "error while creating framebuffer" << std::endl;
    }
    
    //unbind
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GLFrameBuffer::SetViewport(){
    glViewport(0, 0, m_vFrameBufferResolution.x, m_vFrameBufferResolution.y);
}

void GLFrameBuffer::Write(){
    glBindFramebuffer(GL_FRAMEBUFFER, m_hFBOhandle);
    
    glDrawBuffers(m_iFrameBufferCount, m_DrawBuffers);
}


void GLFrameBuffer::ClearFramebuffer(Vec4f clearColor){
    glClearColor(clearColor.x, clearColor.y, clearColor.z, clearColor.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearDepth(GL_COLOR_BUFFER_BIT);
}

const GLuint& GLFrameBuffer::GetFramebufferTexture(const int i){
    return m_vFrameBufferTextureHandles[i];
}