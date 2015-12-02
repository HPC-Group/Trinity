//
//  Camera.cpp
//  TNG
//
//  Created by Andre Waschk on 24/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "Camera.h"

using namespace Tuvok::Renderer;
using namespace Core::Math;

static const Vec4f vDir(0,0,1,0);
static const Vec4f vUp(0,1,0,0);
static const Vec4f vRight(1,0,0,0);

Camera::Camera():
m_bFirstPerson(false),
m_vPosition(0,0,-2),
m_vLookAt(0,0,0),
m_vRotation(0,0,0),
m_vDir(0,0,1),
m_vUp(0,1,0),
m_vRight(1,0,0),
m_fZoom(2){
}
Camera::~Camera(){}

void Camera::ResetCamera(){
	m_vPosition = Vec3f(0, 0, -2);
	m_vLookAt = Vec3f(0, 0, 0);
	m_vRotation = Vec3f(0, 0, 0);
	m_vDir = Vec3f(0, 0, 1);
	m_vUp = Vec3f(0, 1, 0);
	m_vRight = Vec3f(1, 0, 0);
	m_fZoom = 2;
}

void Camera::moveCamera(Core::Math::Vec3f direction){
  Vec3f addDir(0,0,0);

  addDir += m_vDir*direction.z;
  addDir += m_vRight*direction.x;
  addDir += m_vUp*direction.y;
  if(!m_bFirstPerson){
    m_vLookAt += addDir;
    m_vPosition = m_vLookAt-(m_vDir*m_fZoom);
  }else{
    m_vPosition += addDir;
    m_vLookAt = m_vPosition+(m_vDir*m_fZoom);
  }
}
void Camera::rotateCamera(Core::Math::Vec3f rotation){
  tilt	(rotation.x);
  pivit	(rotation.y);
  roll	(rotation.z);

  if(!m_bFirstPerson){
    m_vPosition = m_vLookAt-(m_vDir*m_fZoom);

  }else{
    m_vLookAt = m_vPosition+(m_vDir*m_fZoom);
  }
}

Mat4f Camera::buildLookAt(){
  Mat4f view;
  view.BuildLookAt(m_vPosition,m_vLookAt,m_vUp);
  return view;
}


void Camera::setZoom(float f){
  if (f <= 0.1f) f = 0.1f;
  m_fZoom = f;
  rotateCamera();
}
float Camera::getZoom() const{
  return m_fZoom;
}

void Camera::setCameraMode(bool mode){
  m_bFirstPerson = mode;
}
const bool Camera::getFirstPerson() const{
  return m_bFirstPerson;
}

Core::Math::Vec3f Camera::GetWorldPosition() const{
	return m_vPosition;
}


static float pi = 3.14159265359f;
void Camera::roll(float theta)
{
	//Camera Roll:
	//Assumption: Camera Position annd Camera Lookat remain unchanged, but the upVector is modified through rotation
	//Plan of Attack:	Find new upVector by rotating x degrees around the lookAtVector axis

	m_vDir.normalize();
	Vec3f rotateAroundVector = m_vDir;


	//Now find the new point of cameraLookAt using quaternians: Rotate theta around "rotateAroundVector" at point cameraPos
	//q = cos(qTheta) + (i)sin(qTheta)
	//I(i,j,k) is rotateAroundVector[0,1,2]
	//So:	T^-1 * q * T * p * q^-1

	float qTheta = theta / 2;
	Vec3f Pt = m_vUp;

	float modA = sqrt(rotateAroundVector[0] * rotateAroundVector[0]
		+ rotateAroundVector[1] * rotateAroundVector[1]
		+ rotateAroundVector[2] * rotateAroundVector[2]);
	float qSIN = sin(qTheta*pi / 180) / modA;

	float qREAL = cos(qTheta*pi / 180);
	float qI = rotateAroundVector[0] * qSIN;
	float qJ = rotateAroundVector[1] * qSIN;
	float qK = rotateAroundVector[2] * qSIN;

	float pREAL = 0;
	float pI = Pt[0];
	float pJ = Pt[1];
	float pK = Pt[2];

	float qiREAL = qREAL;
	float qiI = -qI;
	float qiJ = -qJ;
	float qiK = -qK;

	Mat4f q, p, qi;
	q = Mat4f(qREAL, -qI, -qJ, -qK,
		qI, qREAL, -qK, qJ,
		qJ, qK, qREAL, -qI,
		qK, -qJ, qI, qREAL);

	p = Mat4f(pREAL, -pI, -pJ, -pK,
		pI, pREAL, -pK, pJ,
		pJ, pK, pREAL, -pI,
		pK, -pJ, pI, pREAL);

	qi = Mat4f(qiREAL, 0, 0, 0,
		qiI, 0, 0, 0,
		qiJ, 0, 0, 0,
		qiK, 0, 0, 0);

	Mat4f resultantMatrix;

	resultantMatrix = q * p * qi;
	Vec3f resultantPoint(resultantMatrix.m21, resultantMatrix.m31, resultantMatrix.m41);
	m_vUp = resultantPoint;

	m_vUp.normalize();
	m_vRight = m_vDir%m_vUp;
};

void Camera::pivit(float theta)
{
	//Camera Pivit (same as Yaw of an aircraft):
	//Assumption: Camera Position remains unchanged, but the cameraLookAt is modified through rotation
	//Plan of Attack:	Find new cameraLookAt by rotating x degrees around the upVector axis
	m_vUp.normalize();
	Vec3f rotateAroundVector = m_vUp;

	//Now find the new point of cameraLookAt using quaternians: Rotate theta around "rotateAroundVector" at point cameraPos
	//q = cos(qTheta) + (i)sin(qTheta)
	//I(i,j,k) is rotateAroundVector[0,1,2]
	//So:	T^-1 * q * T * p * q^-1

	float qTheta = theta / 2;
	Vec3f Pt = m_vDir;

	float modA = sqrt(rotateAroundVector[0] * rotateAroundVector[0]
		+ rotateAroundVector[1] * rotateAroundVector[1]
		+ rotateAroundVector[2] * rotateAroundVector[2]);
	float qSIN = sin(qTheta*pi / 180) / modA;

	float qREAL = cos(qTheta*pi / 180);
	float qI = rotateAroundVector[0] * qSIN;
	float qJ = rotateAroundVector[1] * qSIN;
	float qK = rotateAroundVector[2] * qSIN;

	float pREAL = 0;
	float pI = Pt[0];
	float pJ = Pt[1];
	float pK = Pt[2];

	float qiREAL = qREAL;
	float qiI = -qI;
	float qiJ = -qJ;
	float qiK = -qK;

	Mat4f q, p, qi;
	q = Mat4f(qREAL, -qI, -qJ, -qK,
		qI, qREAL, -qK, qJ,
		qJ, qK, qREAL, -qI,
		qK, -qJ, qI, qREAL);

	p = Mat4f(pREAL, -pI, -pJ, -pK,
		pI, pREAL, -pK, pJ,
		pJ, pK, pREAL, -pI,
		pK, -pJ, pI, pREAL);

	qi = Mat4f(qiREAL, 0, 0, 0,
		qiI, 0, 0, 0,
		qiJ, 0, 0, 0,
		qiK, 0, 0, 0);

	Mat4f resultantMatrix;
	resultantMatrix = q * p * qi;

	Vec3f resultantPoint(resultantMatrix.m21, resultantMatrix.m31, resultantMatrix.m41);

	m_vDir = resultantPoint;
	m_vDir.normalize();

	m_vRight = m_vDir%m_vUp;
	m_vRight.normalize();

};

void Camera::tilt(float theta)
{
	//Camera Tilt (same as Pitch of an aircraft):
	//Assumption: Camera Position remains unchanged, but the cameraLookAt is modified through rotation
	//Plan of Attack:	Find new cameraLookAt by rotating x degrees around the axis
	//					perpendicular to the plane defined by the upVector  and the vector between
	//				    the cameraPosition and the cameraLookAt, at the cameraPosition
	m_vDir.normalize();
	Vec3f lookAtVector = m_vDir;
	Vec3f rotateAroundVector;

	//Calculate Cross Product: lookAtVector X upVector
	rotateAroundVector = lookAtVector % m_vUp;



	//Now find the new point of cameraLookAt using quaternians: Rotate theta around "rotateAroundVector" at point cameraPos
	//q = cos(qTheta) + (i)sin(qTheta)
	//I(i,j,k) is rotateAroundVector[0,1,2]
	//So:	T^-1 * q * T * p * q^-1

	float qTheta = theta / 2;
	Vec3f Pt = m_vLookAt - m_vPosition;

	float modA = sqrt(rotateAroundVector[0] * rotateAroundVector[0]
		+ rotateAroundVector[1] * rotateAroundVector[1]
		+ rotateAroundVector[2] * rotateAroundVector[2]);
	float qSIN = sin(qTheta*pi / 180) / modA;

	float qREAL = cos(qTheta*pi / 180);
	float qI = rotateAroundVector[0] * qSIN;
	float qJ = rotateAroundVector[1] * qSIN;
	float qK = rotateAroundVector[2] * qSIN;

	float pREAL = 0;
	float pI = Pt[0];
	float pJ = Pt[1];
	float pK = Pt[2];

	float qiREAL = qREAL;
	float qiI = -qI;
	float qiJ = -qJ;
	float qiK = -qK;

	Mat4f q, p, qi;
	q = Mat4f(qREAL, -qI, -qJ, -qK,
		qI, qREAL, -qK, qJ,
		qJ, qK, qREAL, -qI,
		qK, -qJ, qI, qREAL);

	p = Mat4f(pREAL, -pI, -pJ, -pK,
		pI, pREAL, -pK, pJ,
		pJ, pK, pREAL, -pI,
		pK, -pJ, pI, pREAL);

	qi = Mat4f(qiREAL, 0, 0, 0,
		qiI, 0, 0, 0,
		qiJ, 0, 0, 0,
		qiK, 0, 0, 0);

	Mat4f resultantMatrix;
	resultantMatrix = q * p * qi;
	Vec3f resultantPoint(resultantMatrix.m21, resultantMatrix.m31, resultantMatrix.m41);

	m_vDir = resultantPoint;
	//m_vLookAt = resultantPoint + m_vPosition;

	//Do it again for upVector: but this only works if upVector was defined correctly
	pREAL = 0;
	pI = m_vUp.x;
	pJ = m_vUp.y;
	pK = m_vUp.z;

	p = Mat4f(pREAL, -pI, -pJ, -pK,
		pI, pREAL, -pK, pJ,
		pJ, pK, pREAL, -pI,
		pK, -pJ, pI, pREAL);

	resultantMatrix = q * p * qi;

	Vec3f resultantPoint2(resultantMatrix.m21, resultantMatrix.m31, resultantMatrix.m41);
	m_vUp = resultantPoint2;

	m_vUp.normalize();
	m_vDir.normalize();
};
