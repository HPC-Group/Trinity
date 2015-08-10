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


//!    File   : MC.cpp
//!    Author : Jens Krueger
//!             SCI Institute
//!             University of Utah
//!    Date   : January 2009
//
//!    Copyright (C) 2008 SCI Institute


#include <cstring>
#include <sstream>
#include <iomanip>
#include "MC.h"


Isosurface::Isosurface() :
  vfVertices(NULL),
  vfNormals(NULL),
  viTriangles(NULL),
  iVertices(0),
  iTriangles(0)
{}

Isosurface::Isosurface(int iMaxVertices, int iMaxTris) :
  vfVertices(new Vec3f[iMaxVertices]),
  vfNormals(new Vec3f[iMaxVertices]),
  viTriangles(new Vec3i[iMaxTris]),
  iVertices(0),
  iTriangles(0)
{}

Isosurface::~Isosurface() {
  delete [] vfVertices;
  delete [] vfNormals;
  delete [] viTriangles;
}

int Isosurface::AddTriangle(int a, int b, int c) {
  viTriangles[iTriangles++] = Vec3i(a,b,c);
  return iTriangles-1;
}

int Isosurface::AddVertex(Vec3f v, Vec3f n) {
  vfVertices[iVertices] = v;
  vfNormals[iVertices++] = n;
  return iVertices-1;
}

void Isosurface::AppendData(const Isosurface* other) {
  // if verts in other, expand the storage this surface
  if (other->iVertices > 0) {

    // create new mem
    Vec3f*  temp_Vertices  = new Vec3f[iVertices  + other->iVertices];
    Vec3f*  temp_Normals   = new Vec3f[iVertices  + other->iVertices];
    Vec3i*  temp_Triangles = new Vec3i[iTriangles + other->iTriangles];

    // copy "old" data
    memcpy(temp_Vertices, vfVertices, sizeof(Vec3f)*iVertices);
    memcpy(temp_Normals, vfNormals, sizeof(Vec3f)*iVertices);
    memcpy(temp_Triangles, viTriangles, sizeof(Vec3i)*iTriangles);

    // append new data
    memcpy(temp_Vertices+iVertices, other->vfVertices, sizeof(Vec3f)*other->iVertices);
    memcpy(temp_Normals+iVertices, other->vfNormals, sizeof(Vec3f)*other->iVertices);
    memcpy(temp_Triangles+iTriangles, other->viTriangles, sizeof(Vec3i)*other->iTriangles);

    // delete "old" data
    delete [] vfVertices;
    delete [] vfNormals;
    delete [] viTriangles;

    // rename
    vfVertices  = temp_Vertices;
    vfNormals   = temp_Normals;
    viTriangles = temp_Triangles;
  }

  // update this list's counters
  iVertices  += other->iVertices;
  iTriangles += other->iTriangles;
}


void Isosurface::Transform(const Mat4f& matrix) {

  Mat4f itMatrix = matrix.inverse();
  itMatrix = itMatrix.Transpose();

  for (int i = 0;i<iVertices;i++) {
    Vec4f  fVertex(vfVertices[i],1);
    Vec4f  fNormal(vfNormals[i],0);

    fVertex  = fVertex * matrix;
    fNormal  = fNormal * itMatrix;

    vfVertices[i] = fVertex.xyz() / fVertex.w;

    vfNormals[i] = fNormal.xyz();
    vfNormals[i].normalize();
  }
}
