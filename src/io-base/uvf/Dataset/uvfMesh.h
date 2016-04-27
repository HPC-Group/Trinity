#pragma once

#include "Mesh/Mesh.h"

class GeometryDataBlock;

class uvfMesh : public Mesh
{
public:
  uvfMesh(const GeometryDataBlock& tsb);
};
