#pragma once

struct BrickElemInfo {
  BrickElemInfo(const Core::Math::Vec4ui& vBrickID,
                const Core::Math::Vec3ui& vVoxelSize) :
  m_vBrickID(vBrickID),
  m_vVoxelSize(vVoxelSize)
  {}

  Core::Math::Vec4ui    m_vBrickID;
  Core::Math::Vec3ui    m_vVoxelSize;
};