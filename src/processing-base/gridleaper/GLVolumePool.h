#pragma once

#include <list>

#include "Threads.h"

#include <opengl-base/OpenGLincludes.h>
#include <opengl-base/GLTexture2D.h>
#include <opengl-base/GLTexture3D.h>

#include <silverbullet/time/Timer.h>
#include <common/IIO.h>
#include <common/IRenderer.h>


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
  
  enum MissingBrickStrategy {
    OnlyNeeded,
    RequestAll,
    SkipOneLevel,
    SkipTwoLevels
  };
  
  struct MinMax {
    double min;
    double max;
  };

  struct BrickRequest {
    Core::Math::Vec4ui ID;
    BrickKey key;
    bool operator==(const BrickRequest& other) {
      return ID == other.ID && key == other.key;
    }
  };
    
  enum DebugMode {
    DM_NONE = 0, // no debugging
    DM_BUSY,     // prevent the async worker from doing anything useful forcing the situation when the async updater has not yet updated the metadata but should
    DM_SYNC,     // disables the async worker and forces synchronous metadata updates all the time
    DM_NOEMPTYSPACELEAPING // prevent all visibility computations and thus never ever skip/leap empty space
  };
  
  /// @throws Tuvok::Exception on init error
  GLVolumePool(const Core::Math::Vec3ui& poolSize, trinity::IIO& pDataset,
               uint64_t modality,
               GLenum filter, bool bUseGLCore=true, DebugMode dm=DM_NONE);
  virtual ~GLVolumePool();
  
  // signals if meta texture is up-to-date including child emptiness for
  // the whole hierarchy
  bool isVisibilityUpdated() const { return m_bVisibilityUpdated; }
  // @return (totalProcessedBrickCount, emptyBrickCount, childEmptyBrickCount, emptyLeafBrickCount)
  Core::Math::Vec4ui recomputeVisibility(const VisibilityState& visibility,
                                         uint64_t modality, size_t iTimestep,
                                         bool bForceSynchronousUpdate = false);
  // returns number of bricks paged in that must not be equal to given
  // number of brick IDs especially if async updater is busy we'll get
  // requests for empty bricks too that won't be paged in
  uint32_t uploadBricks();
  
  
  void requestBricks(const std::vector<Core::Math::Vec4ui>& vBrickIDs,
                     const VisibilityState& visibility);
  
  void uploadFirstBrick(const BrickKey& bkey);
  
  // returns false if we need to render first before we can continue to upload further bricks
  bool uploadBrick(const BrickElemInfo& metaData, const void* pData); // TODO: we could use the 1D-index here too
  void enable(float fLoDFactor, const Core::Math::Vec3f& vExtend,
              const Core::Math::Vec3f& vAspect,
              GLProgramPtr pShaderProgram) const;
  void disable() const;
  
  bool isValid() const;
  
  std::string getShaderFragment(uint32_t iMetaTextureUnit,
                                uint32_t iDataTextureUnit,
                                enum MissingBrickStrategy,
                                const std::string& = "");
  
  void setFilterMode(GLenum filter);
  
  virtual uint64_t getCPUSize() const;
  virtual uint64_t getGPUSize() const;
  
  
protected:
  trinity::IIO& m_pDataset;
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
  
  bool m_bVisibilityUpdated;

  std::vector<uint32_t>     m_brickMetadata;  // ref by iBrickID, size of total brick count + some unused 2d texture padding
  std::vector<PoolSlotData> m_vPoolSlotData;   // size of available pool slots
  std::vector<uint32_t>     m_vLoDOffsetTable; // size of LoDs, stores index sums, level 0 is finest
  
  size_t m_currentModality;
  size_t m_currentTimestep;
  std::vector<MinMax> m_minMaxScalar;   // accelerates access to minmax scalar information, gets constructed in c'tor
  std::vector<MinMax> m_minMaxGradient; // accelerates access to minmax gradient information, gets constructed on first access to safe some mem
  std::vector<Vec3ui> m_brickVoxelCounts;
  
  
  std::vector <std::vector<Core::Math::Vec3ui>> m_LoDInfoCache;
  
  DebugMode const m_eDebugMode;
  
  DataSetCache m_sDataSetCache;
  
  uint32_t getIntegerBrickID(const Core::Math::Vec4ui& vBrickID) const; // x, y , z, lod (w) to iBrickID
  Core::Math::Vec4ui getVectorBrickID(uint32_t iBrickID) const;
  Vec3ui getBrickVoxelCounts(const Core::Math::Vec4ui& key) const {
    return m_brickVoxelCounts[getIntegerBrickID(key)];
  }
  
  void uploadFirstBrick(const Core::Math::Vec3ui& m_vVoxelSize,
                        const void* pData);

  void uploadMetadataTexture();
  void uploadMetadataTexel(uint32_t iBrickID);
  
  void createGLResources();
  void freeGLResources();
  
  void prepareForPaging();
  
  void uploadBrick(uint32_t iBrickID, const Core::Math::Vec3ui& vVoxelSize,
                   const void* pData,
                   size_t iInsertPos, uint64_t iTimeOfCreation);
  
  
  std::vector<BrickRequest>     m_requestTodo;
  std::vector<BrickRequest>     m_requestDone;
  std::vector<std::shared_ptr<std::vector<uint8_t>>> m_requestStorage;
  
  CriticalSection               m_brickDataCS;
  std::unique_ptr<LambdaThread> m_brickGetterThread;
  
  void brickGetterFunc(Predicate pContinue,
                       LambdaThread::Interface& threadInterface);
  
  void requestBricksFromGetterThread(const std::vector<BrickRequest>& request);
  
  
  void UploadBricksToBrickPool(const std::vector<Core::Math::Vec4ui>& vBrickIDs);  
  
  template<typename T>
  void UploadBricksToBrickPoolT(const std::vector<Core::Math::Vec4ui>& vBrickIDs);
  
  template<trinity::IRenderer::ERenderMode eRenderMode>
  void PotentiallyUploadBricksToBrickPool(const VisibilityState& visibility,
                                          const std::vector<Core::Math::Vec4ui>& vBrickIDs);
  
  template<trinity::IRenderer::ERenderMode eRenderMode, typename T>
  void PotentiallyUploadBricksToBrickPoolT(const VisibilityState& visibility,
                                           const std::vector<Core::Math::Vec4ui>& vBrickIDs);

  template<trinity::IRenderer::ERenderMode eRenderMode>
  bool ContainsData(const VisibilityState& visibility, uint32_t iBrickID);
  
  template<trinity::IRenderer::ERenderMode eRenderMode>
  void RecomputeVisibilityForBrickPool(const VisibilityState& visibility);
  
  template<bool bInterruptable, trinity::IRenderer::ERenderMode eRenderMode>
  Core::Math::Vec4ui RecomputeVisibilityForOctree(const VisibilityState& visibility
  //Tuvok::ThreadClass::PredicateFunction pContinue = Tuvok::ThreadClass::PredicateFunction()
  );
  
};
