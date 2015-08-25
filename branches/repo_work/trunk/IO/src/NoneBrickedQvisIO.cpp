//
//  NonBrickedQvisIO.cpp
//  TNG
//
//  Created by Andre Waschk on 24/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "NoneBrickedQvisIO.h"
#include <IO/VolumeIO/QVisVolume.h>
#include <iostream>

using namespace Tuvok;
using namespace DataIO::VolumeIO;
using namespace Core::Math;

NoneBrickedQvisIO::NoneBrickedQvisIO():
m_pDataset(nullptr),
m_bInitialized(false){

}

bool NoneBrickedQvisIO::load(const std::string& datFile){
  m_pDataset = std::make_shared<QVisVolume>(datFile);
  std::cout << datFile << std::endl;
  m_pDataset->load(false);
  m_bInitialized = true;
  return true;
}

Vec3ui NoneBrickedQvisIO::GetMaxUsedBrickSizes() const{
	return Vec3ui(m_pDataset->getMetadata()->getSize().x, m_pDataset->getMetadata()->getSize().y, m_pDataset->getMetadata()->getSize().z);
}

unsigned NoneBrickedQvisIO::GetLODLevelCount() const {
  return 0;
}

Vec3ui64 NoneBrickedQvisIO::GetDomainSize(const size_t lod,
                                 const size_t ts)const {
  return m_pDataset->getMetadata()->getSize();
}

Vec3d NoneBrickedQvisIO::GetScale() const {
  return Vec3d(1,1,1);
}

size_t NoneBrickedQvisIO::GetLargestSingleBrickLOD(size_t ts) const{
  return 0;
}

Vec3ui NoneBrickedQvisIO::GetBrickVoxelCounts(const BrickKey&) const{
	Vec3ui64 s = m_pDataset->getMetadata()->getSize();
	return Vec3ui(s.x,s.y,s.z);
}

unsigned NoneBrickedQvisIO::GetBitWidth() const {
  return m_pDataset->getMetadata()->getUsedBits();
}

uint64_t NoneBrickedQvisIO::GetComponentCount() const {
  return m_pDataset->getMetadata()->getComponents();
}

bool NoneBrickedQvisIO::GetIsSigned() const {
  return m_pDataset->getMetadata()->getType().m_signed;
}
bool NoneBrickedQvisIO::GetIsFloat() const {
  return m_pDataset->getMetadata()->getType().m_float;
}

void NoneBrickedQvisIO::toString(){
  if(m_bInitialized){
    std::cout << "qvis loaded" << std::endl;
    std::cout << m_pDataset->getMetadata()->getAspectRatio() << std::endl;
  }
}


bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<uint8_t>& ptr) const{
  if(!m_bInitialized){
    return false;
  }
  long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
  std::vector<uint8_t> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
  ptr = data;
  return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<int8_t>& ptr) const{
  if(!m_bInitialized){
    return false;
  }
  long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
  std::vector<int8_t> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
  ptr = data;
  return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<uint16_t>& ptr) const{
  if(!m_bInitialized){
    return false;
  }
  long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z;

  uint16_t* start = (uint16_t*)m_pDataset->getContinousMemoryPointer();
  std::cout << "[FAKE DATA IO] got pointer" << std::endl;
  std::vector<uint16_t> copydata;
  for (int i = 0; i < size; ++i){

	  copydata.push_back((start[i]));
  }
  std::cout << "[FAKE DATA IO] got pointer" << start[0] << std::endl;
  ptr = copydata;
  return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<int16_t>& ptr) const{
  if(!m_bInitialized){
    return false;
  }
  long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
  std::cout << "16bit int " << size << std::endl;
  std::vector<int16_t> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
  ptr = data;
  return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<uint32_t>& ptr) const{
  if(!m_bInitialized){
    return false;
  }
  long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
  std::vector<uint32_t> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
  ptr = data;
  return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<int32_t>& ptr) const{
  if(!m_bInitialized){
    return false;
  }
  long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
  std::vector<int32_t> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
  ptr = data;
  return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<float>& ptr) const{
	if (!m_bInitialized){
		return false;
	}
	long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
	std::vector<float> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
	ptr = data;
	return true;
}
bool NoneBrickedQvisIO::GetBrick(const BrickKey& key, std::vector<double>& ptr) const{
	if (!m_bInitialized){
		return false;
	}
	long size = m_pDataset->getMetadata()->getSize().x*m_pDataset->getMetadata()->getSize().y*m_pDataset->getMetadata()->getSize().z*m_pDataset->getMetadata()->getUsedBits();
	std::vector<double> data(m_pDataset->getContinousMemoryPointer(), m_pDataset->getContinousMemoryPointer() + size);
	ptr = data;
	return true;
}

void* NoneBrickedQvisIO::GetBrick(){
  return (void*)m_pDataset->getContinousMemoryPointer();
}
