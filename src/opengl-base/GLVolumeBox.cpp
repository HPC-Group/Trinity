#include "GLVolumeBox.h"

using namespace Core::Math;


static int CubeIndicies[36]{
  0, 1, 2,
  2, 3, 0,

  4, 7, 6,
  6, 5, 4,

  8, 9, 10,
  10, 11, 8,

  12, 15, 14,
  14, 13, 12,

  16, 17, 18,
  18, 19, 16,

  20, 23, 22,
  22, 21, 20
};


GLVolumeBox::GLVolumeBox(const Core::Math::Vec3f& min,
                         const Core::Math::Vec3f& max){
  fillCube(min, max);
  Initialize(m_Cube.data(), CubeIndicies, 24, 36);
}

void GLVolumeBox::resize(const Core::Math::Vec3f& min,
                         const Core::Math::Vec3f& max){
  fillCube(min, max);
  updateVertexData(m_Cube.data(), 24);
}

void GLVolumeBox::fillCube(const Core::Math::Vec3f& min,
                           const Core::Math::Vec3f& max){

  size_t i = 0;
  //top
  m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
  m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;

  //bottom
  m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
  m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;

  //front
  m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
  m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;

  //back
  m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
  m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
  m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
  m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;

  //left
  m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
  m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
  m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
  m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;

  //right
  m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
  m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
  m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
  m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
}


