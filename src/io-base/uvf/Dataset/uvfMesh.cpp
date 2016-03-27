#include <cassert>
#include <cstring>
#include <stdexcept>
#include "uvfMesh.h"
#include "UVF-File/GeometryDataBlock.h"

using namespace std;

uvfMesh::uvfMesh(const GeometryDataBlock& tsb)
{
  m_DefColor = Core::Math::Vec4f(tsb.GetDefaultColor());
  m_MeshDesc = tsb.m_Desc;

  switch (tsb.GetPolySize()) {
    case 2 :  m_meshType = MT_LINES; break;
    case 3 :  m_meshType = MT_TRIANGLES; break;
    default : throw std::runtime_error("reading unsupported mesh type");
  }

  m_VerticesPerPoly = size_t(tsb.GetPolySize());

  vector<float> fVec;
  fVec = tsb.GetVertices();
  assert(fVec.size()%3 == 0);
  if (fVec.size() > 0) {
    m_Data.m_vertices.resize(fVec.size()/3);
    for (size_t i = 0;i<fVec.size();i+=3) {
      m_Data.m_vertices[i] = Core::Math::Vec3f(fVec[i*3+0], fVec[i*3+1], fVec[i*3+2]);
    }
  }
  fVec = tsb.GetNormals();
  assert(fVec.size()%3 == 0);
  if (fVec.size() > 0) {
    m_Data.m_normals.resize(fVec.size()/3);
    for (size_t i = 0;i<fVec.size();i+=3) {
      m_Data.m_normals[i] = Core::Math::Vec3f(fVec[i*3+0], fVec[i*3+1], fVec[i*3+2]);
    }
  }
  fVec = tsb.GetTexCoords();
  assert(fVec.size()%2 == 0);
  if (fVec.size() > 0) {
    m_Data.m_texcoords.resize(fVec.size()/2);
    for (size_t i = 0;i<fVec.size();i+=2) {
      m_Data.m_texcoords[i] = Core::Math::Vec2f(fVec[i*2+0], fVec[i*2+1]);
    }
  }
  fVec = tsb.GetColors();
  assert(fVec.size()%4 == 0);
  if (fVec.size() > 0) {
    m_Data.m_colors.resize(fVec.size()/4);
    for (size_t i = 0;i<fVec.size();i+=4) {
      m_Data.m_colors[i] = Core::Math::Vec4f(fVec[i*4+0], fVec[i*4+1], fVec[i*4+2], fVec[i*4+3]);
    }
  }
  fVec.clear();

  m_Data.m_VertIndices = tsb.GetVertexIndices();
  assert(m_Data.m_VertIndices.size()%tsb.GetPolySize() == 0);
  m_Data.m_NormalIndices = tsb.GetNormalIndices();
  assert(m_Data.m_NormalIndices.size()%tsb.GetPolySize() == 0);
  m_Data.m_TCIndices = tsb.GetTexCoordIndices();
  assert(m_Data.m_TCIndices.size()%tsb.GetPolySize() == 0);
  m_Data.m_COLIndices = tsb.GetColorIndices();
  assert(m_Data.m_COLIndices.size()%tsb.GetPolySize() == 0);

  GeometryHasChanged(true, true);
}
