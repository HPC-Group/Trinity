#include <cstdlib>
#include <fstream>
#include <iterator>
#include <memory.h>
#include <algorithm>
#include <sstream>

#include "silverbullet/math/MathTools.h"
#include "TransferFunction1D.h"

TransferFunction1D::TransferFunction1D(size_t iSize) :
  m_vValueBBox(0,0)
{
  resize(iSize);
}

TransferFunction1D::TransferFunction1D(const std::string& filename)
{
  load(filename);
}

TransferFunction1D::~TransferFunction1D(void)
{
}

void TransferFunction1D::resize(size_t iSize) {
  m_colorData.resize(iSize*4);
}

float TransferFunction1D::smoothstep(float x) const {
  return 3*x*x-2*x*x*x;
}

void TransferFunction1D::setStdFunction(float fCenterPoint, float fInvGradient) {
  setStdFunction(fCenterPoint, fInvGradient, 0, false);
  setStdFunction(fCenterPoint, fInvGradient, 1, false);
  setStdFunction(fCenterPoint, fInvGradient, 2, false);
  setStdFunction(fCenterPoint, fInvGradient, 3, false);
}

void TransferFunction1D::setStdFunction(float fCenterPoint, float fInvGradient, int iComponent, bool bInvertedStep) {
  size_t iCenterPoint = size_t((m_colorData.size()-1) * float(std::min<float>(std::max<float>(0,fCenterPoint),1)));
  size_t iInvGradient = size_t((m_colorData.size()-1) * float(std::min<float>(std::max<float>(0,fInvGradient),1)));

  size_t iRampStartPoint = (iInvGradient/2 > iCenterPoint)                      ? 0                 : iCenterPoint-(iInvGradient/2);
  size_t iRampEndPoint   = (iInvGradient/2 + iCenterPoint > m_colorData.size())  ? m_colorData.size() : iCenterPoint+(iInvGradient/2);

  if (bInvertedStep) {
    for (size_t i = 0;i<iRampStartPoint;i++)
      m_colorData[i*4+iComponent] = 255;

    for (size_t i = iRampStartPoint;i<iRampEndPoint;i++) {
      float fValue = smoothstep(float(i-iCenterPoint+(iInvGradient/2))/float(iInvGradient));
      m_colorData[i*4+iComponent] = uint8_t(255*(1.0f-fValue));
    }

    for (size_t i = iRampEndPoint;i<m_colorData.size();i++)
      m_colorData[i*4+iComponent] = 0;
  } else {
    for (size_t i = 0;i<iRampStartPoint;i++)
      m_colorData[i*4+iComponent] = 0;

    for (size_t i = iRampStartPoint;i<iRampEndPoint;i++) {
      float fValue = smoothstep(float(i-iCenterPoint+(iInvGradient/2))/float(iInvGradient));
      m_colorData[i*4+iComponent] = uint8_t(255*fValue);
    }

    for (size_t i = iRampEndPoint;i<m_colorData.size();i++)
      m_colorData[i*4+iComponent] = 255;
  }

  computeNonZeroLimits();
}

/// Set the transfer function from an external source.  Assumes the vector
/// has 4-components per element, in RGBA order.
void TransferFunction1D::set(const std::vector<uint8_t>& tf)
{
  m_colorData = tf;
  computeNonZeroLimits();
}

void TransferFunction1D::clear() {
  std::fill(m_colorData.begin(), m_colorData.end(), 0);
  m_vValueBBox = Core::Math::Vec2ui64(0,0);
}

void TransferFunction1D::fillOrTruncate(size_t iTargetSize) {
  size_t iOldSize = m_colorData.size();
  m_colorData.resize(iTargetSize);

  if ( iOldSize < m_colorData.size())
    std::fill(m_colorData.begin()+iOldSize, m_colorData.end(), 0);
  else {
    if ( iOldSize > m_colorData.size())
      computeNonZeroLimits();   // can only change when we reduce the size
  }
}

void TransferFunction1D::resample(size_t iTargetSize) {
  size_t iSourceSize = getSize();
  if (iTargetSize == iSourceSize) return;

  if (iTargetSize < iSourceSize) {
    // downsample

    double delta = double(iSourceSize)/double(iTargetSize);
    for (size_t i = 0;i<iTargetSize;i++) {

      size_t iFrom = size_t(i * delta);
      size_t iTo   = size_t((i+1)* delta);

      uint64_t data[4] = {0,0,0,0};
      for (size_t j = iFrom;j<iTo;++j) {
        data[0] += m_colorData[j*4+0];
        data[1] += m_colorData[j*4+1];
        data[2] += m_colorData[j*4+2];
        data[3] += m_colorData[j*4+3];
      }
      m_colorData[i*4+0] = data[0] / (iTo-iFrom);
      m_colorData[i*4+1] = data[1] / (iTo-iFrom);
      m_colorData[i*4+2] = data[2] / (iTo-iFrom);
      m_colorData[i*4+3] = data[3] / (iTo-iFrom);
    }
    m_colorData.resize(iTargetSize*4);
  } else {
    // upsample
    std::vector< uint8_t > vTmpColorData(iTargetSize*4);
    
    for (size_t i = 0;i<iTargetSize;i++) {
      float fPos = float(i) * float(iSourceSize-1)/float(iTargetSize);
      size_t iFloor = size_t(floor(fPos));
      size_t iCeil  = std::min<size_t>(iFloor+1, iSourceSize-1);
      float fInterp = fPos - float(iFloor);

      vTmpColorData[i*4+0] = m_colorData[iFloor*4+0] * (1-fInterp) + m_colorData[iCeil*4+0] * fInterp;
      vTmpColorData[i*4+2] = m_colorData[iFloor*4+1] * (1-fInterp) + m_colorData[iCeil*4+1] * fInterp;
      vTmpColorData[i*4+3] = m_colorData[iFloor*4+2] * (1-fInterp) + m_colorData[iCeil*4+2] * fInterp;
      vTmpColorData[i*4+4] = m_colorData[iFloor*4+3] * (1-fInterp) + m_colorData[iCeil*4+3] * fInterp;
    }

    m_colorData =  vTmpColorData;
  }

  computeNonZeroLimits();
}

bool TransferFunction1D::load(const std::string& filename, size_t iTargetSize) {
  if (!load(filename)) {
    return false;
  } else {
    resample(iTargetSize);
    return true;
  }
}


bool TransferFunction1D::load(const std::string& filename) {
  std::ifstream file(filename.c_str());
  if (!load(file)) {
    return false;
  }
  file.close();
  computeNonZeroLimits();
  return true;
}

bool TransferFunction1D::load(std::istream& tf, size_t iTargetSize) {
  if (!load(tf)) {
    return false;
  } else {
    resample(iTargetSize);
    return true;
  }
}

bool TransferFunction1D::save(const std::string& filename) const {
  std::ofstream file(filename.c_str());
  if (!save(file)) return false;
  file.close();
  return true;
}

bool TransferFunction1D::load(std::istream& tf) {
  uint32_t iSize;
  tf >> iSize;
  if(!tf) {
    return false;
  }
  m_colorData.resize(iSize);

  for(size_t i=0;i<m_colorData.size();++i){
      tf >> m_colorData[i];
  }

  return tf.good();
}


bool TransferFunction1D::save(std::ostream& file) const {
  file << m_colorData.size() << std::endl;

  for(size_t i=0;i<m_colorData.size();++i){
    file << m_colorData[i] << " ";
    file << std::endl;
  }

  return true;
}

void TransferFunction1D::computeNonZeroLimits() {
  m_vValueBBox = Core::Math::Vec2ui64(uint64_t(getSize()),0);

  for (uint64_t i = 0;i<getSize();i++) {
    if (m_colorData[i*4+3] != 0) {
      m_vValueBBox.x = std::min<>(m_vValueBBox.x, i);
      m_vValueBBox.y = i;
    }
  }
}


Core::Math::Vec4ui8 TransferFunction1D::getColor(size_t index) const {
  return Core::Math::Vec4ui8(m_colorData[index*4+0],
                             m_colorData[index*4+1],
                             m_colorData[index*4+2],
                             m_colorData[index*4+3]);
}

void TransferFunction1D::setColor(size_t index, Core::Math::Vec4ui8 color) {
  m_colorData[index*4+0] = color[0];
  m_colorData[index*4+1] = color[1];
  m_colorData[index*4+2] = color[2];
  m_colorData[index*4+3] = color[3];
}
