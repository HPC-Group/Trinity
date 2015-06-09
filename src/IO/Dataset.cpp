/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2008 Scientific Computing and Imaging Institute,
   University of Utah.


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
/**
  \file    Dataset.cpp
  \author  Tom Fogal
           SCI Institute
           University of Utah
*/
#include "Dataset.h"
#include <Core/Math/MathTools.h>
#include <Core/Other/Mesh.h>

using namespace Core::Math;

namespace Tuvok {

Dataset::Dataset():
  m_UserScale(1.0,1.0,1.0),
  m_DomainScale(1.0, 1.0, 1.0)
{
  m_DomainScale = Vec3d(1.0, 1.0, 1.0);
}

Dataset::~Dataset()
{
  DeleteMeshes();
}

void Dataset::DeleteMeshes() {
  m_vpMeshList.clear();
}

void Dataset::SetRescaleFactors(const Vec3d& rescale) {
  m_UserScale = rescale;
}

Vec3d Dataset::GetRescaleFactors() const {
  return m_UserScale;
}

std::pair<Vec3f, Vec3f>
Dataset::GetTextCoords(BrickTable::const_iterator brick,
                       bool bUseOnlyPowerOfTwo) const {
  Vec3f vTexcoordsMin;
  Vec3f vTexcoordsMax;

  const Vec3ui vOverlap = GetBrickOverlapSize();
  const bool first_x = BrickIsFirstInDimension(0, brick->first);
  const bool first_y = BrickIsFirstInDimension(1, brick->first);
  const bool first_z = BrickIsFirstInDimension(2, brick->first);
  const bool last_x = BrickIsLastInDimension(0, brick->first);
  const bool last_y = BrickIsLastInDimension(1, brick->first);
  const bool last_z = BrickIsLastInDimension(2, brick->first);

  if (bUseOnlyPowerOfTwo) {
    Vec3ui vRealVoxelCount(MathTools::NextPow2(brick->second.n_voxels.x),
                                MathTools::NextPow2(brick->second.n_voxels.y),
                                MathTools::NextPow2(brick->second.n_voxels.z)
                                );
    vTexcoordsMin = Vec3f(
      (first_x) ? 0.5f/vRealVoxelCount.x : vOverlap.x*0.5f/vRealVoxelCount.x,
      (first_y) ? 0.5f/vRealVoxelCount.y : vOverlap.y*0.5f/vRealVoxelCount.y,
      (first_z) ? 0.5f/vRealVoxelCount.z : vOverlap.z*0.5f/vRealVoxelCount.z
      );
    vTexcoordsMax = Vec3f(
      (last_x) ? 1.0f-0.5f/vRealVoxelCount.x : 1.0f-vOverlap.x*0.5f/vRealVoxelCount.x,
      (last_y) ? 1.0f-0.5f/vRealVoxelCount.y : 1.0f-vOverlap.y*0.5f/vRealVoxelCount.y,
      (last_z) ? 1.0f-0.5f/vRealVoxelCount.z : 1.0f-vOverlap.z*0.5f/vRealVoxelCount.z
      );

    vTexcoordsMax -= Vec3f(vRealVoxelCount - brick->second.n_voxels) /
                                    Vec3f(vRealVoxelCount);
  } else {
    vTexcoordsMin = Vec3f(
      (first_x) ? 0.5f/brick->second.n_voxels.x : vOverlap.x*0.5f/brick->second.n_voxels.x,
      (first_y) ? 0.5f/brick->second.n_voxels.y : vOverlap.y*0.5f/brick->second.n_voxels.y,
      (first_z) ? 0.5f/brick->second.n_voxels.z : vOverlap.z*0.5f/brick->second.n_voxels.z
      );
    // for padded volume adjust texcoords
    vTexcoordsMax = Vec3f(
      (last_x) ? 1.0f-0.5f/brick->second.n_voxels.x : 1.0f-vOverlap.x*0.5f/brick->second.n_voxels.x,
      (last_y) ? 1.0f-0.5f/brick->second.n_voxels.y : 1.0f-vOverlap.y*0.5f/brick->second.n_voxels.y,
      (last_z) ? 1.0f-0.5f/brick->second.n_voxels.z : 1.0f-vOverlap.z*0.5f/brick->second.n_voxels.z
      );
  }

  return std::make_pair(vTexcoordsMin, vTexcoordsMax);
}

} // Tuvok namespace.
