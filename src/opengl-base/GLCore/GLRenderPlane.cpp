//
//  GLRenderPlane.cpp
//  TNG
//
//  Created by Andre Waschk on 18/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "GLRenderPlane.h"


using namespace OpenGL::GLCore;
using namespace Core::Math;



static float PlaneData[32] =
{
  -1.0f, 1.0f, -0.5f,
  -1.0f, -1.0f, -0.5f,
  1.0f, -1.0f, -0.5f,
  1.0f, 1.0f, -0.5f,
};

static int PlaneIndices[6]{
  0, 1, 2,
  2, 3, 0,
};


GLRenderPlane::GLRenderPlane(const Core::Math::Vec2ui screenResolution){
  Initialize(PlaneData, PlaneIndices, 4, 6);
  
  m_mWorld.Translation(0, 0, 0);
  m_mWorld.Scaling(Vec3f(1,1,1));
  m_mView.BuildLookAt(Vec3f(0,0,2), Vec3f(0,0,0), Vec3f(0,1,0));
  m_mProjection.Ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.1f, 100.0f);
}
GLRenderPlane::~GLRenderPlane(){}

const Core::Math::Mat4f GLRenderPlane::getPlaneView() const{
  return m_mView;
}
const Core::Math::Mat4f GLRenderPlane::getPlaneProjection() const{
  return m_mProjection;
}
const Core::Math::Mat4f GLRenderPlane::getPlaneWorld() const{
  return m_mWorld;
}
