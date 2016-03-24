#pragma once

class PoolSlotData {
public:
  PoolSlotData(const Core::Math::Vec3ui& vPositionInPool) :
  m_iBrickID(-1),
  m_iTimeOfCreation(0),
  m_iOrigTimeOfCreation(0),
  m_vPositionInPool(vPositionInPool)
  {}
  
  bool wasEverUsed() const {
    return m_iBrickID != -1;
  }
  
  bool containsVisibleBrick() const {
    return m_iTimeOfCreation > 1;
  }
  
  void flagEmpty() {
    m_iOrigTimeOfCreation = m_iTimeOfCreation;
    m_iTimeOfCreation = 1;
  }
  
  void restore() {
    m_iTimeOfCreation = m_iOrigTimeOfCreation;
  }
  
  const Core::Math::Vec3ui& positionInPool() const {
    return m_vPositionInPool;
  }
  
  int32_t     m_iBrickID;
  uint64_t    m_iTimeOfCreation;
  uint64_t    m_iOrigTimeOfCreation;
  
private:
  Core::Math::Vec3ui m_vPositionInPool;
  PoolSlotData();
};
