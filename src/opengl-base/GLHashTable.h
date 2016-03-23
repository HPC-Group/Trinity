#pragma once

#include "mocca/log/LogManager.h"
#include <opengl-base/OpenGLincludes.h>

#include <opengl-base/OpenGLError.h>

#include <silverbullet/math/Vectors.h>
#include <memory>
#include <string>

// adds glFinish() calls all over the place, to allow for timing
// but should be excluded in a "normal" build due to the performance
// hit
//#define GLHASHTABLE_PROFILE

class GLTexture;
typedef std::shared_ptr<GLTexture> GLTexturePtr;

class GLHashTable {
public:
  GLHashTable(const Core::Math::Vec3ui& maxBrickCount, uint32_t iTableSize=509,
              uint32_t iRehashCount=10, bool bUseGLCore=true,
              const std::string&  strPrefixName = "");
  virtual ~GLHashTable();
  
  void initGL(); // might throw
  void freeGL();
  
  std::string getShaderFragment(uint32_t iMountPoint=0);
  void enable();
  std::vector<Core::Math::Vec4ui> getData();
  std::string const& getPrefixName() const { return m_strPrefixName; }
  void clearData();
  
  virtual uint64_t getCPUSize() const;
  virtual uint64_t getGPUSize() const;
  
private:
  std::string               m_strPrefixName;
  Core::Math::Vec3ui        m_maxBrickCount;
  uint32_t                  m_iTableSize;
  uint32_t                  m_iRehashCount;
  GLTexturePtr              m_pHashTableTex;
  Core::Math::Vec2ui        m_texSize;
  std::shared_ptr<uint32_t> m_pRawData;
  bool                      m_bUseGLCore;
  uint32_t                  m_iMountPoint;
  
  Core::Math::Vec4ui int2Vector(uint32_t index) const;
  bool is2DTexture() const;
  //moved from VolumeTools
  Core::Math::Vec2ui fit1DIndexTo2DArray(uint64_t iMax1DIndex, uint32_t
                                         iMax2DArraySize);
};
