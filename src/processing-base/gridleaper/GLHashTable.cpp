
#include "GLHashTable.h"
#define GLHASHTABLE_PROFILE

//#define WRITE_SHADERS 1

#ifdef WRITE_SHADERS
# include <fstream>
# include <iterator>
#endif
#include <sstream>
#include <stdexcept>


#include <opengl-base/GLProgram.h>
#include <opengl-base/GLTexture1D.h>
#include <opengl-base/GLTexture2D.h>
#include <opengl-base/ShaderDescriptor.h>

using namespace Core::Math;

GLHashTable::GLHashTable(const Vec3ui& maxBrickCount, uint32_t iTableSize, uint32_t iRehashCount, bool bUseGLCore, std::string const& strPrefixName) :
m_strPrefixName(strPrefixName),
m_maxBrickCount(maxBrickCount),
m_iTableSize(iTableSize),
m_iRehashCount(iRehashCount),
m_pHashTableTex(nullptr),
m_bUseGLCore(bUseGLCore),
m_iMountPoint(0)
{

}

GLHashTable::~GLHashTable() {
  freeGL();
  LINFO("shutdown ~GLHashTable");
}

void GLHashTable::initGL() {

  int gpumax;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &gpumax);


  try {
    m_texSize = fit1DIndexTo2DArray(m_iTableSize, gpumax);
    LDEBUGC("GLHashTable", "Hashtable texture size "<< m_texSize);
  } catch (std::runtime_error const&) {
    // this is very unlikely but not impossible
    //T_ERROR(e.what());
    throw;
  }

  // try to use 1D texture if possible because it appears to be slightly faster than a 2D texture
  if (is2DTexture()) {
    m_pHashTableTex = std::make_shared<GLTexture2D>(m_texSize.x, m_texSize.y, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
    LDEBUGC("GLHashTable", "using a 2D hashtable : textureID" << m_pHashTableTex->GetGLID());
  } else {
    assert(m_texSize.x == m_iTableSize);
    m_pHashTableTex = std::make_shared<GLTexture1D>(m_texSize.x, GL_R32UI, GL_RED_INTEGER, GL_UNSIGNED_INT);
    LDEBUGC("GLHashTable", "using a 1D hashtable : textureID" << m_pHashTableTex->GetGLID());
  }

  m_pRawData = std::shared_ptr<uint32_t>(
                                         new uint32_t[m_texSize.area()]
                                         );
}

Vec4ui GLHashTable::int2Vector(uint32_t index) const {
  Vec4ui coords;

  coords.w = index / m_maxBrickCount.volume();
  index -= coords.w*m_maxBrickCount.volume();
  coords.z = index / (m_maxBrickCount.x * m_maxBrickCount.y);
  index -= coords.z*(m_maxBrickCount.x * m_maxBrickCount.y);
  coords.y = index / m_maxBrickCount.x;
  index -= coords.y*m_maxBrickCount.x;
  coords.x = index;

  return coords;
}


bool GLHashTable::is2DTexture() const {
  return m_texSize.y > 1;
}


void GLHashTable::enable() {
#ifndef DETECTED_OS_APPLE
  glBindImageTexture(m_iMountPoint, m_pHashTableTex->GetGLID(), 0, false, 0, GL_READ_WRITE, GL_R32UI);
#endif
}


std::vector<UINTVECTOR4> GLHashTable::getData() {
#ifndef DETECTED_OS_APPLE
  glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
#endif
#ifdef GLHASHTABLE_PROFILE
  glFinish();
#endif
  //TimedStatement(PERF_READ_HTABLE,
  m_pRawData = std::static_pointer_cast<uint32_t>(m_pHashTableTex->GetData());
  //);
  std::vector<UINTVECTOR4> requests;
  //StackTimer condense(PERF_CONDENSE_HTABLE);
  for (size_t i = 0;i<m_iTableSize;++i) {
    uint32_t elem = m_pRawData.get()[i];
    if (elem != 0) requests.push_back(int2Vector(elem-1));
  }

  return requests;
}

void GLHashTable::clearData() {
  std::fill(m_pRawData.get(), m_pRawData.get()+m_iTableSize, 0);
  m_pHashTableTex->SetData(m_pRawData.get());
}

std::string GLHashTable::getShaderFragment(uint32_t iMountPoint) {
  m_iMountPoint = iMountPoint;
  std::stringstream ss;

#ifdef WRITE_SHADERS
  const char* shname = "hashtable.glsl";
  std::ifstream shader(shname);
  if(shader) {
    ////MESSAGE("Reusing hashtable.glsl shader on disk.");
    shader >> std::noskipws;
    std::string sh(
                   (std::istream_iterator<char>(shader)),
                   (std::istream_iterator<char>())
                   );
    shader.close();
    return sh;
  }
#endif

  if (m_bUseGLCore)
    ss << "#version 420 core\n";
  else
    ss << "#version 420 compatibility\n";

  ss << "\n"
  << "layout(binding = " << iMountPoint << ", size1x32) coherent uniform "
  << (is2DTexture() ? "uimage2D " : "uimage1D ") << m_strPrefixName << "hashTable;\n"
  << "\n"
  << "uint " << m_strPrefixName << "Serialize(uvec4 bd) {\n"
  << "  return 1 + bd.x + bd.y * " << m_maxBrickCount.x << " + bd.z * "
  <<           m_maxBrickCount.x * m_maxBrickCount.y << " + bd.w * "
  <<           m_maxBrickCount.volume() << ";\n"
  << "}\n"
  << "\n"
  << "uint " << m_strPrefixName << "HashValue(uint serializedValue) {\n"
  << "  return uint(serializedValue % " << m_iTableSize << ");\n"
  << "}\n"
  << "\n";

  if (is2DTexture()) {
    // we are using a 2D texture
    ss << "uint " << m_strPrefixName << "AccessHashTable(uint hashValue, uint serializedValue) {\n"
    << "  ivec2 hashPosition = ivec2(hashValue % " << m_texSize.x << ", "
    "hashValue / " << m_texSize.x << ");\n"
    << "  return imageAtomicCompSwap(" << m_strPrefixName << "hashTable,"
    "hashPosition, uint(0), serializedValue);\n"
    << "}\n"
    << "\n";
  } else {
    // we are using a 1D texture
    ss << "uint " << m_strPrefixName << "AccessHashTable(uint hashValue, uint serializedValue) {\n"
    << "  int hashPosition = int(hashValue);\n"
    << "  return imageAtomicCompSwap(" << m_strPrefixName << "hashTable,"
    "hashPosition, uint(0), serializedValue);\n"
    << "}\n"
    << "\n";
  }

  ss << "uint " << m_strPrefixName << "Hash(uvec4 bd) {\n"
  << "  uint rehashCount = 0;\n"
  << "  uint serializedValue =  " << m_strPrefixName << "Serialize(bd);\n"
  << "\n"
  << "  do {\n"
  << "    uint hash = " << m_strPrefixName << "HashValue(serializedValue+rehashCount);\n"
  << "    uint valueInImage = " << m_strPrefixName << "AccessHashTable(hash, serializedValue);\n"
  << "    if (valueInImage == 0 || valueInImage == serializedValue)\n"
  << "      return rehashCount;\n"
  << "  } while (++rehashCount < " << m_iRehashCount << ");\n"
  << "\n"
  << "  return uint(" << m_iRehashCount << ");\n"
  << "}\n";

#ifdef WRITE_SHADERS
  std::ofstream hashtab(shname, std::ios::trunc);
  if(hashtab) {
    printf("Writing new hashtable shader.");
    const std::string& s = ss.str();
    std::copy(s.begin(), s.end(), std::ostream_iterator<char>(hashtab, ""));
  }
  hashtab.close();
#endif // WRITE_SHADERS

  return ss.str();
}

void GLHashTable::freeGL() {
  if (m_pHashTableTex) {
    m_pHashTableTex->Delete();
    m_pHashTableTex = nullptr;
  }
}

uint64_t GLHashTable::getCPUSize() const {
  return m_pHashTableTex->GetCPUSize() + m_iTableSize*4;
}
uint64_t GLHashTable::getGPUSize() const {
  return m_pHashTableTex->GetGPUSize();
}

Vec2ui GLHashTable::fit1DIndexTo2DArray(uint64_t iMax1DIndex,
                                        uint32_t iMax2DArraySize) {
  // check if 1D index exceeds given 2D array
  if (iMax1DIndex > uint64_t(iMax2DArraySize) * uint64_t(iMax2DArraySize)) {
    std::stringstream ss;
    ss << "element count of " << iMax1DIndex << " exceeds the addressable indices "
    << "of a " << iMax2DArraySize << "x" << iMax2DArraySize << " array";
    throw std::runtime_error(ss.str().c_str());
  }

  // 1D index fits into a row
  if (iMax1DIndex <= uint64_t(iMax2DArraySize))
    return Vec2ui(uint32_t(iMax1DIndex), 1);
  
  // fit 1D index into the smallest possible rectangle
  Vec2ui v2DArraySize;
  v2DArraySize.x = uint32_t(std::ceil(std::sqrt(double(iMax1DIndex))));
  v2DArraySize.y = uint32_t(std::ceil(double(iMax1DIndex)/double(v2DArraySize.x)));
  
  return v2DArraySize;
}
