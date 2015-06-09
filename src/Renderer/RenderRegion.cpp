//
//  RenderRegion.cpp
//  TNG
//
//  Created by Andre Waschk on 28/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#include "RenderRegion.h"

using namespace Tuvok::Renderer;
using namespace Core::Math;

RenderRegion::RenderRegion(Vec2ui minCoord, Vec2ui maxCoord):
m_minCoord(minCoord),
m_maxCoord(maxCoord){
}

RenderRegion::~RenderRegion(){

}

bool RenderRegion::ContainsPoint(Vec2ui pos) {
  return (m_minCoord.x < pos.x && pos.x < m_maxCoord.x) &&
  (m_minCoord.y < pos.y && pos.y < m_maxCoord.y);
}

const Vec2ui RenderRegion::getMinCoord() const{
  return m_minCoord;
}
const Vec2ui RenderRegion::getMaxCoord() const{
  return m_maxCoord;
}