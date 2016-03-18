#pragma once

#ifndef GLHASHTABLE_H
#define GLHASHTABLE_H

#include "mocca/log/LogManager.h"
#include <opengl-base/OpenGLincludes.h>

#include <opengl-base/OpenGLError.h>

#include <silverbullet/math/Vectors.h>
#include <memory>
#include <string>

//#define GLHASHTABLE_PROFILE    // adds some glFinish() commands all over the place

namespace OpenGL{

      namespace GLCore{
        class GLTexture;
        typedef std::shared_ptr<GLTexture> GLTexturePtr;
      }


  class GLHashTable {
  public:
    GLHashTable(const Core::Math::Vec3ui& maxBrickCount, uint32_t iTableSize=509, uint32_t iRehashCount=10, bool bUseGLCore=true, std::string const& strPrefixName = "");
    virtual ~GLHashTable();

    void InitGL(); // might throw
    void FreeGL();

    std::string GetShaderFragment(uint32_t iMountPoint=0);
    void Enable();
    std::vector<Core::Math::Vec4ui> GetData();
    std::string const& GetPrefixName() const { return m_strPrefixName; }
    void ClearData();

    virtual uint64_t GetCPUSize() const;
    virtual uint64_t GetGPUSize() const;
  private:
    std::string               m_strPrefixName;
    Core::Math::Vec3ui        m_maxBrickCount;
    uint32_t                  m_iTableSize;
    uint32_t                  m_iRehashCount;
    GLCore::GLTexturePtr      m_pHashTableTex;
    Core::Math::Vec2ui        m_texSize;
    std::shared_ptr<uint32_t> m_pRawData;
    bool                      m_bUseGLCore;
    uint32_t                  m_iMountPoint;

    Core::Math::Vec4ui Int2Vector(uint32_t index) const;
    bool Is2DTexture() const;
  };

}


#endif // GLHASHTABLE_H

/*
 The MIT License

 Copyright (c) 2012 Interactive Visualization and Data Analysis Group,
 Saarland University


 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */
