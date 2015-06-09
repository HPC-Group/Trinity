//
//  GLVolumeBox.cpp
//  TNG
//
//  Created by Andre Waschk on 12/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "GLVolumeBox.h"

using namespace Tuvok::Renderer::OpenGL::GLCore;

using namespace Core::Math;



static float CubeData[72] =
{
	//0123 top
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,

	//4567 bot
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,

	//8-9-10-11 front
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	//12-13-14-15 back
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,

	//16-17-18-19 left
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	//20-21-22-23 right
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
};

static Side Cube[6]{

		//top
		Side(
		Vec3f(-0.5f, 0.5f, -0.5f),
		Vec3f(-0.5f, 0.5f, 0.5f),
		Vec3f(0.5f, 0.5f, 0.5f),
		Vec3f(0.5f, 0.5f, -0.5f)
		),

		//bottom
		Side(
		Vec3f(-0.5f, -0.5f, -0.5f),
		Vec3f(-0.5f, -0.5f, 0.5f),
		Vec3f(0.5f, -0.5f, 0.5f),
		Vec3f(0.5f, -0.5f, -0.5f)
		),

		//front
		Side(
		Vec3f(-0.5f, 0.5f, 0.5f),
		Vec3f(-0.5f, -0.5f, 0.5f),
		Vec3f(0.5f, -0.5f, 0.5f),
		Vec3f(0.5f, 0.5f, 0.5f)
		),

		//back
		Side(
		Vec3f(-0.5f, 0.5f, -0.5f),
		Vec3f(-0.5f, -0.5f, -0.5f),
		Vec3f(0.5f, -0.5f, -0.5f),
		Vec3f(0.5f, 0.5f, -0.5f)
		),

		//left
		Side(
		Vec3f(-0.5f, 0.5f, -0.5f),
		Vec3f(-0.5f, -0.5f, -0.5f),
		Vec3f(-0.5f, -0.5f, 0.5f),
		Vec3f(-0.5f, 0.5f, 0.5f)
		),

		//right
		Side(
		Vec3f(0.5f, 0.5f, -0.5f),
		Vec3f(0.5f, -0.5f, -0.5f),
		Vec3f(0.5f, -0.5f, 0.5f),
		Vec3f(0.5f, 0.5f, 0.5f)
		)
};


/*static float CubeData[72] =
{
	//0123 top
	-0.5f, 0.5f, -0.5f,
	-0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
	0.5f, 0.5f, -0.5f,

	//4567 bot
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, -0.5f,

	//8-9-10-11 front
	-0.5f, 0.5f, 0.5f,
	-0.5f, -0.5f, 0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,

	//12-13-14-15 back
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, 0.5f, -0.5f,

	//16-17-18-19 left
	-0.5f, 0.5f, -0.5f,
	-0.5f, -0.5f, -0.5f,
	-0.5f, -0.5f, 0.5f,
	-0.5f, 0.5f, 0.5f,

	//20-21-22-23 right
	0.5f, 0.5f, -0.5f,
	0.5f, -0.5f, -0.5f,
	0.5f, -0.5f, 0.5f,
	0.5f, 0.5f, 0.5f,
};*/

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
	m_Cube = new Side[6];
	FillCube(Vec3f(-0.5f, -0.5f, -0.5f), Vec3f(0.5f, 0.5f, 0.5f));
	Initialize(&(m_Cube[0].m_p1.x), CubeIndicies, 24, 36);
}
GLVolumeBox::~GLVolumeBox(){}


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

	//top
	m_Cube[0].m_p1 = Vec3f(min.x, max.y, min.z);
	m_Cube[0].m_p2 = Vec3f(min.x, max.y, max.z);
	m_Cube[0].m_p3 = Vec3f(max.x, max.y, max.z);
	m_Cube[0].m_p4 = Vec3f(max.x, max.y, min.z);

	//bottom
	m_Cube[1].m_p1 = Vec3f(min.x, min.y, min.z);
	m_Cube[1].m_p2 = Vec3f(min.x, min.y, max.z);
	m_Cube[1].m_p3 = Vec3f(max.x, min.y, max.z);
	m_Cube[1].m_p4 = Vec3f(max.x, min.y, min.z);

	//front
	m_Cube[2].m_p1 = Vec3f(min.x, max.y, max.z);
	m_Cube[2].m_p2 = Vec3f(min.x, min.y, max.z);
	m_Cube[2].m_p3 = Vec3f(max.x, min.y, max.z);
	m_Cube[2].m_p4 = Vec3f(max.x, max.y, max.z);

	//back
	m_Cube[3].m_p1 = Vec3f(min.x, max.y, min.z);
	m_Cube[3].m_p2 = Vec3f(min.x, min.y, min.z);
	m_Cube[3].m_p3 = Vec3f(max.x, min.y, min.z);
	m_Cube[3].m_p4 = Vec3f(max.x, max.y, min.z);

	//left
	m_Cube[4].m_p1 = Vec3f(min.x, max.y, min.z);
	m_Cube[4].m_p2 = Vec3f(min.x, min.y, min.z);
	m_Cube[4].m_p3 = Vec3f(min.x, min.y, max.z);
	m_Cube[4].m_p4 = Vec3f(min.x, max.y, max.z);

	//right
	m_Cube[5].m_p1 = Vec3f(max.x, max.y, min.z);
	m_Cube[5].m_p2 = Vec3f(max.x, min.y, min.z);
	m_Cube[5].m_p3 = Vec3f(max.x, min.y, max.z);
	m_Cube[5].m_p4 = Vec3f(max.x, max.y, max.z);
}


void GLVolumeBox::SetSlideLevels(Core::Math::Vec3f min, Core::Math::Vec3f max){
	FillCube(min, max);
	updateVertexData(&(m_Cube[0].m_p1.x), 24);
}