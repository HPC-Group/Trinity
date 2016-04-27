#define _NOMINMAX
#include <stdexcept>
#include "MaxMinDataBlock.h"

using namespace std;
using namespace UVFTables;

MaxMinDataBlock::MaxMinDataBlock(size_t iComponentCount) : 
  DataBlock()
{
  ulBlockSemantics = BS_MAXMIN_VALUES;
  strBlockID       = "Brick Max/Min Values";
  m_vfMaxMinData.clear();

  SetComponentCount(iComponentCount);
}

void MaxMinDataBlock::SetComponentCount(size_t iComponentCount) {
  m_iComponentCount = iComponentCount;
  m_GlobalMaxMin.resize(m_iComponentCount);
  ResetGlobal();
}

void MaxMinDataBlock::ResetGlobal() {
  for (size_t i = 0;i<m_iComponentCount;++i)
    m_GlobalMaxMin[i] = MinMaxBlock( std::numeric_limits<double>::max(),
                                    -std::numeric_limits<double>::max(),
                                     std::numeric_limits<double>::max(),
                                    -std::numeric_limits<double>::max());
}

MaxMinDataBlock::MaxMinDataBlock(const MaxMinDataBlock &other) :
  DataBlock(other),
  m_GlobalMaxMin(other.m_GlobalMaxMin),
  m_vfMaxMinData(other.m_vfMaxMinData),
  m_iComponentCount(other.m_iComponentCount)
{
}

MaxMinDataBlock& MaxMinDataBlock::operator=(const MaxMinDataBlock& other) {
  strBlockID = other.strBlockID;
  ulBlockSemantics = other.ulBlockSemantics;
  ulCompressionScheme = other.ulCompressionScheme;
  ulOffsetToNextDataBlock = other.ulOffsetToNextDataBlock;

  m_iComponentCount = other.m_iComponentCount;
  m_GlobalMaxMin = other.m_GlobalMaxMin;
  m_vfMaxMinData = other.m_vfMaxMinData;

  return *this;
}


MaxMinDataBlock::MaxMinDataBlock(LargeRAWFile_ptr pStreamFile, uint64_t iOffset, bool bIsBigEndian) {
  GetHeaderFromFile(pStreamFile, iOffset, bIsBigEndian);
}

MaxMinDataBlock::~MaxMinDataBlock() 
{
}

DataBlock* MaxMinDataBlock::Clone() const {
  return new MaxMinDataBlock(*this);
}

uint64_t MaxMinDataBlock::GetHeaderFromFile(LargeRAWFile_ptr pStreamFile, uint64_t iOffset, bool bIsBigEndian) {
  uint64_t iStart = iOffset + DataBlock::GetHeaderFromFile(pStreamFile, iOffset, bIsBigEndian);
  pStreamFile->SeekPos(iStart);

  uint64_t ulBrickCount;
  pStreamFile->ReadData(ulBrickCount, bIsBigEndian);
  { // Widen component count to 64 bits during the read.
    uint64_t component_count;
    pStreamFile->ReadData(component_count, bIsBigEndian);
    SetComponentCount(static_cast<size_t>(component_count));
  }

  m_vfMaxMinData.resize(size_t(ulBrickCount));

  for(MaxMinVec::iterator i = m_vfMaxMinData.begin();
    i != m_vfMaxMinData.end(); ++i) {
    (*i).resize(m_iComponentCount);
    for (size_t j = 0;j<m_iComponentCount;j++) {
      pStreamFile->ReadData((*i)[j].minScalar, bIsBigEndian);
      pStreamFile->ReadData((*i)[j].maxScalar, bIsBigEndian);
      pStreamFile->ReadData((*i)[j].minGradient, bIsBigEndian);
      pStreamFile->ReadData((*i)[j].maxGradient, bIsBigEndian);
  
      m_GlobalMaxMin[j].merge((*i)[j]);
    }
  }

  return pStreamFile->GetPos() - iOffset;
}

uint64_t MaxMinDataBlock::CopyToFile(LargeRAWFile_ptr pStreamFile, uint64_t iOffset, bool bIsBigEndian, bool bIsLastBlock) {
  CopyHeaderToFile(pStreamFile, iOffset, bIsBigEndian, bIsLastBlock);

  // for some strange reason throwing in the raw expression (RHS) into
  // WriteData causes random values to written into the file on windows
  uint64_t ulBrickCount = uint64_t(m_vfMaxMinData.size());
  pStreamFile->WriteData(ulBrickCount, bIsBigEndian);
  { // Widen to 64bits during the write.
    uint64_t component_count = m_iComponentCount;
    pStreamFile->WriteData(component_count, bIsBigEndian);
  }

  for (MaxMinVec::const_iterator i = m_vfMaxMinData.begin();
     i != m_vfMaxMinData.end(); ++i) {
    for (size_t j = 0;j<m_iComponentCount;j++) {
      pStreamFile->WriteData((*i)[j].minScalar, bIsBigEndian);
      pStreamFile->WriteData((*i)[j].maxScalar, bIsBigEndian);
      pStreamFile->WriteData((*i)[j].minGradient, bIsBigEndian);
      pStreamFile->WriteData((*i)[j].maxGradient, bIsBigEndian);
    }
  }

  return pStreamFile->GetPos() - iOffset;
}


uint64_t MaxMinDataBlock::GetOffsetToNextBlock() const {
  return DataBlock::GetOffsetToNextBlock() + ComputeDataSize();
}

uint64_t MaxMinDataBlock::ComputeDataSize() const {  
  return sizeof(uint64_t) +                              // length of the vector
         sizeof(uint64_t) +                              // component count
         32 * m_vfMaxMinData.size() * m_iComponentCount; // vector of data
}

const MinMaxBlock& MaxMinDataBlock::GetValue(size_t iIndex, size_t iComponent) const {
  if(iIndex >= m_vfMaxMinData.size() ||
     iComponent >= m_vfMaxMinData[iIndex].size()) {
    throw std::length_error("MaxMinDataBlock: Invalid maxmin index.");
  }
  return m_vfMaxMinData[iIndex][iComponent];
}

void MaxMinDataBlock::StartNewValue() {
  vector<MinMaxBlock> elems;
  MinMaxBlock elem(std::numeric_limits<double>::max(),
                  -std::numeric_limits<double>::max(),
                   std::numeric_limits<double>::max(),
                  -std::numeric_limits<double>::max());
  for (uint64_t i = 0;i<m_iComponentCount;i++) elems.push_back(elem);  
  m_vfMaxMinData.push_back(elems);
}

void MaxMinDataBlock::MergeData(const std::vector<Core::Math::Vec4d>& fMaxMinData)
{
  for (size_t i = 0;i<m_iComponentCount;i++) {
    MergeData(MinMaxBlock(fMaxMinData[i].x, fMaxMinData[i].y,
                          fMaxMinData[i].z, fMaxMinData[i].w), i);
  }
}

void MaxMinDataBlock::MergeData(const MinMaxBlock& data, const size_t iComponent) {
  m_GlobalMaxMin[iComponent].merge(data);
  m_vfMaxMinData[m_vfMaxMinData.size()-1][iComponent].merge(data);
}

void MaxMinDataBlock::SetDataFromFlatVector(BrickStatVec& source, uint64_t iComponentCount) {
  const size_t stcc = size_t(iComponentCount);

  ResetGlobal();  
  size_t brickCount = size_t(source.size()/stcc);
  
  m_vfMaxMinData.resize(brickCount);
  for (size_t i = 0;i<brickCount;++i) {
    m_vfMaxMinData[i].resize(stcc);

    for (size_t j = 0;j<stcc;++j) {
      MinMaxBlock data(source[i*stcc+j].minScalar,
                       source[i*stcc+j].maxScalar,
                      -std::numeric_limits<double>::max(),
                       std::numeric_limits<double>::max());

      m_vfMaxMinData[i][j] = data;
      m_GlobalMaxMin[j].merge(data);
    }
  }
}
