#pragma once

#include <list>

#include <opengl-base/OpenGLincludes.h>
#include <opengl-base/GLTexture2D.h>
#include <opengl-base/GLTexture3D.h>

#include <silverbullet/time/Timer.h>
#include <common/IIO.h>

//#define GLVOLUMEPOOL_PROFILE // define to measure some timings

#ifdef GLVOLUMEPOOL_PROFILE
#include "Basics/AvgMinMaxTracker.h"
#endif

#include "PoolSlotData.h"
#include "BrickElemInfo.h"


class VisibilityState;
typedef std::shared_ptr<VisibilityState> VisibilityStatePtr;

class GLProgram;
typedef std::shared_ptr<GLProgram> GLProgramPtr;



class GLVolumePool  {
public:
		struct DataSetCache{
      uint32_t m_iBitWidth;
      uint8_t  m_iIsSigned;
      uint8_t  m_iIsFloat;
      uint8_t  m_iCompCount;
      Core::Math::Vec2f m_vRange;
    };
  
  
  enum DebugMode {
    DM_NONE = 0, // no debugging
    DM_BUSY,     // prevent the async worker from doing anything useful forcing the situation when the async updater has not yet updated the metadata but should
    DM_SYNC,     // disables the async worker and forces synchronous metadata updates all the time
    DM_NOEMPTYSPACELEAPING // prevent all visibility computations and thus never ever skip/leap empty space
  };
  
  /// @throws Tuvok::Exception on init error
  GLVolumePool(const Core::Math::Vec3ui& poolSize, std::shared_ptr<trinity::IIO> pDataset, uint64_t modality,
               GLenum filter, bool bUseGLCore=true, DebugMode dm=DM_NONE);
  virtual ~GLVolumePool();
  
  // signals if meta texture is up-to-date including child emptiness for
  // the whole hierarchy
  bool IsVisibilityUpdated() const { return m_bVisibilityUpdated; }
  // @return (totalProcessedBrickCount, emptyBrickCount, childEmptyBrickCount, emptyLeafBrickCount)
  Core::Math::Vec4ui RecomputeVisibility(const VisibilityState& visibility,uint64_t modality, size_t iTimestep, bool bForceSynchronousUpdate = false);
  // returns number of bricks paged in that must not be equal to given
  // number of brick IDs especially if async updater is busy we'll get
  // requests for empty bricks too that won't be paged in
  /// @param brickDebug write out md5sums of bricks as we read them.
  uint32_t uploadBricks(const std::vector<Core::Math::Vec4ui>& vBrickIDs,
                        VisibilityState const& visibility,
                        uint64_t modality,
                        bool brickDebug);
  
  void uploadFirstBrick(const BrickKey& bkey);
  
  // returns false if we need to render first before we can continue to upload further bricks
  bool uploadBrick(const BrickElemInfo& metaData, void* pData); // TODO: we could use the 1D-index here too
  void uploadFirstBrick(const Core::Math::Vec3ui& m_vVoxelSize, void* pData);
  void uploadMetadataTexture();
  void uploadMetadataTexel(uint32_t iBrickID);
  bool isBrickResident(const Core::Math::Vec4ui& vBrickID) const;
  void enable(float fLoDFactor, const Core::Math::Vec3f& vExtend,
              const Core::Math::Vec3f& vAspect,
              GLProgramPtr pShaderProgram) const;
  void disable() const;
  
  // resets the pool and metadata and preserves the single largest brick
  void PH_Reset(const VisibilityState& visibility, size_t iTimestep);
  
  enum MissingBrickStrategy {
    OnlyNeeded,
    RequestAll,
    SkipOneLevel,
    SkipTwoLevels
  };
  std::string getShaderFragment(uint32_t iMetaTextureUnit,
                                uint32_t iDataTextureUnit,
                                enum MissingBrickStrategy,
                                const std::string& = "");
  
  void setFilterMode(GLenum filter);
  
  virtual uint64_t getCPUSize() const;
  virtual uint64_t getGPUSize() const;
  
  uint32_t getLoDCount() const;
  uint32_t getIntegerBrickID(const Core::Math::Vec4ui& vBrickID) const; // x, y , z, lod (w) to iBrickID
  Core::Math::Vec4ui getVectorBrickID(uint32_t iBrickID) const;
  Core::Math::Vec3ui const& getPoolCapacity() const;
  Core::Math::Vec3ui const& getVolumeSize() const;
  Core::Math::Vec3ui const& getMaxInnerBrickSize() const;
  
  struct MinMax {
    double min;
    double max;
  };
  
  uint64_t getMaxUsedBrickBytes() const { return m_iMaxUsedBrickBytes; }
  DataSetCache getCacheInfo() const { return m_sDataSetCache; }
  
protected:
  GLTexture3DPtr m_pPoolMetadataTexture;
  GLTexture3DPtr m_pPoolDataTexture;
  Core::Math::Vec3ui m_vPoolCapacity;
  Core::Math::Vec3ui m_poolSize;
  Core::Math::Vec3ui m_maxInnerBrickSize;
  Core::Math::Vec3ui m_maxTotalBrickSize;
  Core::Math::Vec3ui m_volumeSize;
  uint32_t m_iLoDCount;
  GLenum m_filter;
  GLint m_internalformat;
  GLenum m_format;
  GLenum m_type;
  uint64_t m_iTimeOfCreation;
  
  uint32_t m_iMetaTextureUnit;
  uint32_t m_iDataTextureUnit;
  bool m_bUseGLCore;
  
  size_t m_iInsertPos;
  
  uint32_t m_iTotalBrickCount;
  std::shared_ptr<trinity::IIO> m_pDataset;
  
  bool m_bVisibilityUpdated;
  
  void* lastbrick;
  
#ifdef GLVOLUMEPOOL_PROFILE
  Timer m_Timer;
  AvgMinMaxTracker<float> m_TimesRecomputeVisibilityForBrickPool;
  AvgMinMaxTracker<float> m_TimesMetaTextureUpload;
  AvgMinMaxTracker<float> m_TimesRecomputeVisibility;
#endif
  
  std::vector<uint32_t>     m_vBrickMetadata;  // ref by iBrickID, size of total brick count + some unused 2d texture padding
  std::vector<PoolSlotData> m_vPoolSlotData;   // size of available pool slots
  std::vector<uint32_t>     m_vLoDOffsetTable; // size of LoDs, stores index sums, level 0 is finest
  
  size_t m_iMinMaxScalarTimestep;        // current timestep of scalar acceleration structure below
  size_t m_iMinMaxGradientTimestep;      // current timestep of gradient acceleration structure below
  std::vector<MinMax> m_vMinMaxScalar;   // accelerates access to minmax scalar information, gets constructed in c'tor
  std::vector<MinMax> m_vMinMaxGradient; // accelerates access to minmax gradient information, gets constructed on first access to safe some mem
  double m_BrickIOTime;
  uint64_t m_BrickIOBytes;
  
  // time savers, derived from Dataset::GetMaxUsedBrickSize()
  uint64_t m_iMaxUsedBrickVoxelCount;
  uint64_t m_iMaxUsedBrickBytes;
  
  DebugMode const m_eDebugMode;
  
  DataSetCache m_sDataSetCache;

  
  void createGLResources();
  void freeGLResources();
  
  void prepareForPaging();
  
  void uploadBrick(uint32_t iBrickID, const Core::Math::Vec3ui& vVoxelSize, void* pData,
                   size_t iInsertPos, uint64_t iTimeOfCreation);
  
};
