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

//!    File   : MobileGeoConverter.cpp
//!    Author : Georg Tamm
//!             DFKI, Saarbruecken
//!    Date   : July 2010
//
//!    Copyright (C) 2010 DFKI Institute

#include "MobileGeoConverter.h"

#include "G3D.h"
#include <Core/Other/Mesh.h>
#include "../TuvokIOError.h"

#include <Core/IO/FileTools.h>
using namespace Core::IO::FileTools;
using namespace Tuvok;
using namespace std;

MobileGeoConverter::MobileGeoConverter() :
  AbstrGeoConverter()
{
  m_vConverterDesc = "Mobile Geometry File";
  m_vSupportedExt.push_back("G3D");
  m_vSupportedExt.push_back("G3DX");
}

bool MobileGeoConverter::ConvertToNative(const Mesh& m,
                                         const std::string& strTargetFilename) {

  if (m.GetVertices().empty()) return false;
/*
  // include this once we have mesh downsampling implemented
  bool bSimplify = ToUpperCase(
                            GetExt(strTargetFilename)
                                            ) == "G3D";
*/
  G3D::GeometrySoA geometry;
  geometry.info.isOpaque = false;
  geometry.info.numberPrimitives =
    uint32_t(m.GetVertexIndices().size() / m.GetVerticesPerPoly());

  if (geometry.info.numberPrimitives == 0) {
    IVDA_ERROR("No primitives to export.");
    return false; // nothing to export
  }

  geometry.info.primitiveType = (m.GetMeshType() == Mesh::MT_TRIANGLES)
                                      ? G3D::Triangle : G3D::Line;
  geometry.info.numberIndices = uint32_t(m.GetVertexIndices().size());
  geometry.info.numberVertices = uint32_t(m.GetVertices().size());
  uint32_t vertexFloats = 0;

  geometry.info.attributeSemantics.push_back(G3D::Position);
  geometry.vertexAttributes.push_back((float*)&m.GetVertices().at(0));
  vertexFloats += G3D::floats(G3D::Position);

  if (m.GetNormals().size() == m.GetVertices().size())
  {
	  geometry.info.attributeSemantics.push_back(G3D::Normal);
	  geometry.vertexAttributes.push_back((float*)&m.GetNormals().at(0));
	  vertexFloats += G3D::floats(G3D::Normal);
  }
  if (m.GetTexCoords().size() == m.GetVertices().size())
  {
	  geometry.info.attributeSemantics.push_back(G3D::Tex);
	  geometry.vertexAttributes.push_back((float*)&m.GetTexCoords().at(0));
	  vertexFloats += G3D::floats(G3D::Tex);
  }
  geometry.info.attributeSemantics.push_back(G3D::Color);
  vertexFloats += G3D::floats(G3D::Color);
  ColorVec colors;
  if (m.GetColors().size() == m.GetVertices().size())
  {
	  geometry.vertexAttributes.push_back((float*)&m.GetColors().at(0));
  }
  else
  {
	  colors = ColorVec(m.GetVertices().size(), m.GetDefaultColor());
	  geometry.vertexAttributes.push_back((float*)&colors.at(0));
  }

  geometry.info.indexSize = sizeof(uint32_t);
  geometry.info.vertexSize = vertexFloats * sizeof(float);
  geometry.indices = (uint32_t*)&m.GetVertexIndices().at(0);
  G3D::write(strTargetFilename, &geometry);

  return true;
}

std::shared_ptr<Mesh>
MobileGeoConverter::ConvertToMesh(const std::string& strFilename) {
  VertVec       vertices;
  NormVec       normals;
  TexCoordVec   texcoords;
  ColorVec      colors;

  IndexVec      VertIndices;
  IndexVec      NormalIndices;
  IndexVec      TCIndices;
  IndexVec      COLIndices;

  G3D::GeometrySoA geometry;
  G3D::read(strFilename, &geometry);
  if (geometry.info.indexSize == sizeof(uint16_t))
  {
	  for (uint32_t i=0; i<geometry.info.numberIndices; ++i) VertIndices.push_back((uint32_t)((uint16_t*)geometry.indices)[i]);
  }
  else VertIndices = IndexVec(geometry.indices, (uint32_t*)geometry.indices + geometry.info.numberIndices);

  uint32_t i = 0;
  for (std::vector<uint32_t>::iterator it=geometry.info.attributeSemantics.begin(); it<geometry.info.attributeSemantics.end(); ++it)
  {
	  if (*it == G3D::Position) vertices = VertVec((Vec3f*)geometry.vertexAttributes.at(i), (Vec3f*)geometry.vertexAttributes.at(i) + geometry.info.numberVertices);
	  else if (*it == G3D::Normal) normals = NormVec((Vec3f*)geometry.vertexAttributes.at(i), (Vec3f*)geometry.vertexAttributes.at(i) + geometry.info.numberVertices);
      else if (*it == G3D::Color) colors = ColorVec((Vec4f*)geometry.vertexAttributes.at(i), (Vec4f*)geometry.vertexAttributes.at(i) + geometry.info.numberVertices);
      else if (*it == G3D::Tex) texcoords = TexCoordVec((Vec2f*)geometry.vertexAttributes.at(i), (Vec2f*)geometry.vertexAttributes.at(i) + geometry.info.numberVertices);
	  ++i;
  }
  G3D::clean(&geometry);

  std::string desc = m_vConverterDesc + " data converted from " + GetFilename(strFilename);
  return std::shared_ptr<Mesh>(
    new Mesh(vertices,normals,texcoords,colors,
             VertIndices,NormalIndices,TCIndices,COLIndices,
             false, false, desc, Mesh::MT_TRIANGLES)
  );
}
