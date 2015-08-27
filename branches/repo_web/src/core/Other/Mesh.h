/*
   For more information, please see: http://software.sci.utah.edu

   The MIT License

   Copyright (c) 2010 Interactive Visualization and Data Analysis Group.

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

//!    File   : Mesh.h
//!    Author : Jens Krueger
//!             IVCI & DFKI & MMCI, Saarbruecken
//!             SCI Institute, University of Utah
//!    Date   : July 2010
//
//!    Copyright (C) 2010 DFKI, MMCI, SCI Institute

#pragma once

#ifndef BASICS_MESH_H
#define BASICS_MESH_H

#include <vector>
#include <string>
#include "StdDefines.h"
#include <core/Math/Vectors.h>
#include "Ray.h"

using Core::Math::Vec2f;
using Core::Math::Vec3f;
using Core::Math::Vec4f;
using Core::Math::Mat4f;

namespace Tuvok {

class KDTree;

typedef std::vector<Vec3f> VertVec;
typedef std::vector<Vec3f> NormVec;
typedef std::vector<Vec2f> TexCoordVec;
typedef std::vector<Vec4f> ColorVec;
typedef std::vector<uint32_t> IndexVec;

#define noIntersection (std::numeric_limits<double>::max())

class BasicMeshData {
public:
  BasicMeshData() {}
  BasicMeshData(const VertVec& vertices, const NormVec& normals,
    const TexCoordVec& texcoords, const ColorVec& colors,
    const IndexVec& vIndices, const IndexVec& nIndices,
    const IndexVec& tIndices, const IndexVec& cIndices) :
    m_vertices(vertices),
    m_normals(normals),
    m_texcoords(texcoords),
    m_colors(colors),
    m_VertIndices(vIndices),
    m_NormalIndices(nIndices),
    m_TCIndices(tIndices),
    m_COLIndices(cIndices) {}

  VertVec       m_vertices;
  NormVec       m_normals;
  TexCoordVec   m_texcoords;
  ColorVec      m_colors;

  IndexVec      m_VertIndices;
  IndexVec      m_NormalIndices;
  IndexVec      m_TCIndices;
  IndexVec      m_COLIndices;

  void RemoveUnusedVertices();

protected:
  template <typename T>
  void RemoveUnusedEntries(IndexVec& indices, std::vector<T>& entries) const {
    // compute an inverse lookup index, i.e. for each
    // vertex, store what indices point to it
    std::vector<size_t> inverseIndex(entries.size(),0);
    for (size_t i = 0;i<indices.size();++i) {
      inverseIndex[indices[i]]++;
    }

    for (int64_t i = int64_t(inverseIndex.size()-1);i>=0;--i) {
      // unused vertex found, i.e. vertex that has no indices pointing to it
      if (inverseIndex[size_t(i)] == 0) {
        // shift all indices
        for (size_t j = 0;j<indices.size();++j) {
          if (indices[j] > size_t(i) ) indices[j]--;
        }
        // delete vertex
        entries.erase(entries.begin()+size_t(i));
      }
    }
  }

};

class Mesh
{
public:
  enum EMeshType {
    MT_TRIANGLES = 0,
    MT_LINES,
    MT_COUNT
  };

  Mesh();
  Mesh(const VertVec& vertices, const NormVec& normals,
    const TexCoordVec& texcoords, const ColorVec& colors,
    const IndexVec& vIndices, const IndexVec& nIndices,
    const IndexVec& tIndices, const IndexVec& cIndices,
    bool bBuildKDTree, bool bScaleToUnitCube,
    const std::string& desc, EMeshType meshType);
  Mesh(const BasicMeshData& bmd,
       bool bBuildKDTree, bool bScaleToUnitCube,
       const std::string& desc, EMeshType meshType);
  virtual ~Mesh();

  void Clone(const Mesh* other);
  void RecomputeNormals();
  void ScaleToUnitCube();
  void ComputeUnitCubeScale(Vec3f& scale,
                            Vec3f& translation);
  void Transform(const Mat4f& m);
  void ScaleAndBias(const Vec3f& scale,
                    const Vec3f& translation);

  double Pick(const Ray& ray, Vec3f& normal,
              Vec2f& tc, Vec4f& color) const {
    double tmin=0, tmax=0;
    if (!AABBIntersect(ray, tmin, tmax))
      return std::numeric_limits<double>::max();
    else
      return IntersectInternal(ray, normal, tc, color, tmin, tmax);
  }
  void ComputeKDTree();
  const KDTree* GetKDTree() const;

  // reorders normals, textures, etc. such that the index lists
  // all contain the same entries (i.e. reduce the mesh to a
  // single index list), returns true iff the unification was
  // possible
  bool UnifyIndices();

  // computes a vector of meshes whereas all of those meshes
  // have index vectors smaller then iMaxIndexCount
  std::vector<Mesh*> PartitionMesh(size_t iMaxIndexCount, bool bOptimize) const;

  bool HasUniformIndices() const;

  bool Validate(bool bDeepValidation=false);

  const BasicMeshData& GetBasicMeshData() const {return m_Data;}

  const VertVec&       GetVertices() const {return m_Data.m_vertices;}
  const NormVec&       GetNormals() const {return m_Data.m_normals;}
  const TexCoordVec&   GetTexCoords() const {return m_Data.m_texcoords;}
  const ColorVec&      GetColors() const {return m_Data.m_colors;}

  const IndexVec& GetVertexIndices() const {return m_Data.m_VertIndices;}
  const IndexVec& GetNormalIndices() const {return m_Data.m_NormalIndices;}
  const IndexVec& GetTexCoordIndices() const {return m_Data.m_TCIndices;}
  const IndexVec& GetColorIndices() const {return m_Data.m_COLIndices;}

  const Vec4f& GetDefaultColor() const {return m_DefColor;}
  virtual void SetDefaultColor(const Vec4f& color) {m_DefColor = color;}
  bool UseDefaultColor() const {return m_Data.m_COLIndices.empty();}

  const std::string& Name() const {return m_MeshDesc;}

  const Mat4f& GetTransformFromOriginal() const {
    return m_TransformFromOriginal;
  }
  void DeleteTransformFromOriginal() {m_TransformFromOriginal = Mat4f();}

  const Vec3f& GetMin() const {return m_Bounds[0];}
  const Vec3f& GetMax() const {return m_Bounds[1];}

  // currently these two calls are somewhat redundant, but in the future we
  // may want to support curved surfaces, in that case the first call would
  // be used during rendering while the second would be useful for list
  // traversal
  EMeshType GetMeshType() const {return m_meshType;}
  size_t GetVerticesPerPoly() const {return m_VerticesPerPoly;}

protected:
  KDTree*       m_KDTree;

  BasicMeshData m_Data;

  Vec4f  m_DefColor;

  std::string   m_MeshDesc;
  EMeshType     m_meshType;
  size_t        m_VerticesPerPoly;

  Mat4f  m_TransformFromOriginal;

  void ComputeAABB();
  virtual void GeometryHasChanged(bool bUpdateAABB, bool bUpdateKDtree);

private:
  // picking
  double IntersectInternal(const Ray& ray, Vec3f& normal,
                           Vec2f& tc, Vec4f& color,
                           double tmin, double tmax) const;
  double IntersectTriangle(size_t i,
                           const Ray& ray, Vec3f& normal,
                           Vec2f& tc, Vec4f& color) const;

  // AABB Test
  bool AABBIntersect(const Ray& r, double& tmin, double& tmax) const;
  Vec3f  m_Bounds[2];

  friend class KDTree;
};
}
#endif // BASICS_MESH_H
