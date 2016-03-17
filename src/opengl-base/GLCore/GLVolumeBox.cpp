//
//  GLVolumeBox.cpp
//  TNG
//
//  Created by Andre Waschk on 12/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "GLVolumeBox.h"

using namespace OpenGL::GLCore;

using namespace Core::Math;



static int CubeIndicies[36]{
  0, 1, 2,
  2, 3, 0,
  
  4, 7, 6,
  6, 5, 4,
  
  8, 9, 10,
  10, 11, 8,
  
  12, 15, 14,
  14, 13, 12,
  
  16, 17, 18,
  18, 19, 16,
  
  20, 23, 22,
  22, 21, 20
};


GLVolumeBox::GLVolumeBox(){
	m_Cube = new float[6*4*3];
	FillCube(Vec3f(-0.5f, -0.5f, -0.5f), Vec3f(0.5f, 0.5f, 0.5f));
	Initialize(m_Cube, CubeIndicies, 24, 36);
}
GLVolumeBox::~GLVolumeBox(){
    delete [] m_Cube;
}

void GLVolumeBox::FillCube(Core::Math::Vec3f min, Core::Math::Vec3f max){

	//take care of range
	min.x < -0.5f ? min.x = -0.5f : min.x = min.x;
	min.y < -0.5f ? min.y = -0.5f : min.y = min.y;
	min.z < -0.5f ? min.z = -0.5f : min.z = min.z;


	max.x < -0.5f ? max.x = -0.5f : max.x = max.x;
	max.y < -0.5f ? max.y = -0.5f : max.y = max.y;
	max.z < -0.5f ? max.z = -0.5f : max.z = max.z;


	if (max.x < min.x){
		std::swap(max.x, min.x);
	}
	if (max.y < min.y){
		std::swap(max.y, min.y);
	}
	if (max.z < min.z){
		std::swap(max.z, min.z);
	}

    size_t i = 0;
	//top
    m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
    m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
    
	//bottom
    m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
    m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;

	//front
    m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
    m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;

	//back
    m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
    m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
    m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
    m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;

	//left
    m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
    m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
    m_Cube[i++] = min.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
    m_Cube[i++] = min.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;

	//right
    m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = min.z;
    m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = min.z;
    m_Cube[i++] = max.x; m_Cube[i++] = min.y; m_Cube[i++] = max.z;
    m_Cube[i++] = max.x; m_Cube[i++] = max.y; m_Cube[i++] = max.z;
}


void GLVolumeBox::SetSlideLevels(Core::Math::Vec3f min, Core::Math::Vec3f max){
	FillCube(min, max);
	updateVertexData(m_Cube, 24);
}
