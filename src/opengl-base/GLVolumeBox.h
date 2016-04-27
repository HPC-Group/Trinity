#pragma once

#include <array>

#include "GLModel.h"

class GLVolumeBox: public GLModel{
public:
  GLVolumeBox(const Core::Math::Vec3f& min, const Core::Math::Vec3f& max);
  void resize(const Core::Math::Vec3f& min, const Core::Math::Vec3f& max);

private:
  std::array<float,72>		m_Cube;

  void fillCube(const Core::Math::Vec3f& min, const Core::Math::Vec3f& max);

};

