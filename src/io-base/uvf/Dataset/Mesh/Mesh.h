#pragma once

#include <vector>
#include <string>
#include "Ray.h"

class KDTree;

typedef std::vector<Core::Math::Vec3f> VertVec;
typedef std::vector<Core::Math::Vec3f> NormVec;
typedef std::vector<Core::Math::Vec2f> TexCoordVec;
typedef std::vector<Core::Math::Vec4f> ColorVec;
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
    const std::string& desc, EMeshType meshType,
    const Core::Math::Vec4f& defColor = Core::Math::Vec4f(1, 1, 1, 1));
  Mesh(const BasicMeshData& bmd,
       bool bBuildKDTree, bool bScaleToUnitCube, 
       const std::string& desc, EMeshType meshType,
       const Core::Math::Vec4f& defColor = Core::Math::Vec4f(1, 1, 1, 1));
  virtual ~Mesh();

  void Clone(const Mesh* other);
  void RecomputeNormals();
  void ScaleToUnitCube();
  void ComputeUnitCubeScale(Core::Math::Vec3f& scale,
                            Core::Math::Vec3f& translation);
  void Transform(const Core::Math::Mat4f& m);
  void ScaleAndBias(const Core::Math::Vec3f& scale,
                    const Core::Math::Vec3f& translation);

  double Pick(const Ray& ray, Core::Math::Vec3f& normal, 
              Core::Math::Vec2f& tc, Core::Math::Vec4f& color) const {
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

  const Core::Math::Vec4f& GetDefaultColor() const {return m_DefColor;}
  virtual void SetDefaultColor(const Core::Math::Vec4f& color) {m_DefColor = color;}
  bool UseDefaultColor() const {return m_Data.m_COLIndices.empty();}

  const std::string& Name() const {return m_MeshDesc;}

  const Core::Math::Mat4f& GetTransformFromOriginal() const {
    return m_TransformFromOriginal;
  }
  void DeleteTransformFromOriginal() {m_TransformFromOriginal = Core::Math::Mat4f();}

  const Core::Math::Vec3f& GetMin() const {return m_Bounds[0];}
  const Core::Math::Vec3f& GetMax() const {return m_Bounds[1];}

  // currently these two calls are somewhat redundant, but in the future we 
  // may want to support curved surfaces, in that case the first call would
  // be used during rendering while the second would be useful for list 
  // traversal
  EMeshType GetMeshType() const {return m_meshType;}
  size_t GetVerticesPerPoly() const {return m_VerticesPerPoly;}

protected:
  KDTree*       m_KDTree;

  BasicMeshData m_Data;

  Core::Math::Vec4f  m_DefColor;

  std::string   m_MeshDesc;
  EMeshType     m_meshType;
  size_t        m_VerticesPerPoly;

  Core::Math::Mat4f  m_TransformFromOriginal;

  void ComputeAABB();
  virtual void GeometryHasChanged(bool bUpdateAABB, bool bUpdateKDtree);

private:
  // picking
  double IntersectInternal(const Ray& ray, Core::Math::Vec3f& normal, 
                           Core::Math::Vec2f& tc, Core::Math::Vec4f& color,
                           double tmin, double tmax) const;
  double IntersectTriangle(size_t i, 
                           const Ray& ray, Core::Math::Vec3f& normal, 
                           Core::Math::Vec2f& tc, Core::Math::Vec4f& color) const;

  // AABB Test
  bool AABBIntersect(const Ray& r, double& tmin, double& tmax) const;
  Core::Math::Vec3f  m_Bounds[2];

  friend class KDTree;
};