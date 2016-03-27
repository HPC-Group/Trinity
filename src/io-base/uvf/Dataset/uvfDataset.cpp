#include <cstring>
#include <sstream>

#include "uvfDataset.h"


// TODO: bring back cropping
// #include "RAWConverter.h"


#include "silverbullet/math/MathTools.h"
#include "silverbullet/io/FileTools.h"
#include "UVF-File/UVF.h"
#include "UVF-File/DataBlock.h"
#include "UVF-File/Histogram1DDataBlock.h"
#include "UVF-File/KeyValuePairDataBlock.h"
#include "UVF-File/Histogram2DDataBlock.h"
#include "UVF-File/GeometryDataBlock.h"
#include "uvfMesh.h"

#include "common/TrinityError.h"

const unsigned int DEFAULT_BRICKSIZE = 256;
const unsigned int DEFAULT_BUILDER_BRICKSIZE = 64;
const unsigned int DEFAULT_BRICKOVERLAP = 2;
const unsigned int DEFAULT_INCORESIZE =
DEFAULT_BRICKSIZE * DEFAULT_BRICKSIZE * DEFAULT_BRICKSIZE;


using namespace std;
using namespace trinity;

UVFDataset::UVFDataset(const std::string& strFilename,
                       uint64_t iMaxAcceptableBricksize,
                       bool bVerify,
                       bool bMustBeSameVersion) :
  m_bToCBlock(false),
  m_pKVDataBlock(NULL),
  m_bIsSameEndianness(true),
  m_pDatasetFile(NULL),
  m_strFilename(strFilename),
  m_CachedRange(make_pair(+1,-1)),
  m_iMaxAcceptableBricksize(iMaxAcceptableBricksize)
{
  Open(bVerify, false, bMustBeSameVersion);
}

UVFDataset::UVFDataset() :
  m_bToCBlock(false),
  m_pKVDataBlock(NULL),
  m_bIsSameEndianness(true),
  m_pDatasetFile(NULL),
  m_strFilename(""),
  m_CachedRange(make_pair(+1,-1)),
  m_iMaxAcceptableBricksize(DEFAULT_BRICKSIZE)
{
}

UVFDataset::~UVFDataset()
{
  Close();
}

void UVFDataset::Open(bool bVerify, bool bReadWrite, bool bMustBeSameVersion) {
  // open the file
  const std::string& fn = Filename();
  std::wstring wstrFilename(fn.begin(), fn.end());
  m_pDatasetFile = new UVF(wstrFilename);
  std::string strError;
  if(!m_pDatasetFile->Open(bMustBeSameVersion, bVerify, bReadWrite, &strError))
  {
    throw TrinityError("Could not open file", __FILE__, __LINE__);
  }

  if(m_pDatasetFile->ms_ulReaderVersion !=
     m_pDatasetFile->GetGlobalHeader().ulFileVersion) {
    // bMustBeSameVersion must not be set otherwise Open would have thrown an error
    assert(!bMustBeSameVersion && "Open should have failed!");
    LWARNING("Opening UVF file with a version (" << unsigned(m_pDatasetFile->GetGlobalHeader().ulFileVersion) <<
            ") different from this program's ("<< unsigned(m_pDatasetFile->ms_ulReaderVersion) << ")!");
    if (m_pDatasetFile->ms_ulReaderVersion < 4)
      LWARNING("Opening UVF file with a version"
              " older than 4 without TOCBlock support, "
              "some features may not be available.");
  }

  m_timesteps.clear();
  size_t n_timesteps = DetermineNumberOfTimesteps();
  if (n_timesteps == 0) {
    LERROR("No suitable volume block found in UVF file.  Check previous "
            "messages for rejected blocks.");
    Close();
    throw TrinityError("No volume data", __FILE__, __LINE__);
  }

  for (size_t i = 0;i<m_timesteps.size();++i)
    delete m_timesteps[i];
  m_timesteps.resize(n_timesteps);
  if (m_bToCBlock) {
    for (size_t i = 0;i<m_timesteps.size();++i) {
      m_timesteps[i] = new TOCTimestep();
    }
  } else {
    for (size_t i = 0;i<m_timesteps.size();++i) {
      m_timesteps[i] = new RDTimestep();
    }
  }

  // analyze the main data blocks
  FindSuitableDataBlocks();

  LINFO("Open successfully found " << static_cast<unsigned>(n_timesteps) <<
        "suitable data block in the UVF file.");
  LINFO("Analyzing data...");

  m_bIsSameEndianness = m_pDatasetFile->GetGlobalHeader().bIsBigEndian ==
                          Core::Math::EndianConvert::IsBigEndian();

  SetRescaleFactors(Core::Math::Vec3d(1.0,1.0,1.0));
  // get the metadata and the histograms
  for(size_t i=0; i < n_timesteps; ++i) {
    ComputeMetaData(i);
    GetHistograms(i);
  }

  ComputeRange();

  // print out data statistics
  LINFO("  "<<static_cast<unsigned>(n_timesteps)<<" timesteps found in the UVF.");
  for(size_t tsi=0; tsi < n_timesteps; ++tsi) {
    ostringstream stats;
    if (m_bToCBlock) {
      const TOCTimestep* ts = (TOCTimestep*)m_timesteps[tsi];
      stats << "Timestep " << tsi << ":\n"
            << "  Dataset size: "
            << ts->GetDB()->GetLODDomainSize(0).x << " x "
            << ts->GetDB()->GetLODDomainSize(0).y << " x "
            << ts->GetDB()->GetLODDomainSize(0).z << "\n"
            << "  Brick layout of highest resolution level: "
            << ts->GetDB()->GetBrickCount(0).x << " x "
            << ts->GetDB()->GetBrickCount(0).y << " x "
            << ts->GetDB()->GetBrickCount(0).z << "\n  "
            << GetBitWidth() << " bit, "
            << GetComponentCount() << " components\n"
            << "  LOD down to "
            << ts->GetDB()->GetBrickCount(ts->GetDB()->GetLoDCount()-1).x << " x "
            << ts->GetDB()->GetBrickCount(ts->GetDB()->GetLoDCount()-1).y << " x "
            << ts->GetDB()->GetBrickCount(ts->GetDB()->GetLoDCount()-1).z
            << " bricks found.";
    } else {
      const RDTimestep* ts = (RDTimestep*)m_timesteps[tsi];
      stats << "Timestep " << tsi << ":\n"
            << "  Dataset size: "
            << ts->m_aDomainSize[0].x << " x "
            << ts->m_aDomainSize[0].y << " x "
            << ts->m_aDomainSize[0].z << "\n"
            << "  Brick layout of highest resolution level: "
            << ts->m_vaBrickCount[0].x << " x "
            << ts->m_vaBrickCount[0].y << " x "
            << ts->m_vaBrickCount[0].z << "\n  "
            << (ts->GetDB()->bSignedElement[0][0] ?
                std::string("signed ") : std::string("unsigned "))
            << GetBitWidth() << " bit, "
            << GetComponentCount() << " components\n"
            << "  LOD down to "
            << ts->m_vaBrickCount[ts->m_vaBrickCount.size()-1].x << " x "
            << ts->m_vaBrickCount[ts->m_vaBrickCount.size()-1].y << " x "
            << ts->m_vaBrickCount[ts->m_vaBrickCount.size()-1].z
            << " bricks found.";
    }
    LINFO(stats.str().c_str());
  }

  if (m_TriSoupBlocks.size()) {
    LINFO("Extracting Meshes.");
    for(auto tsb = m_TriSoupBlocks.begin(); tsb != m_TriSoupBlocks.end();
        tsb++) {
      shared_ptr<uvfMesh> m(new uvfMesh(**tsb));
      m_vpMeshList.push_back(m);

      stringstream stats;
      stats << "Mesh Statistics:\n"
            << "  Vertex count:"    << m->GetVertices().size() << "\n  "
            << "  Normal count:"    << m->GetNormals().size() << "\n  "
            << "  TexCoords count:" << m->GetTexCoords().size() << "\n  "
            << "  Color count:"     << m->GetColors().size() << "\n  "
            << "  Vertex Index count:"    << m->GetVertexIndices().size() << "\n  "
            << "  Normal Index count:"    << m->GetNormalIndices().size() << "\n  "
            << "  TexCoords Index count:" << m->GetTexCoordIndices().size() << "\n  "
            << "  Color Index count:"     << m->GetColorIndices().size();
      LINFO(stats.str().c_str());
    }
  }
}

void UVFDataset::Close() {
  delete m_pDatasetFile;

  for(std::vector<Timestep*>::iterator ts = m_timesteps.begin();
      ts != m_timesteps.end(); ++ts) {
    delete *ts;
    *ts = NULL;
  }
  m_timesteps.clear();

  m_TriSoupBlocks.clear();
  DeleteMeshes();

  m_pKVDataBlock= NULL;
  m_pDatasetFile= NULL;
}

void UVFDataset::ComputeMetaData(size_t timestep) {
  if(m_bToCBlock) {
    this->ComputeMetadataTOC(timestep);
  } else {
    this->ComputeMetadataRDB(timestep);
  }
}
void UVFDataset::ComputeMetadataTOC(size_t timestep) {
  TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[timestep]);
  const TOCBlock* pVolumeDataBlock = ts->GetDB();
  m_DomainScale = pVolumeDataBlock->GetScale();

  size_t n_bricks = 0;
  for(size_t i=0; i < pVolumeDataBlock->GetLoDCount(); ++i) {
    n_bricks += pVolumeDataBlock->GetBrickCount(i).volume();
  }
  this->NBricksHint(n_bricks);

  for (size_t j = 0;j<pVolumeDataBlock->GetLoDCount();j++) {
    Core::Math::Vec3ui64 bc = pVolumeDataBlock->GetBrickCount(j);

    BrickMD bmd;
    Core::Math::Vec3f vBrickCorner(0,0,0);

    for (uint64_t x=0; x < bc.x; x++) {
      vBrickCorner.y = 0;
      for (uint64_t y=0; y < bc.y; y++) {
        vBrickCorner.z = 0;
        for (uint64_t z=0; z < bc.z; z++) {
          const Core::Math::Vec4ui64 coords(x,y,z,j);
          const BrickKey k = BrickKey(0,timestep, j,
            static_cast<size_t>(z*bc.x*bc.y+y*bc.x + x));

          Core::Math::Vec3f vNormalizedDomainSize =
            Core::Math::Vec3f(GetDomainSize(j, timestep)) *
            Core::Math::Vec3f(pVolumeDataBlock->GetBrickAspect(coords));
          float maxVal = vNormalizedDomainSize.maxVal();
          vNormalizedDomainSize /= maxVal;

          bmd.extents  = Core::Math::Vec3f(GetEffectiveBrickSize(k)) *
            Core::Math::Vec3f(pVolumeDataBlock->GetBrickAspect(coords)) / maxVal;
          bmd.center   = Core::Math::Vec3f(vBrickCorner + bmd.extents/2.0f) -
                         vNormalizedDomainSize * 0.5f;
          bmd.n_voxels = Core::Math::Vec3ui(pVolumeDataBlock->GetBrickSize(coords));
          AddBrick(k, bmd);
          vBrickCorner.z += bmd.extents.z;
        }
        vBrickCorner.y += bmd.extents.y;
      }
      vBrickCorner.x += bmd.extents.x;
    }
  }
  m_aMaxBrickSize = pVolumeDataBlock->GetMaxBrickSize();
}

void UVFDataset::ComputeMetadataRDB(size_t timestep) {
  std::vector<double> vfScale;
  RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[timestep]);
  const RasterDataBlock* pVolumeDataBlock = ts->GetDB();

  size_t iSize = pVolumeDataBlock->ulDomainSize.size();

  // we require the data to be at least 3D
  assert(iSize >= 3);

  // we also assume that x,y,z are in the first 3 components and
  // we have no anisotropy (i.e. ulLODLevelCount.size=1)
  size_t iLODLevel = static_cast<size_t>(pVolumeDataBlock->ulLODLevelCount[0]);
  for (size_t i=0; i < 3 ; i++) {
    ts->m_aOverlap[i] = static_cast<uint32_t>(pVolumeDataBlock->ulBrickOverlap[i]);
    /// @todo FIXME badness -- assume domain scaling information is the
    /// same across all raster data blocks (across all timesteps)
    m_DomainScale[i] = pVolumeDataBlock->dDomainTransformation[i+(iSize+1)*i];
  }
  m_aMaxBrickSize.StoreMax(Core::Math::Vec3ui(
    static_cast<unsigned>(pVolumeDataBlock->ulBrickSize[0]),
    static_cast<unsigned>(pVolumeDataBlock->ulBrickSize[1]),
    static_cast<unsigned>(pVolumeDataBlock->ulBrickSize[2])
  ));

  ts->m_vvaBrickSize.resize(iLODLevel);
  if (ts->m_pMaxMinData) {
    ts->m_vvaMaxMin.resize(iLODLevel);
  }

  for (size_t j = 0;j<iLODLevel;j++) {
    std::vector<uint64_t> vLOD;  vLOD.push_back(j);
    std::vector<uint64_t> vDomSize = pVolumeDataBlock->GetLODDomainSize(vLOD);
    ts->m_aDomainSize.push_back(Core::Math::Vec3ui64(vDomSize[0], vDomSize[1], vDomSize[2]));

    std::vector<uint64_t> vBrickCount = pVolumeDataBlock->GetBrickCount(vLOD);
    ts->m_vaBrickCount.push_back(Core::Math::Vec3ui64(vBrickCount[0], vBrickCount[1], vBrickCount[2]));

    ts->m_vvaBrickSize[j].resize(size_t(ts->m_vaBrickCount[j].x));
    if (ts->m_pMaxMinData) {
      ts->m_vvaMaxMin[j].resize(size_t(ts->m_vaBrickCount[j].x));
    }

    Core::Math::Vec3f vBrickCorner;

    Core::Math::Vec3f vNormalizedDomainSize = Core::Math::Vec3f(GetDomainSize(j, timestep));
    vNormalizedDomainSize /= vNormalizedDomainSize.maxVal();

    BrickMD bmd;
    for (uint64_t x=0; x < ts->m_vaBrickCount[j].x; x++) {
      ts->m_vvaBrickSize[j][size_t(x)].resize(size_t(ts->m_vaBrickCount[j].y));
      if (ts->m_pMaxMinData) {
        ts->m_vvaMaxMin[j][size_t(x)].resize(size_t(ts->m_vaBrickCount[j].y));
      }

      vBrickCorner.y = 0;
      for (uint64_t y=0; y < ts->m_vaBrickCount[j].y; y++) {
        if (ts->m_pMaxMinData) {
          ts->m_vvaMaxMin[j][size_t(x)][size_t(y)].resize(size_t(ts->m_vaBrickCount[j].z));
        }

        vBrickCorner.z = 0;
        for (uint64_t z=0; z < ts->m_vaBrickCount[j].z; z++) {
          std::vector<uint64_t> vBrick;
          vBrick.push_back(x);
          vBrick.push_back(y);
          vBrick.push_back(z);
          std::vector<uint64_t> vBrickSize =
            pVolumeDataBlock->GetBrickSize(vLOD, vBrick);

          ts->m_vvaBrickSize[j][size_t(x)][size_t(y)].push_back(
            Core::Math::Vec3ui64(vBrickSize[0], vBrickSize[1], vBrickSize[2]));

          const BrickKey k = BrickKey(0, timestep, j,
            static_cast<size_t>(z*ts->m_vaBrickCount[j].x*ts->m_vaBrickCount[j].y+
                                y*ts->m_vaBrickCount[j].x + x));

          bmd.extents  = Core::Math::Vec3f(GetEffectiveBrickSize(k)) /
                         float(GetDomainSize(j, timestep).maxVal());
          bmd.center   = Core::Math::Vec3f(vBrickCorner + bmd.extents/2.0f) -
                         vNormalizedDomainSize * 0.5f;
          bmd.n_voxels = Core::Math::Vec3ui(static_cast<unsigned>(vBrickSize[0]),
                                     static_cast<unsigned>(vBrickSize[1]),
                                     static_cast<unsigned>(vBrickSize[2]));
          AddBrick(k, bmd);
          vBrickCorner.z += bmd.extents.z;
        }
        vBrickCorner.y += bmd.extents.y;
      }
      vBrickCorner.x += bmd.extents.x;
    }
  }

  size_t iSerializedIndex = 0;
  if (ts->m_pMaxMinData) {
    for (size_t lod=0; lod < iLODLevel; lod++) {
      for (uint64_t z=0; z < ts->m_vaBrickCount[lod].z; z++) {
        for (uint64_t y=0; y < ts->m_vaBrickCount[lod].y; y++) {
          for (uint64_t x=0; x < ts->m_vaBrickCount[lod].x; x++) {
            // for four-component data we use the fourth component
            // (presumably the alpha channel); for all other data we use
            // the first component
            /// \todo we may have to change this if we add support for other
            /// kinds of multicomponent data.
            try {
              ts->m_vvaMaxMin[lod][size_t(x)][size_t(y)][size_t(z)] =
                ts->m_pMaxMinData->GetValue(iSerializedIndex++,
                   (pVolumeDataBlock->ulElementDimensionSize[0] == 4) ? 3 : 0
                );
            } catch(const std::length_error&) {
              MinMaxBlock elem;
              const std::pair<double,double> mm = std::make_pair(
                -std::numeric_limits<double>::max(),
                 std::numeric_limits<double>::max()
              );
              elem.minScalar = elem.minGradient = mm.first;
              elem.maxScalar = elem.maxGradient = mm.second;
              ts->m_vvaMaxMin[lod][size_t(x)][size_t(y)][size_t(z)] = elem;
            }
          }
        }
      }
    }
  }
}

// One dimensional brick shrinking for internal bricks that have some overlap
// with neighboring bricks.  Assumes overlap is constant per dataset: this
// brick's overlap with the brick to its right is the same as the overlap with
// the right brick's overlap with the brick to the left.
/// @param v            original brick size for this dimension
/// @param brickIndex   index of the brick in this dimension
/// @param maxindex     number-1 of bricks for this dimension
/// @param overlap      amount of per-brick overlap.
void UVFDataset::FixOverlap(uint64_t& v, uint64_t brickIndex, uint64_t maxindex, uint64_t overlap) const {
  if(brickIndex > 0) {
    v -= static_cast<size_t>(overlap/2.0f);
  }
  if(brickIndex < maxindex) {
    v -= static_cast<size_t>(overlap/2.0f);
  }
}

size_t UVFDataset::DetermineNumberOfTimesteps()
{
  size_t toc=0, raster=0, hist1d=0, hist2d=0, accel=0;
  bool is_color = false;
  for(size_t block=0; block < m_pDatasetFile->GetDataBlockCount(); ++block) {
    switch(m_pDatasetFile->GetDataBlock(block)->GetBlockSemantic()) {
      case UVFTables::BS_1D_HISTOGRAM: hist1d++; break;
      case UVFTables::BS_2D_HISTOGRAM: hist2d++; break;
      case UVFTables::BS_MAXMIN_VALUES: accel++; break;
      case UVFTables::BS_REG_NDIM_GRID:
        {
          const RasterDataBlock *rdb =
            static_cast<const RasterDataBlock*>
                       (m_pDatasetFile->GetDataBlock(block).get());
          if(VerifyRasterDataBlock(rdb)) {
            ++raster;
            if (rdb->ulElementDimensionSize[0] == 4 ||
                rdb->ulElementDimensionSize[0] == 3) {
              is_color = true;
            }
          }
        }
        break;
      case UVFTables::BS_TOC_BLOCK:
        {
          const TOCBlock *tb =
            static_cast<const TOCBlock*>
                       (m_pDatasetFile->GetDataBlock(block).get());
          if(VerifyTOCBlock(tb)) {
            ++toc;
            if (tb->GetComponentCount() == 4 || tb->GetComponentCount() == 3) {
              is_color = true;
            }
          }
        }
        break;
      default: break;
    }
  }
  LINFO("Block counts: (toc=" <<
        static_cast<unsigned>(toc) << ", raster=" <<
        static_cast<unsigned>(raster) << ", histo1D=" <<
        static_cast<unsigned>(hist1d)<< ", histo2D=" <<
        static_cast<unsigned>(hist2d)<< ", accel=" <<
        static_cast<unsigned>(accel) << ")");

  if (toc > raster) m_bToCBlock = true;

  // color data is weird; none of our extra blocks are useful.
  if(is_color) {
    return std::max(raster,toc);
  }
  if(raster+toc == hist1d && hist1d == hist2d && hist2d == accel) {
    return std::max(raster,toc);
  }
  // if the number of blocks don't match, say we have 0 valid timesteps.
  LERROR("UVF Block combinations do not match; " <<
         "do not know how to interpret data.  " <<
         "Block counts (toc="<< static_cast<unsigned>(toc) <<
         ", raster="<< static_cast<unsigned>(raster) <<
         ", hist1="<< static_cast<unsigned>(hist1d) <<
         ", hist2="<< static_cast<unsigned>(hist2d) <<
         ", accel="<< static_cast<unsigned>(accel) << ")");
  throw TrinityError("No valid timesteps in UVF!", __FILE__, __LINE__);
}

bool UVFDataset::VerifyRasterDataBlock(const RasterDataBlock* rdb) const
{
  if((rdb->ulDomainSize.size() < 3) ||
     (rdb->ulElementDimension != 1) ||
     (rdb->ulLODGroups[0] != rdb->ulLODGroups[1]) ||
     (rdb->ulLODGroups[1] != rdb->ulLODGroups[2])) {
    return false;
  }

  /// \todo: change this if we want to support vector data
  // check if we have anything other than scalars or color
  if (rdb->ulElementDimensionSize[0] == 1 ||
      rdb->ulElementDimensionSize[0] == 3 ||
      rdb->ulElementDimensionSize[0] == 4) {
    // check if the data's coarsest LOD level contains only one brick
    const vector<uint64_t>& vSmallestLODBrickCount = rdb->GetBrickCount(rdb->GetSmallestBrickIndex());
    uint64_t iSmallestLODBrickCount = vSmallestLODBrickCount[0];
    for (size_t i = 1;i<3;i++) iSmallestLODBrickCount *= vSmallestLODBrickCount[i]; // currently we only care about the first 3 dimensions
    if (iSmallestLODBrickCount > 1) {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

bool UVFDataset::VerifyTOCBlock(const TOCBlock* tb) const
{
  /// \todo: change this if we want to support vector data
  // check if we have anything other than scalars or color
  if (tb->GetComponentCount() == 1 || tb->GetComponentCount() == 4) {
    assert(tb->GetLoDCount() > 0); // we should have some data..
    // check if the data's coarsest LOD level contains only one brick
    // this should always be true by design of the TOC-Block but
    // we check it here in case we allow exceptionsto this in the future
    const uint64_t vSmallestLODBrickCount =
      tb->GetBrickCount(tb->GetLoDCount()-1).volume();
    return vSmallestLODBrickCount == 1;
  } else {
    return false;
  }
}


// Gives the size of a brick in real space.
Core::Math::Vec3ui64 UVFDataset::GetEffectiveBrickSize(const BrickKey &k) const
{
  if (m_bToCBlock) {
    const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[k.timestep]);
    return Core::Math::Vec3ui64(ts->GetDB()->GetBrickSize(KeyToTOCVector(k)) - 2*ts->GetDB()->GetOverlap());
  } else {
    const NDBrickKey& key = IndexToVectorKey(k);
    size_t iLOD = static_cast<size_t>(k.lod);
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[key.timestep]);

    Core::Math::Vec3ui64 vBrickSize =
      ts->m_vvaBrickSize[iLOD]
                      [static_cast<size_t>(key.brick[0])]
                      [static_cast<size_t>(key.brick[1])]
                      [static_cast<size_t>(key.brick[2])];


    // If this is an internal brick, the size is a bit smaller based on the
    // amount of overlap per-brick.
    if (ts->m_vaBrickCount[iLOD].x > 1) {
      FixOverlap(vBrickSize.x, key.brick[0], ts->m_vaBrickCount[iLOD].x-1, ts->m_aOverlap.x);
    }
    if (ts->m_vaBrickCount[iLOD].y > 1) {
      FixOverlap(vBrickSize.y, key.brick[1], ts->m_vaBrickCount[iLOD].y-1, ts->m_aOverlap.y);
    }
    if (ts->m_vaBrickCount[iLOD].z > 1) {
      FixOverlap(vBrickSize.z, key.brick[2], ts->m_vaBrickCount[iLOD].z-1, ts->m_aOverlap.z);
    }

    return vBrickSize;
  }
}

BrickTable::size_type UVFDataset::GetBrickCount(size_t lod, size_t iTs) const
{
  if (m_bToCBlock) {
    const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[iTs]);
    return  BrickTable::size_type(ts->GetDB()->GetBrickCount(lod).volume());
  } else {
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[iTs]);
    return BrickTable::size_type(ts->m_vaBrickCount[lod].volume());
  }
}

size_t UVFDataset::GetLargestSingleBrickLOD(size_t ts) const {
  for (size_t iLoD = 0;iLoD<GetLODLevelCount();++iLoD) {
    if (GetBrickCount(iLoD, ts) == 1) return iLoD;
  }
  throw TrinityError("invalid brick structure found, no "
                     "single brick level exists.", __FILE__, __LINE__);
}

Core::Math::Vec3ui UVFDataset::GetBrickLayout(const size_t lod,
                                       const size_t iTs) const {
  if (m_bToCBlock) {
    const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[iTs]);
    return Core::Math::Vec3ui(ts->GetDB()->GetBrickCount(lod));
  } else {
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[iTs]);
    return Core::Math::Vec3ui(ts->m_vaBrickCount[lod]);
  }
}


Core::Math::Vec3ui64 UVFDataset::GetDomainSize(const size_t lod, const size_t iTs) const
{
  if (m_bToCBlock) {
    const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[iTs]);
    return ts->GetDB()->GetLODDomainSize(lod);
  } else {
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[iTs]);
    return ts->m_aDomainSize[lod];
  }
}

uint64_t UVFDataset::GetNumberOfTimesteps() const {
  return m_timesteps.size();
}

float UVFDataset::MaxGradientMagnitude() const
{
  float mx = -std::numeric_limits<float>::max();
  for(std::vector<Timestep*>::const_iterator ts = m_timesteps.begin();
      ts != m_timesteps.end(); ++ts) {
    mx = std::max(mx, (*ts)->m_fMaxGradMagnitude);
  }
  return mx;
}

void UVFDataset::FindSuitableDataBlocks() {
  // keep a count of each type of block.  We require that the number of blocks
  // match, or put another way, that all blocks exist for all timesteps.  This
  // isn't strictly necessary; we could still, technically, work with a
  // timestep that was missing acceleration structures.
  size_t data=0, hist1d=0, hist2d=0, accel=0;

  for (size_t iBlocks = 0;
       iBlocks < m_pDatasetFile->GetDataBlockCount();
       iBlocks++) {
    switch(m_pDatasetFile->GetDataBlock(iBlocks)->GetBlockSemantic()) {
      case UVFTables::BS_1D_HISTOGRAM:
        m_timesteps[hist1d++]->m_pHist1DDataBlock =
          static_cast<const Histogram1DDataBlock*>
                     (m_pDatasetFile->GetDataBlock(iBlocks).get());
        break;
      case UVFTables::BS_2D_HISTOGRAM:
        m_timesteps[hist2d++]->m_pHist2DDataBlock =
          static_cast<const Histogram2DDataBlock*>
                     (m_pDatasetFile->GetDataBlock(iBlocks).get());
        break;
      case UVFTables::BS_KEY_VALUE_PAIRS:
        if(m_pKVDataBlock != NULL) {
          LWARNING("Multiple Key-Value pair blocks; using first!");
          continue;
        }
        m_pKVDataBlock = static_cast<const KeyValuePairDataBlock*>
                                 (m_pDatasetFile->GetDataBlock(iBlocks).get());
        break;
      case UVFTables::BS_MAXMIN_VALUES:
        m_timesteps[accel++]->m_pMaxMinData = static_cast<MaxMinDataBlock*>
                                 (m_pDatasetFile->GetDataBlock(iBlocks).get());
        break;
      case UVFTables::BS_TOC_BLOCK:
        if (m_bToCBlock) {
          const TOCBlock* pVolumeDataBlock =
            static_cast<const TOCBlock*>
                       (m_pDatasetFile->GetDataBlock(iBlocks).get());

          if(!VerifyTOCBlock(pVolumeDataBlock)) {
            LWARNING("A TOCBlock failed verification; skipping it");
            continue;
          }
          Core::Math::Vec3ui bsize = pVolumeDataBlock->GetMaxBrickSize();
          for(size_t i=0; i < 3; ++i) {
            if(bsize[i] > m_iMaxAcceptableBricksize) {
              std::stringstream large;
              large << "Brick size used in UVF file is too large ("
                    << bsize[i] << " > " << m_iMaxAcceptableBricksize
                    << "); rebricking necessary.";
              throw TrinityError(large.str().c_str(), __FILE__, __LINE__);
            }
          }

          m_timesteps[data]->block_number = iBlocks;
          m_timesteps[data++]->m_pVolumeDataBlock = pVolumeDataBlock;
        }
        break;
      case UVFTables::BS_REG_NDIM_GRID:
        if (!m_bToCBlock) {
          const RasterDataBlock* pVolumeDataBlock =
            static_cast<const RasterDataBlock*>
                       (m_pDatasetFile->GetDataBlock(iBlocks).get());

          if(!VerifyRasterDataBlock(pVolumeDataBlock)) {
            LWARNING("A RasterDataBlock failed verification; skipping it");
            continue;
          }

          // check if the data's biggest brick dimensions are
          // smaller than m_iMaxAcceptableBricksize
          std::vector<uint64_t> vMaxBrickSizes =
            pVolumeDataBlock->GetLargestBrickSizes();
          // currently we only care about the first 3 dimensions
          for (size_t i=0; i < 3; i++) {
            if (vMaxBrickSizes[i] > m_iMaxAcceptableBricksize) {
              std::stringstream large;
              large << "Brick size used in UVF file is too large ("
                    << vMaxBrickSizes[i] << " > " << m_iMaxAcceptableBricksize
                    << "); rebricking necessary.";
              LWARNING(large.str().c_str());
              throw TrinityError(large.str().c_str(), __FILE__, __LINE__);
            }
          }

          m_timesteps[data]->block_number = iBlocks;
          m_timesteps[data++]->m_pVolumeDataBlock = pVolumeDataBlock;
        }
        break;
      case UVFTables::BS_GEOMETRY: {
        LINFO("Found triangle mesh.");
        m_TriSoupBlocks.push_back(
          static_cast<const GeometryDataBlock*>
                     (m_pDatasetFile->GetDataBlock(iBlocks).get())
        );
      }
      default:
        LINFO("Non-volume block found in UVF file, skipping.");
        break;
    }
  }
}


/// @todo fixme (hack): we only look at the first timestep for the
/// histograms.  should really set a vector of histograms, one per timestep.
void UVFDataset::GetHistograms(size_t) {
  m_pHist1D.reset();

  Timestep* ts = m_timesteps[0];
  if (ts->m_pHist1DDataBlock != NULL) {
    const std::vector<uint64_t>& vHist1D = ts->m_pHist1DDataBlock->GetHistogram();

    m_pHist1D.reset(new Histogram1D(std::min<size_t>(vHist1D.size(),
                                     std::min<size_t>(MAX_TRANSFERFUNCTION_SIZE,
                                                     1<<GetBitWidth()))));

    if (m_pHist1D->getSize() != vHist1D.size()) {
      LINFO("1D Histogram too big to be drawn efficiently, resampling.");
      // "resample" the histogram

      float sampleFactor = static_cast<float>(vHist1D.size()) /
                           static_cast<float>(m_pHist1D->getSize());

      float accWeight = 0.0f;
      float currWeight = 1.0f;
      float accValue = 0.0f;
      size_t j  = 0;
      bool bLast = false;

      for (size_t i = 0;i < vHist1D.size(); i++) {
        if (bLast) {
            m_pHist1D->set(j, uint32_t( accValue ));

            currWeight = 1.0f - currWeight;
            j++;
            i--;
            bLast = false;
            accValue = 0;
            accWeight = 0;
        } else {
          if (sampleFactor-accWeight > 1) {
            currWeight = 1.0f;
          } else {
            currWeight = sampleFactor-accWeight;
            bLast = true;
          }
        }

        accValue  += static_cast<float>(vHist1D[i]) * currWeight;
        accWeight += currWeight;

        // make sure we are not writing beyond m_pHist1D's end
        // due to accumulated float errors in the sampling computation above
        if (j == m_pHist1D->getSize() - 1) break;
      }
    } else {
      for (size_t i = 0;i < m_pHist1D->getSize(); i++) {
        m_pHist1D->set(i, uint32_t(vHist1D[i]));
      }
    }
  } else {
    // generate a zero 1D histogram (max 4k) if none is found in the file
    m_pHist1D.reset(new Histogram1D(
            std::min(MAX_TRANSFERFUNCTION_SIZE, 1<<GetBitWidth())));

    // set all values to one so "getFilledsize" later does not return a
    // completely empty dataset
    for (size_t i = 0;i<m_pHist1D->getSize();i++) {
      m_pHist1D->set(i, 1);
    }
  }

  m_pHist2D.reset();
  if (ts->m_pHist2DDataBlock != NULL) {
    const std::vector<std::vector<uint64_t>>& vHist2D =
      ts->m_pHist2DDataBlock->GetHistogram();

    Core::Math::Vec2ui64 vSize(vHist2D.size(),vHist2D[0].size());

    vSize.x = min<size_t>(MAX_TRANSFERFUNCTION_SIZE, vSize.x);
    vSize.y = min<size_t>(256, vSize.y);
    m_pHist2D.reset(new Histogram2D(vSize));

    if (vSize.x != vHist2D.size() || vSize.y != vHist2D[0].size() ) {
      LINFO("2D Histogram too big to be drawn efficiently, resampling.");

      // TODO: implement the same linear resampling as above
      //       for now we just clear the histogram with ones

      for (size_t y = 0;y<m_pHist2D->getSize().y;y++)
        for (size_t x = 0;x<m_pHist2D->getSize().x;x++)
          m_pHist2D->set(x,y,1);
    } else {
      for (size_t y = 0;y<m_pHist2D->getSize().y;y++)
        for (size_t x = 0;x<m_pHist2D->getSize().x;x++)
          m_pHist2D->set(x,y,uint32_t(vHist2D[x][y]));
    }

    ts->m_fMaxGradMagnitude = ts->m_pHist2DDataBlock->GetMaxGradMagnitude();
  } else {
    // generate a zero 2D histogram (max 4k) if none is found in the file
    Core::Math::Vec2ui64 vec(256, std::min(MAX_TRANSFERFUNCTION_SIZE, 1<<GetBitWidth()));

    m_pHist2D.reset(new Histogram2D(vec));
    for (size_t y=0; y < m_pHist2D->getSize().y; y++) {
      // set all values to one so "getFilledsize" later does not return a
      // completely empty dataset
      for (size_t x=0; x < m_pHist2D->getSize().x; x++) {
        m_pHist2D->set(x,y,1);
      }
    }

    ts->m_fMaxGradMagnitude = 0;
  }
}

Core::Math::Vec3ui UVFDataset::GetBrickVoxelCounts(const BrickKey& k) const
{
 if (m_bToCBlock) {
    const Core::Math::Vec4ui64 coords = KeyToTOCVector(k);
    const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[k.timestep]);
    return Core::Math::Vec3ui(ts->GetDB()->GetBrickSize(coords));
  } else {
    size_t iLOD = static_cast<size_t>(k.lod);
    const NDBrickKey& key = IndexToVectorKey(k);
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[key.timestep]);
    return Core::Math::Vec3ui(ts->m_vvaBrickSize[iLOD]
                                         [static_cast<size_t>(key.brick[0])]
                                         [static_cast<size_t>(key.brick[1])]
                                         [static_cast<size_t>(key.brick[2])]);
  }
}



bool UVFDataset::Export(uint64_t iLODLevel, const std::string& targetFilename,
                        bool bAppend) const {
 if (m_bToCBlock) {
    bool okay = true;
    for(std::vector<Timestep*>::const_iterator ts = m_timesteps.begin();
      ts != m_timesteps.end(); ++ts) {
      const TOCTimestep* toc_ts = static_cast<TOCTimestep*>(*ts);
      // Unbrick each timestep.  Append the data if the user asks, but we must
      // always append on second and subsequent timesteps!
      okay &= toc_ts->GetDB()->BrickedLODToFlatData(
                iLODLevel, targetFilename,
                bAppend || ts != m_timesteps.begin());
    }
    return okay;
  } else {
    std::vector<uint64_t> vLOD; vLOD.push_back(iLODLevel);
    bool okay = true;
    for(std::vector<Timestep*>::const_iterator ts = m_timesteps.begin();
        ts != m_timesteps.end(); ++ts) {
      const RDTimestep* rd_ts = static_cast<RDTimestep*>(*ts);
      // Unbrick each timestep.  Append the data if the user asks, but we must
      // always append on second and subsequent timesteps!
      okay &= rd_ts->GetDB()->BrickedLODToFlatData(
                vLOD, targetFilename,
                bAppend || ts != m_timesteps.begin());
    }
    return okay;
  }
}

bool UVFDataset::ApplyFunction(uint64_t iLODLevel, bfqn* brickFunc,
                               void *pUserContext, uint64_t iOverlap) const {
 if (m_bToCBlock) {
    bool okay = true;
    for(std::vector<Timestep*>::const_iterator ts = m_timesteps.begin();
      ts != m_timesteps.end(); ++ts) {
      const TOCTimestep* toc_ts = static_cast<TOCTimestep*>(*ts);
      okay &= toc_ts->GetDB()->ApplyFunction(
                iLODLevel,
                brickFunc, pUserContext,
                uint32_t(iOverlap)
              );
    }
    return okay;
  } else {
    std::vector<uint64_t> vLOD; vLOD.push_back(iLODLevel);
    bool okay = true;
    for(std::vector<Timestep*>::const_iterator ts = m_timesteps.begin();
        ts != m_timesteps.end(); ++ts) {
      const RDTimestep* rd_ts = static_cast<RDTimestep*>(*ts);
      okay &= rd_ts->GetDB()->ApplyFunction(
                vLOD, brickFunc, pUserContext,
                iOverlap
                );
    }
    return okay;
  }
}

// BrickKey's index is 1D. For UVF's RDB, we've got a 3D index.  When
// we create the brick index to satisfy the interface, we do so in a
// reversible way.  This methods reverses the 1D into into UVF's 3D
// index.
std::vector<uint64_t>
UVFDataset::IndexToVector(const BrickKey &k) const {
  std::vector<uint64_t> vBrick;
  if (!m_bToCBlock) {
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[k.timestep]);
    const size_t lod = k.lod;

    uint64_t iIndex = uint64_t(k.index);
    uint64_t iZIndex = uint64_t(iIndex / (ts->m_vaBrickCount[lod].x *
                                      ts->m_vaBrickCount[lod].y));
    iIndex = iIndex % (ts->m_vaBrickCount[lod].x *
                       ts->m_vaBrickCount[lod].y);
    uint64_t iYIndex = uint64_t(iIndex / ts->m_vaBrickCount[lod].x);
    iIndex = iIndex % ts->m_vaBrickCount[lod].x;
    uint64_t iXIndex = iIndex;

    vBrick.push_back(iXIndex);
    vBrick.push_back(iYIndex);
    vBrick.push_back(iZIndex);
  }
  return vBrick;
}

Core::Math::Vec4ui64 UVFDataset::KeyToTOCVector(const BrickKey &k) const {
  if (m_bToCBlock) {
    const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[k.timestep]);
    const uint64_t iLOD = k.lod;
    const uint64_t iLinearIndex = k.index;
    const Core::Math::Vec3ui64 iBricks = ts->GetDB()->GetBrickCount(iLOD);

    const uint64_t x = iLinearIndex % iBricks.x;
    const uint64_t y = (iLinearIndex % (iBricks.x*iBricks.y)) / iBricks.x;
    const uint64_t z = iLinearIndex / (iBricks.x*iBricks.y);

    return Core::Math::Vec4ui64(x,y,z,iLOD);
  } else {
    return Core::Math::Vec4ui64();
  }

}

BrickKey UVFDataset::TOCVectorToKey(const Core::Math::Vec4ui& hash, size_t timestep) const {
  Core::Math::Vec3ui layout = GetBrickLayout(hash.w, timestep);

  return BrickKey(0, timestep, hash.w, hash.x+
                  hash.y*layout.x+
                  hash.z*layout.x*layout.y);
}

NDBrickKey UVFDataset::IndexToVectorKey(const BrickKey &k) const {
  NDBrickKey ndk;
  ndk.timestep = k.timestep;
  ndk.lod.push_back(k.lod);
  ndk.brick = IndexToVector(k);
  return ndk;
}

// determines the largest actually used brick dimensions
// in the current dataset
Core::Math::Vec3ui UVFDataset::GetMaxUsedBrickSizes() const
{
  Core::Math::Vec3ui vMaxSize(1,1,1);
  Core::Math::Vec3ui vAbsoluteMax(m_aMaxBrickSize);
  // we can do things differently/more intelligently for TOCBlocks.
  if (m_bToCBlock) {
    for(size_t tsi=0; tsi < m_timesteps.size(); ++tsi) {
      const TOCTimestep* ts = (const TOCTimestep*)m_timesteps[tsi];

      // the first brick in the highest LoD is the biggest brick
      // (usually of size vAbsoluteMax)
      vMaxSize.StoreMax(
        Core::Math::Vec3ui(ts->GetDB()->GetBrickSize(Core::Math::Vec4ui64(0,0,0,0)))
      );

      if (vMaxSize == vAbsoluteMax) return vAbsoluteMax;
    }
    return vMaxSize;
  }
  // our parent has no real info, just calculates this on the fly.
  return BrickedDataset::GetMaxUsedBrickSizes();
}

Core::Math::Vec3ui UVFDataset::GetMaxBrickSize() const {
  return m_aMaxBrickSize;
}

Core::Math::Vec3ui UVFDataset::GetBrickOverlapSize() const
{
  /// @todo HACK fixme: should take into account the LOD and timestep, probably
  /// need them as arguments.
  assert(!m_timesteps.empty() && "no data, haven't analyzed UVF?");

  if (m_bToCBlock) {
    uint32_t overlap = ((TOCTimestep*)(m_timesteps[0]))->GetDB()->GetOverlap();
    return Core::Math::Vec3ui(overlap,overlap,overlap);
  } else
    return ((RDTimestep*)(m_timesteps[0]))->m_aOverlap;
}

unsigned UVFDataset::GetLODLevelCount() const {
  /// @todo HACK fixme: should take into account the timestep, needed as arg
  assert(!m_timesteps.empty() && "no data, haven't analyzed UVF?");

  if (m_bToCBlock) {
    return (static_cast<const TOCTimestep*>
                       (m_timesteps[0]))->GetDB()->GetLoDCount();
  } else {
    return (static_cast<const RDTimestep*>
                       (m_timesteps[0]))->m_vvaBrickSize.size();
  }
}

/// \todo change this if we want to support data where elements are of
// different size
unsigned UVFDataset::GetBitWidth() const
{
  assert(!m_timesteps.empty() && "no data, haven't analyzed UVF?");
  // All data in the time series should have the same bit width, so any
  // timestep we choose to query the bit width from should be fine.

  if (m_bToCBlock)
    return ((TOCTimestep*)(m_timesteps[0]))->GetDB()->GetComponentTypeSize()*8;
  else
    return ((RDTimestep*)(m_timesteps[0]))->GetDB()->ulElementBitSize[0][0];

}

uint64_t UVFDataset::GetComponentCount() const
{
  assert(!m_timesteps.empty() && "no data, haven't analyzed UVF?");
  // All data in the time series should have the same bit number of
  // components, so any timestep we choose to query the component count
  // from should be fine.

  if (m_bToCBlock)
    return ((TOCTimestep*)(m_timesteps[0]))->GetDB()->GetComponentCount();
  else
    return ((RDTimestep*)(m_timesteps[0]))->GetDB()->ulElementDimensionSize[0];
}

/// \todo change this if we want to support data where elements are of
/// different type
bool UVFDataset::GetIsSigned() const
{
  assert(!m_timesteps.empty() && "no data, haven't analyzed UVF?");
  // All data in the time series should have the same signedness,
  // so any timestep we choose to query the signedness from should be fine.

  if (m_bToCBlock)
    return ((TOCTimestep*)(m_timesteps[0]))->GetDB()->GetIsSigned();
  else
    return ((RDTimestep*)(m_timesteps[0]))->GetDB()->bSignedElement[0][0];
}

/// \todo change this if we want to support data where elements are of
///  different type
bool UVFDataset::GetIsFloat() const
{
  assert(!m_timesteps.empty() && "no data, haven't analyzed UVF?");
  // All data in the time series should have the same type, so any timestep
  // we choose to query the type from should be fine.
  if (m_bToCBlock)
    return ((TOCTimestep*)(m_timesteps[0]))->GetDB()->GetIsFloat();
  else
    return GetBitWidth() != ((RDTimestep*)(m_timesteps[0]))->GetDB()->ulElementMantissa[0][0];
}

bool UVFDataset::IsSameEndianness() const
{
  return m_bIsSameEndianness;
}

std::pair<double,double> UVFDataset::GetRange() const {
  return m_CachedRange;
}

void UVFDataset::ComputeRange() {

  // If we're missing MaxMin data for any timestep, we don't have maxmin data.
  bool have_maxmin_data = true;
  for(size_t tsi=0; tsi < m_timesteps.size(); ++tsi) {
    if(m_timesteps[tsi]->m_pMaxMinData == NULL) {
      LWARNING("Missing acceleration structure for timestep " <<
              static_cast<unsigned>(tsi));
      have_maxmin_data = false;
    }
  }

  // second < first is a convention we use to indicate "haven't figured this
  // out yet".  We might not have MaxMin data though; in some cases, we'll
  // never figure it out.
  if (have_maxmin_data && m_CachedRange.second < m_CachedRange.first) {
    // to find the range of values we simply traverse all the bricks in
    // LOD level 0 (highest res) and compute the max & min
    std::pair<double,double> limits;

    if (m_bToCBlock) {
      for(size_t tsi=0; tsi < m_timesteps.size(); ++tsi) {
        for (size_t i=0; i < GetBrickCount(0, tsi); i++) {
          const TOCTimestep* ts = static_cast<TOCTimestep*>(m_timesteps[tsi]);
          const MinMaxBlock& maxMinElement = ts->m_pMaxMinData->GetValue(i, ts->GetDB()->GetComponentCount() == 4 ? 3 : 0);

          if (i>0) {
            limits.first  = min(limits.first, maxMinElement.minScalar);
            limits.second = max(limits.second, maxMinElement.maxScalar);
          } else {
            limits = make_pair(maxMinElement.minScalar, maxMinElement.maxScalar);
          }
        }
      }
      m_CachedRange = limits;

    } else {
      for(size_t tsi=0; tsi < m_timesteps.size(); ++tsi) {
        const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[tsi]);

        for (size_t i=0; i < GetBrickCount(0, tsi); i++) {
          BrickKey k(0,tsi,0,i);
          const NDBrickKey& key = IndexToVectorKey(k);

          const MinMaxBlock& maxMinElement =
            ts->m_vvaMaxMin[0]
                          [static_cast<size_t>(key.brick[0])]
                          [static_cast<size_t>(key.brick[1])]
                          [static_cast<size_t>(key.brick[2])];

          if (i>0) {
            limits.first  = min(limits.first, maxMinElement.minScalar);
            limits.second = max(limits.second, maxMinElement.maxScalar);
          } else {
            limits = make_pair(maxMinElement.minScalar, maxMinElement.maxScalar);
          }
        }
        m_CachedRange = limits;
      }
    }
  }
}

MinMaxBlock UVFDataset::MaxMinForKey(const BrickKey& k) const {
  MinMaxBlock maxMinElement;
  if (m_bToCBlock) {
    const TOCTimestep* ts = dynamic_cast<const TOCTimestep*>(m_timesteps[k.timestep]);
    size_t iLinIndex = size_t(ts->GetDB()->GetLinearBrickIndex(KeyToTOCVector(k)));
    return  ts->m_pMaxMinData->GetValue(iLinIndex, ts->GetDB()->GetComponentCount() == 4 ? 3 : 0 );
  } else {
    const NDBrickKey& key = IndexToVectorKey(k);
    size_t iLOD = k.lod;

    const RDTimestep* ts = dynamic_cast<const RDTimestep*>(m_timesteps[key.timestep]);
    return ts->m_vvaMaxMin[iLOD]
                          [static_cast<size_t>(key.brick[0])]
                          [static_cast<size_t>(key.brick[1])]
                          [static_cast<size_t>(key.brick[2])];
  }
}

bool UVFDataset::ContainsData(const BrickKey &k, double isoval) const
{
  // if we have no max min data we have to assume that every block is visible
  if(NULL == m_timesteps[k.timestep]->m_pMaxMinData) {return true;}
  const MinMaxBlock maxMinElement = MaxMinForKey(k);
  return (isoval <= maxMinElement.maxScalar);
}

bool UVFDataset::ContainsData(const BrickKey &k, double fMin,double fMax) const
{
  // if we have no max min data we have to assume that every block is visible
  if(NULL == m_timesteps[k.timestep]->m_pMaxMinData) {return true;}
  const MinMaxBlock maxMinElement = MaxMinForKey(k);
  return (fMax >= maxMinElement.minScalar && fMin <= maxMinElement.maxScalar);
}

bool UVFDataset::ContainsData(const BrickKey &k, double fMin,double fMax, double fMinGradient,double fMaxGradient) const
{
  // if we have no max min data we have to assume that every block is visible
  if(NULL == m_timesteps[k.timestep]->m_pMaxMinData) {return true;}
  const MinMaxBlock maxMinElement = MaxMinForKey(k);
  return (fMax >= maxMinElement.minScalar &&
          fMin <= maxMinElement.maxScalar)
                         &&
         (fMaxGradient >= maxMinElement.minGradient &&
          fMinGradient <= maxMinElement.maxGradient);
}

const std::vector<std::pair<std::string, std::string>> UVFDataset::GetMetadata() const {
  std::vector<std::pair<std::string, std::string>> v;
  if (m_pKVDataBlock)  {
    for (size_t i = 0;i<m_pKVDataBlock->GetKeyCount();i++) {
      v.push_back(std::make_pair(m_pKVDataBlock->GetKeyByIndex(i), m_pKVDataBlock->GetValueByIndex(i)));
    }
  }
  return v;
}

bool UVFDataset::GeometryTransformToFile(size_t iMeshIndex,
                                         const Core::Math::Mat4f& m,
                                         const Core::Math::Vec4f& c) {
  Close();

  LINFO("Attempting to reopen file in readwrite mode.");

  try {
    Open(false,true,false);
    LINFO("Successfully reopened file in readwrite mode.");

    // turn meshindex into block index, those are different as the
    // uvf file most likely also contains data other than meshes
    // such as the volume or histograms, etc.
    size_t iBlockIndex = 0;
    bool bFound = false;

    for(size_t block=0; block < m_pDatasetFile->GetDataBlockCount(); ++block) {
      if (m_pDatasetFile->GetDataBlock(block)->GetBlockSemantic()
                                                  == UVFTables::BS_GEOMETRY) {
        if (iMeshIndex == 0) {
          iBlockIndex = block;
          bFound = true;
          break;
        }
        iMeshIndex--;
      }
    }

    if (!bFound) {
      LERROR("Unable to locate mesh data block " <<
              static_cast<unsigned>(iBlockIndex));
      return false;
    }

    GeometryDataBlock* block = dynamic_cast<GeometryDataBlock*>(
      m_pDatasetFile->GetDataBlockRW(iBlockIndex,false)
    );

    if (!block) {
      LERROR("Inconsistent UVF block at index " <<
              static_cast<unsigned>(iBlockIndex));
      return false;
    }

    LINFO("Transforming Vertices ...");
    std::vector<float> vertices  = block->GetVertices();
    if (vertices.size() % 3) {
      LERROR("Inconsistent data vertex in UVF block at index " <<
              static_cast<unsigned>(iBlockIndex));
      return false;
    }
    for (size_t i = 0;i<vertices.size();i+=3) {
      Core::Math::Vec3f v = (Core::Math::Vec4f(vertices[i+0], vertices[i+1],
                                     vertices[i+2],1)*m).xyz();
      vertices[i+0] = v.x;
      vertices[i+1] = v.y;
      vertices[i+2] = v.z;
    }
    block->SetVertices(vertices);

    LINFO("Transforming Normals ...");
    Core::Math::Mat4f invTranspose(m);
    invTranspose = invTranspose.inverse();
    invTranspose = invTranspose.Transpose();

    std::vector< float > normals  = block->GetNormals();
    if (normals.size() % 3) {
      LERROR("Inconsistent normal data in UVF block at index " <<
              static_cast<unsigned>(iBlockIndex));
      return false;
    }
    for (size_t i = 0;i<normals.size();i+=3) {
      Core::Math::Vec3f n = (Core::Math::Vec4f(normals[i+0], normals[i+1],
                                     normals[i+2],0)*invTranspose).xyz();
      n.normalize();
      normals[i+0] = n.x;
      normals[i+1] = n.y;
      normals[i+2] = n.z;
    }
    block->SetNormals(normals);

    // also write default color
    std::vector<float> defaultColor { c.x, c.y, c.z, c.w };
    block->SetDefaultColor(defaultColor);

    LINFO("Writing changes to disk");
    Close();
    LINFO("Reopening in read-only mode");

    Open(false,false,false);
    return true;
  } catch(const TrinityError&) {
    LERROR("Readwrite mode failed, maybe file is write protected?");
    Open(false,false,false);
    return false;
  }
}

bool UVFDataset::RemoveMesh(size_t iMeshIndex) {
  Close();

  LINFO("Attempting to reopen file in read/write mode.");

  try {
    Open(false, true, false);
  } catch(const TrinityError&) {
    LERROR("Read/write mode failed, maybe file is write protected?");
    Open(false,false,false);
    return false;
  }
  LINFO("Successfully reopened file in readwrite mode.");

  // turn meshindex into block index, those are different as the
  // uvf file most likely also contains data other than meshes
  // such as the volume or histograms, etc.
  size_t iBlockIndex = 0;
  bool bFound = false;

  for(size_t block=0; block < m_pDatasetFile->GetDataBlockCount(); ++block) {
    if (m_pDatasetFile->GetDataBlock(block)->GetBlockSemantic()
                                                == UVFTables::BS_GEOMETRY) {
      if (iMeshIndex == 0) {
        iBlockIndex = block;
        bFound = true;
        break;
      }
      iMeshIndex--;
    }
  }

  if (!bFound) {
    LERROR("Unable to locate mesh data block " <<
            static_cast<unsigned>(iMeshIndex));
    return false;
  }

  bool bResult = m_pDatasetFile->DropBlockFromFile(iBlockIndex);

  LINFO("Writing changes to disk");
  Close();
  LINFO("Reopening in read-only mode");

  Open(false,false,false);
  return bResult;
}

bool UVFDataset::AppendMesh(std::shared_ptr<const Mesh> meshIn) {
  const Mesh& m = *meshIn;

  Close();

  LINFO("Attempting to reopen file in readwrite mode.");

  try {
    Open(false,true,false);
  } catch(const TrinityError&) {
    LERROR("Read/write mode failed, maybe file is write protected?");
    Open(false,false,false);
    return false;
  }
  LINFO("Successfully reopened file in readwrite mode.");

  // now create a GeometryDataBlock ...
  std::shared_ptr<GeometryDataBlock> tsb(new GeometryDataBlock());

  // ... and transfer the data from the mesh object
  // source data
  const VertVec&      v = m.GetVertices();
  const NormVec&      n = m.GetNormals();
  const TexCoordVec&  t = m.GetTexCoords();
  const ColorVec&     c = m.GetColors();

  // target data
  vector<float> fVec;
  size_t iVerticesPerPoly = m.GetVerticesPerPoly();
  tsb->SetPolySize(iVerticesPerPoly);

  if (v.size()) {
    fVec.resize(v.size()*3);
    size_t index = 0;
    for (auto e : v) {
      fVec[index++] = e.x;
      fVec[index++] = e.y;
      fVec[index++] = e.z;
    }
    tsb->SetVertices(fVec);
  } else {
    // even if the vectors are empty still let the datablock know
    tsb->SetVertices(vector<float>());
  }

  if (n.size()) {
    fVec.resize(n.size()*3);
    size_t index = 0;
    for (auto e : n) {
      fVec[index++] = e.x;
      fVec[index++] = e.y;
      fVec[index++] = e.z;
    }
    tsb->SetNormals(fVec);
  } else {
    // even if the vectors are empty still let the datablock know
    tsb->SetNormals(vector<float>());
  }
  if (t.size()) {
    fVec.resize(t.size()*2);
    size_t index = 0;
    for (auto e : t) {
      fVec[index++] = e.x;
      fVec[index++] = e.y;
    }
    tsb->SetTexCoords(fVec);
  } else {
    // even if the vectors are empty still let the datablock know
    tsb->SetTexCoords(vector<float>());
  }
  if (c.size()) {
    fVec.resize(c.size()*4);
    size_t index = 0;
    for (auto e : c) {
      fVec[index++] = e.x;
      fVec[index++] = e.y;
      fVec[index++] = e.z;
      fVec[index++] = e.w;
    }
    tsb->SetColors(fVec);
  } else {
    // even if the vectors are empty still let the datablock know
    tsb->SetColors(vector<float>());
  }

  tsb->SetVertexIndices(m.GetVertexIndices());
  tsb->SetNormalIndices(m.GetNormalIndices());
  tsb->SetTexCoordIndices(m.GetTexCoordIndices());
  tsb->SetColorIndices(m.GetColorIndices());

  tsb->m_Desc = m.Name();

  m_pDatasetFile->AppendBlockToFile(tsb);

  LINFO("Writing changes to disk");
  Close();
  LINFO("Reopening in read-only mode");

  Open(false,false,false);
  return true;
}

bool UVFDataset::Crop(const Core::Math::PLANE<float>& plane, const std::string& strTempDir,
                      bool bKeepOldData, bool bUseMedianFilter, bool bClampToEdge,
                      uint64_t incoreSize)
{
  /*
  
  LINFO("Flattening dataset");
  string strTempRawFilename = SysTools::FindNextSequenceName(
    strTempDir + "crop-tmp.raw"
  );
  Export(0, strTempRawFilename , false);

  LINFO("Cropping at plane (%g %g %g %g)", plane.x, plane.y, plane.z,
                                             plane.w);
  Core::Math::Mat4f m;
  m.Scaling(Core::Math::Vec3f(GetScale()/GetScale().maxVal()) *
            Core::Math::Vec3f(GetDomainSize()) /float(GetDomainSize().maxVal()));
  PLANE<float> scaleInvariantPlane = plane;
  scaleInvariantPlane.transformIT(m);

  TempFile dataFile(strTempRawFilename);
  if (!dataFile.Open(true)) {
    LERROR("Unable to open flattened data.");
    return false;
  }

  // crop data
  bool bCroppingOK = false;
  switch (GetBitWidth()) {
    case 8:
      if(GetIsSigned()) {
        bCroppingOK = CropData<int8_t>(dataFile, scaleInvariantPlane,
                                       GetDomainSize(), GetComponentCount(),
                                       incoreSize);
      } else {
        bCroppingOK = CropData<uint8_t>(dataFile, scaleInvariantPlane,
                                        GetDomainSize(), GetComponentCount(),
                                        incoreSize);
      }
      break;
    case 16 :
      if(GetIsSigned()) {
        bCroppingOK = CropData<int16_t>(dataFile, scaleInvariantPlane,
                                        GetDomainSize(), GetComponentCount(),
                                        incoreSize);
      } else {
        bCroppingOK = CropData<uint16_t>(dataFile, scaleInvariantPlane,
                                         GetDomainSize(), GetComponentCount(),
                                         incoreSize);
      }
      break;
    case 32 :
      if (GetIsFloat()) {
        bCroppingOK = CropData<float>(dataFile, scaleInvariantPlane,
                                      GetDomainSize(), GetComponentCount(),
                                      incoreSize);
      } else {
        if(GetIsSigned()) {
          bCroppingOK = CropData<int32_t>(dataFile, scaleInvariantPlane,
                                          GetDomainSize(), GetComponentCount(),
                                          incoreSize);
        } else {
          bCroppingOK = CropData<uint32_t>(dataFile, scaleInvariantPlane,
                                           GetDomainSize(), GetComponentCount(),
                                           incoreSize);
        }
      }
      break;
    case 64 :
      if (GetIsFloat()) {
        bCroppingOK = CropData<double>(dataFile, scaleInvariantPlane,
                                       GetDomainSize(), GetComponentCount(),
                                       incoreSize);
      } else {
        if(GetIsSigned()) {
          bCroppingOK = CropData<int64_t>(dataFile, scaleInvariantPlane,
                                          GetDomainSize(), GetComponentCount(),
                                          incoreSize);
        } else {
          bCroppingOK = CropData<uint64_t>(dataFile, scaleInvariantPlane,
                                           GetDomainSize(), GetComponentCount(),
                                           incoreSize);
        }
      }
      break;
  }

  if (!bCroppingOK)  {
    return false;
  }

  // TODO shrink volume to the largest non-zero aabb
  dataFile.Close();

  LINFO("Rebuilding UVF data");
  string strTempFilename = SysTools::FindNextSequenceName(Filename());

  std::string strDesc = std::string("Cropped ") + std::string(Name());
  std::string strSource = SysTools::GetFilename(Filename());

  if(!RAWConverter::ConvertRAWDataset(
      strTempRawFilename, strTempFilename, strTempDir, 0, GetBitWidth(),
      size_t(GetComponentCount()), 1, !IsSameEndianness(), GetIsSigned(),
      GetIsFloat(), GetDomainSize(), Core::Math::Vec3f(GetScale()), strDesc,
      strSource, Controller::Instance().IOMan()->GetMaxBrickSize(),
      Controller::Instance().IOMan()->GetBrickOverlap(), bUseMedianFilter,
      bClampToEdge,
      1, // compress with zlib
      4, // use default compression level for LZMA
      0 // default scanline layout
    )) {
    LERROR("Unable to convert cropped data back to UVF");
    return false;
  }

  LINFO("Replacing original UVF by the new one");
  Close();

  if (bKeepOldData) {
    std::string newFilename = SysTools::AppendFilename(Filename(),"-beforeCropping");
    if (SysTools::FileExists(newFilename)) {
      newFilename = SysTools::FindNextSequenceName(newFilename);
    }
    rename(Filename().c_str(), newFilename.c_str());
  } else {
    remove(Filename().c_str());
  }


  if (SysTools::FileExists(Filename())) {
    LERROR("Unable to delete original UVF file, a new file ("<<strTempFilename.c_str()<<") has been "
            "created alongside the old.");
    Open(false,false,false);
    return false;
  }

  rename(strTempFilename.c_str(), Filename().c_str());

  LINFO("Opening new file");
  Open(false,false,false);

   */
  return true;
}


bool UVFDataset::SaveRescaleFactors() {
  Core::Math::Vec3d saveUserScale = m_UserScale;
  Close();

  LINFO("Attempting to reopen file in readwrite mode.");

  try {
    Open(false,true,false);
  } catch(const TrinityError&) {
    LERROR("Read/write mode failed, maybe file is write protected?");
    Open(false,false,false);
    return false;
  }
  LINFO("Successfully reopened file in readwrite mode.");

  if (m_bToCBlock) {
    for(size_t tsi=0; tsi < m_timesteps.size(); ++tsi) {
      TOCBlock* tocb =
        static_cast<TOCBlock*>(
          m_pDatasetFile->GetDataBlockRW(m_timesteps[tsi]->block_number, true)
        );
      tocb->SetScale(saveUserScale);
    }
  } else {
    for(size_t tsi=0; tsi < m_timesteps.size(); ++tsi) {
      RasterDataBlock* rdb =
        static_cast<RasterDataBlock*>(
          m_pDatasetFile->GetDataBlockRW(m_timesteps[tsi]->block_number, true)
        );

      size_t iSize = rdb->ulDomainSize.size();
      for (size_t i=0; i < 3; i++) {
        m_DomainScale[i] = saveUserScale[i];
        // matrix multiplication with scale factors
        rdb->dDomainTransformation[0+(iSize+1)*i] *= saveUserScale[0];
        rdb->dDomainTransformation[1+(iSize+1)*i] *= saveUserScale[1];
        rdb->dDomainTransformation[2+(iSize+1)*i] *= saveUserScale[2];
      }
    }
  }

  LINFO("Writing changes to disk");
  Close();
  LINFO("Reopening in read-only mode");
  Open(false,false,false);

  return true;
}

bool UVFDataset::CanRead(const std::string&,
                         const std::vector<int8_t>& bytes) const
{
  return bytes[0] == 0x55 && // 'U'
         bytes[1] == 0x56 && // 'V'
         bytes[2] == 0x46 && // 'F'
         bytes[3] == 0x2D && // '-'
         bytes[4] == 0x44 && // 'D'
         bytes[5] == 0x41 && // 'A'
         bytes[6] == 0x54 && // 'T'
         bytes[7] == 0x41;   // 'A'
}

bool UVFDataset::Verify(const std::string& filename) const
{
  std::wstring wstrFilename(filename.begin(), filename.end());
  bool checksumFail=false;
  UVF::IsUVFFile(wstrFilename, checksumFail);
  // negate it; IsUVFFile sets the argument if the checksum *fails*!
  return !checksumFail;
}

Dataset* UVFDataset::Create(const std::string& filename,
                            uint64_t max_brick_size, bool verify) const
{
  return new UVFDataset(filename, max_brick_size, verify, false);
}

std::list<std::string> UVFDataset::Extensions() const
{
  std::list<std::string> retval;
  retval.push_back("UVF");
  return retval;
}

template <class T> bool
UVFDataset::GetBrickTemplate(const BrickKey& k, std::vector<T>& vData) const
{
  if(m_bToCBlock) {
    const Core::Math::Vec4ui64 coords = KeyToTOCVector(k);
    const TOCTimestep* ts = static_cast<TOCTimestep*>(
      m_timesteps[k.timestep]
    );
    const size_t targetSize = size_t(
      ts->GetDB()->GetComponentTypeSize() *
      ts->GetDB()->GetComponentCount() *
      ts->GetDB()->GetBrickSize(coords).volume()
    ) / sizeof(T);
    vData.resize(targetSize);
    uint8_t* pData = (uint8_t*)&vData[0];
    ts->GetDB()->GetData(pData,coords);
    if(ts->GetDB()->GetAtlasSize(coords).area() != 0) {
      VolumeTools::DeAtalasify(targetSize, ts->GetDB()->GetAtlasSize(coords),
                               ts->GetDB()->GetMaxBrickSize(),
                               ts->GetDB()->GetBrickSize(coords), pData,
                               pData);
    }
    return true;
  } else {
    const NDBrickKey& key = this->IndexToVectorKey(k);
    const RDTimestep* ts = static_cast<RDTimestep*>(m_timesteps[key.timestep]);
    return ts->GetDB()->GetData(vData, key.lod, key.brick);
  }
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<uint8_t>& vData) const {
  return GetBrickTemplate<uint8_t>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<int8_t>& vData) const {
  return GetBrickTemplate<int8_t>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<uint16_t>& vData) const {
  return GetBrickTemplate<uint16_t>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<int16_t>& vData) const {
  return GetBrickTemplate<int16_t>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<uint32_t>& vData) const {
  return GetBrickTemplate<uint32_t>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<int32_t>& vData) const {
  return GetBrickTemplate<int32_t>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<float>& vData) const {
  return GetBrickTemplate<float>(k,vData);
}

bool UVFDataset::GetBrick(const BrickKey& k, std::vector<double>& vData) const {
  return GetBrickTemplate<double>(k,vData);
}

std::pair<Core::Math::Vec3f, Core::Math::Vec3f> UVFDataset::GetTextCoords(BrickTable::const_iterator brick, bool bUseOnlyPowerOfTwo) const {
  if (m_bToCBlock) {
    const Core::Math::Vec4ui64 coords = KeyToTOCVector(brick->first);
    const TOCBlock* tb = static_cast<TOCTimestep*>(m_timesteps[brick->first.timestep])->GetDB();

    uint32_t iOverlap = tb->GetOverlap();
    Core::Math::Vec3f brickAspect = Core::Math::Vec3f(tb->GetBrickAspect(coords));
    Core::Math::Vec3f vTexcoordsMin;
    Core::Math::Vec3f vTexcoordsMax;

    if (bUseOnlyPowerOfTwo) {
      Core::Math::Vec3ui vRealVoxelCount(Core::Math::MathTools::nextPow2(brick->second.n_voxels.x),
        Core::Math::MathTools::nextPow2(brick->second.n_voxels.y),
        Core::Math::MathTools::nextPow2(brick->second.n_voxels.z)
        );
      vTexcoordsMin = float(iOverlap) / Core::Math::Vec3f(vRealVoxelCount);
      vTexcoordsMax = 1.0f-vTexcoordsMin;
      vTexcoordsMax -= Core::Math::Vec3f(vRealVoxelCount - brick->second.n_voxels) / Core::Math::Vec3f(vRealVoxelCount);
    } else {
      vTexcoordsMin = float(iOverlap) / Core::Math::Vec3f(brick->second.n_voxels);
      vTexcoordsMax = 1.0f-vTexcoordsMin;
    }

    return std::make_pair(vTexcoordsMin, vTexcoordsMax*brickAspect);
  } else {
    return Dataset::GetTextCoords(brick, bUseOnlyPowerOfTwo);
  }
}
