#include "GLRenderPlane.h"

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


GLRenderPlane::GLRenderPlane(){
  Initialize(PlaneData, PlaneIndices, 4, 6);
}

GLRenderPlane::~GLRenderPlane(){}
