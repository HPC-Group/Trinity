#include "Dataset.h"
#include "silverbullet/math/MathTools.h"

using namespace Core::Math::MathTools;

Dataset::Dataset():
  m_UserScale(1.0,1.0,1.0),
  m_DomainScale(1.0, 1.0, 1.0)
{
  m_DomainScale = Core::Math::Vec3d(1.0, 1.0, 1.0);
}

Dataset::~Dataset()
{
  DeleteMeshes();
}

void Dataset::DeleteMeshes() {
  m_vpMeshList.clear();
}

void Dataset::SetRescaleFactors(const Core::Math::Vec3d& rescale) {
  m_UserScale = rescale;
}

Core::Math::Vec3d Dataset::GetRescaleFactors() const {
  return m_UserScale;
}

std::pair<Core::Math::Vec3f, Core::Math::Vec3f>
Dataset::GetTextCoords(BrickTable::const_iterator brick,
                       bool bUseOnlyPowerOfTwo) const {
  Core::Math::Vec3f vTexcoordsMin;
  Core::Math::Vec3f vTexcoordsMax;

  const Core::Math::Vec3ui vOverlap = GetBrickOverlapSize();
  const bool first_x = BrickIsFirstInDimension(0, brick->first);
  const bool first_y = BrickIsFirstInDimension(1, brick->first);
  const bool first_z = BrickIsFirstInDimension(2, brick->first);
  const bool last_x = BrickIsLastInDimension(0, brick->first);
  const bool last_y = BrickIsLastInDimension(1, brick->first);
  const bool last_z = BrickIsLastInDimension(2, brick->first);

  if (bUseOnlyPowerOfTwo) {
    Core::Math::Vec3ui vRealVoxelCount(nextPow2(brick->second.n_voxels.x),
                                nextPow2(brick->second.n_voxels.y),
                                nextPow2(brick->second.n_voxels.z)
                                );
    vTexcoordsMin = Core::Math::Vec3f(
      (first_x) ? 0.5f/vRealVoxelCount.x : vOverlap.x*0.5f/vRealVoxelCount.x,
      (first_y) ? 0.5f/vRealVoxelCount.y : vOverlap.y*0.5f/vRealVoxelCount.y,
      (first_z) ? 0.5f/vRealVoxelCount.z : vOverlap.z*0.5f/vRealVoxelCount.z
      );
    vTexcoordsMax = Core::Math::Vec3f(
      (last_x) ? 1.0f-0.5f/vRealVoxelCount.x : 1.0f-vOverlap.x*0.5f/vRealVoxelCount.x,
      (last_y) ? 1.0f-0.5f/vRealVoxelCount.y : 1.0f-vOverlap.y*0.5f/vRealVoxelCount.y,
      (last_z) ? 1.0f-0.5f/vRealVoxelCount.z : 1.0f-vOverlap.z*0.5f/vRealVoxelCount.z
      );

    vTexcoordsMax -= Core::Math::Vec3f(vRealVoxelCount - brick->second.n_voxels) /
                                    Core::Math::Vec3f(vRealVoxelCount);
  } else {
    vTexcoordsMin = Core::Math::Vec3f(
      (first_x) ? 0.5f/brick->second.n_voxels.x : vOverlap.x*0.5f/brick->second.n_voxels.x,
      (first_y) ? 0.5f/brick->second.n_voxels.y : vOverlap.y*0.5f/brick->second.n_voxels.y,
      (first_z) ? 0.5f/brick->second.n_voxels.z : vOverlap.z*0.5f/brick->second.n_voxels.z
      );
    // for padded volume adjust texcoords
    vTexcoordsMax = Core::Math::Vec3f(
      (last_x) ? 1.0f-0.5f/brick->second.n_voxels.x : 1.0f-vOverlap.x*0.5f/brick->second.n_voxels.x,
      (last_y) ? 1.0f-0.5f/brick->second.n_voxels.y : 1.0f-vOverlap.y*0.5f/brick->second.n_voxels.y,
      (last_z) ? 1.0f-0.5f/brick->second.n_voxels.z : 1.0f-vOverlap.z*0.5f/brick->second.n_voxels.z
      );
  }

  return std::make_pair(vTexcoordsMin, vTexcoordsMax);
}
