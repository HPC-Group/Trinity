#include "VolumeProcessor.h"

#include <sstream>
#include <limits>
#include <Core/Math/Vectors.h>
#include <Core/IO/FileTools.h>
#include "RawVolume.h"

using namespace Core::Math;
using namespace Core::IO::FileTools;

namespace DataIO {
  namespace VolumeIO {
    VolumeProcessor::VolumeProcessor(const std::string& tmpDir) :
    m_transformer(),
    m_tmpDir(tmpDir)
    {
      m_transformer.setParent(this);
    }

    void VolumeProcessor::map(VolumePtr i, VolumePtr o, MapperPtr mapper) {
      
      if (i->getMetadata()->getComponents() != 1) {
        std::stringstream error;
        error << "map currently only works with single component input data";
        throw VolumeError(error.str());
      }

      if (o->getMetadata()->getComponents() != mapper->getComponents()) {
        std::stringstream error;
        error << "mapper writes " << mapper->getComponents() << " but specified output has " <<  o->getMetadata()->getComponents() << " component";
        throw VolumeError(error.str());
      }
      
      const DataType& iDT = i->getMetadata()->getType();
      const DataType& oDT = o->getMetadata()->getType();
      
      const Vec3ui64& iSize = i->getMetadata()->getSize();
      const Vec3ui64& oSize = o->getMetadata()->getSize();
      
      if (oDT.getTypeID() != DataType::TN_DOUBLE || iSize != oSize) {
        // types don't match, create intermediate volume and transform later
        std::string tmpFile = FindNextSequenceName(AssembleFullFilename(m_tmpDir, "tmp.raw"));
        {
          VolumeMetadataPtr md = std::make_shared<VolumeMetadata>(iSize,
                                                                  DataType(true, true, 8),
                                                                  64,
                                                                  o->getMetadata()->getComponents(),
                                                                  o->getMetadata()->getAspectRatio(),
                                                                  o->getMetadata()->getTransformation());
          RawVolumePtr tempVol = std::make_shared<RawVolume>(tmpFile, md);
          tempVol->create();
          map(i, tempVol, mapper);
          ResizeParams rp(ResizeParams::RM_Resample, Vec3ui64(0,0,0), 0);
          m_transformer.transform(tempVol, o, rp);
        }
        remove(tmpFile.c_str());
        return;
      }
      
      switch (iDT.getTypeID() ) {
        case DataType::TN_FLOAT   :
          map<float>(i,o,mapper); break;
        case DataType::TN_DOUBLE  :
          map<double>(i,o,mapper); break;
        case DataType::TN_UINT64  :
          map<uint64_t>(i,o,mapper); break;
        case DataType::TN_UINT32  :
          map<uint32_t>(i,o,mapper); break;
        case DataType::TN_UINT16  :
          map<uint16_t>(i,o,mapper);break;
        case DataType::TN_UINT8   :
          map<uint8_t>(i,o,mapper); break;
        case DataType::TN_INT64   :
          map<int64_t>(i,o,mapper); break;
        case DataType::TN_INT32   :
          map<int32_t>(i,o,mapper); break;
        case DataType::TN_INT16   :
          map<int16_t>(i,o,mapper); break;
        case DataType::TN_INT8    :
          map<int8_t>(i,o,mapper); break;
        case DataType::TN_UNKNOWN : {
          std::stringstream error;
          error << "invalid data format ("<< iDT << ") in volume, can't compute mapping";
          throw VolumeError(error.str());
        }
      }
      
      
    }
    
    void VolumeProcessor::compute(VolumePtr i1, VolumePtr i2,
                                  VolumePtr o, VolOp op) {
      const DataType& i1DT = i1->getMetadata()->getType();
      const DataType& i2DT = i2->getMetadata()->getType();
      
      const Vec3ui64& iSize1 = i1->getMetadata()->getSize();
      const Vec3ui64& iSize2 = i2->getMetadata()->getSize();

      if (i1DT != i2DT || iSize1 != iSize2) {
        const DataType commonDT = DataType::getCommonSupertype(i1DT, i2DT);
        Vec3ui64 commonSize = iSize1;
        commonSize.StoreMax(iSize2);
        
        ResizeParams rp(ResizeParams::RM_PadCrop, Vec3ui64(0,0,0), 0);
        
        VolumePtr i1c = i1;
        std::string tmpFile1 = "";
        if (i1DT != commonDT || iSize1 != commonSize) {
          tmpFile1 = FindNextSequenceName(AssembleFullFilename(m_tmpDir, "tmp.raw"));
          RawVolumePtr tempVol = std::make_shared<RawVolume>(tmpFile1, std::make_shared<VolumeMetadata>(i1->getMetadata()));
          tempVol->create();
          m_transformer.transform(i1, tempVol, rp);
          i1c = tempVol;
        }
        VolumePtr i2c = i2;
        std::string tmpFile2 = "";
        if (i2DT != commonDT || iSize2 != commonSize) {
          tmpFile2 = FindNextSequenceName(AssembleFullFilename(m_tmpDir, "tmp.raw"));
          RawVolumePtr tempVol = std::make_shared<RawVolume>(tmpFile2, std::make_shared<VolumeMetadata>(i1->getMetadata()));
          tempVol->create();
          m_transformer.transform(i2, tempVol, rp);
          i2c = tempVol;
        }

        compute(i1c, i2c, o, op);
        
        if (!tmpFile1.empty()) remove(tmpFile1.c_str());
        if (!tmpFile2.empty()) remove(tmpFile2.c_str());
        return;
      }
      
      const DataType& oDT = o->getMetadata()->getType();
      const Vec3ui64& oSize = o->getMetadata()->getSize();
      
      if (i1DT != oDT || iSize1 != oSize) {
        // types don't match, create intermediate volume and transform later
        std::string tmpFile = FindNextSequenceName(AssembleFullFilename(m_tmpDir, "tmp.raw"));
        {
          RawVolumePtr tempVol = std::make_shared<RawVolume>(tmpFile, std::make_shared<VolumeMetadata>(i1->getMetadata()));
          tempVol->create();
          compute(i1, i2, tempVol, op);
          ResizeParams rp(ResizeParams::RM_Resample, Vec3ui64(0,0,0), 0);
          m_transformer.transform(tempVol, o, rp);
        }
        remove(tmpFile.c_str());
        return;
      }
            
      switch (op) {
        case VO_add : {
          switch (i1DT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeV<float,VO_add>(i1,i2,o); break;
            case DataType::TN_DOUBLE  :
              computeV<double,VO_add>(i1,i2,o); break;
            case DataType::TN_UINT64  :
              computeV<uint64_t,VO_add>(i1,i2,o); break;
            case DataType::TN_UINT32  :
              computeV<uint32_t,VO_add>(i1,i2,o); break;
            case DataType::TN_UINT16  :
              computeV<uint16_t,VO_add>(i1,i2,o);break;
            case DataType::TN_UINT8   :
              computeV<uint8_t,VO_add>(i1,i2,o); break;
            case DataType::TN_INT64   :
              computeV<int64_t,VO_add>(i1,i2,o); break;
            case DataType::TN_INT32   :
              computeV<int32_t,VO_add>(i1,i2,o); break;
            case DataType::TN_INT16   :
              computeV<int16_t,VO_add>(i1,i2,o); break;
            case DataType::TN_INT8    :
              computeV<int8_t,VO_add>(i1,i2,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< i1DT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case VO_sub : {
          switch (i1DT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeV<float,VO_sub>(i1,i2,o); break;
            case DataType::TN_DOUBLE  :
              computeV<double,VO_sub>(i1,i2,o); break;
            case DataType::TN_UINT64  :
              computeV<uint64_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_UINT32  :
              computeV<uint32_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_UINT16  :
              computeV<uint16_t,VO_sub>(i1,i2,o);break;
            case DataType::TN_UINT8   :
              computeV<uint8_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_INT64   :
              computeV<int64_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_INT32   :
              computeV<int32_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_INT16   :
              computeV<int16_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_INT8    :
              computeV<int8_t,VO_sub>(i1,i2,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< i1DT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case VO_mul : {
          switch (i1DT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeV<float,VO_mul>(i1,i2,o); break;
            case DataType::TN_DOUBLE  :
              computeV<double,VO_mul>(i1,i2,o); break;
            case DataType::TN_UINT64  :
              computeV<uint64_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_UINT32  :
              computeV<uint32_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_UINT16  :
              computeV<uint16_t,VO_mul>(i1,i2,o);break;
            case DataType::TN_UINT8   :
              computeV<uint8_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_INT64   :
              computeV<int64_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_INT32   :
              computeV<int32_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_INT16   :
              computeV<int16_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_INT8    :
              computeV<int8_t,VO_mul>(i1,i2,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< i1DT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case VO_div : {
          switch (i1DT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeV<float,VO_div>(i1,i2,o); break;
            case DataType::TN_DOUBLE  :
              computeV<double,VO_div>(i1,i2,o); break;
            case DataType::TN_UINT64  :
              computeV<uint64_t,VO_div>(i1,i2,o); break;
            case DataType::TN_UINT32  :
              computeV<uint32_t,VO_div>(i1,i2,o); break;
            case DataType::TN_UINT16  :
              computeV<uint16_t,VO_div>(i1,i2,o);break;
            case DataType::TN_UINT8   :
              computeV<uint8_t,VO_div>(i1,i2,o); break;
            case DataType::TN_INT64   :
              computeV<int64_t,VO_div>(i1,i2,o); break;
            case DataType::TN_INT32   :
              computeV<int32_t,VO_div>(i1,i2,o); break;
            case DataType::TN_INT16   :
              computeV<int16_t,VO_div>(i1,i2,o); break;
            case DataType::TN_INT8    :
              computeV<int8_t,VO_div>(i1,i2,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< i1DT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case VO_max : {
          switch (i1DT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeV<float,VO_max>(i1,i2,o); break;
            case DataType::TN_DOUBLE  :
              computeV<double,VO_max>(i1,i2,o); break;
            case DataType::TN_UINT64  :
              computeV<uint64_t,VO_max>(i1,i2,o); break;
            case DataType::TN_UINT32  :
              computeV<uint32_t,VO_max>(i1,i2,o); break;
            case DataType::TN_UINT16  :
              computeV<uint16_t,VO_max>(i1,i2,o);break;
            case DataType::TN_UINT8   :
              computeV<uint8_t,VO_max>(i1,i2,o); break;
            case DataType::TN_INT64   :
              computeV<int64_t,VO_max>(i1,i2,o); break;
            case DataType::TN_INT32   :
              computeV<int32_t,VO_max>(i1,i2,o); break;
            case DataType::TN_INT16   :
              computeV<int16_t,VO_max>(i1,i2,o); break;
            case DataType::TN_INT8    :
              computeV<int8_t,VO_max>(i1,i2,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< i1DT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case VO_min : {
          switch (i1DT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeV<float,VO_min>(i1,i2,o); break;
            case DataType::TN_DOUBLE  :
              computeV<double,VO_min>(i1,i2,o); break;
            case DataType::TN_UINT64  :
              computeV<uint64_t,VO_min>(i1,i2,o); break;
            case DataType::TN_UINT32  :
              computeV<uint32_t,VO_min>(i1,i2,o); break;
            case DataType::TN_UINT16  :
              computeV<uint16_t,VO_min>(i1,i2,o);break;
            case DataType::TN_UINT8   :
              computeV<uint8_t,VO_min>(i1,i2,o); break;
            case DataType::TN_INT64   :
              computeV<int64_t,VO_min>(i1,i2,o); break;
            case DataType::TN_INT32   :
              computeV<int32_t,VO_min>(i1,i2,o); break;
            case DataType::TN_INT16   :
              computeV<int16_t,VO_min>(i1,i2,o); break;
            case DataType::TN_INT8    :
              computeV<int8_t,VO_min>(i1,i2,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< i1DT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
      }
      
    }
    
    void VolumeProcessor::compute(VolumePtr i, double s,
                                  VolumePtr o, ScalarOp op) {
      const DataType& iDT = i->getMetadata()->getType();
      const DataType& oDT = o->getMetadata()->getType();
      
      const Vec3ui64& iSize = i->getMetadata()->getSize();
      const Vec3ui64& oSize = o->getMetadata()->getSize();
      
      if (iDT != oDT || iSize != oSize) {
        // types don't match, create intermediate volume and transform later
        std::string tmpFile = FindNextSequenceName(AssembleFullFilename(m_tmpDir, "tmp.raw"));
        {
          RawVolumePtr tempVol = std::make_shared<RawVolume>(tmpFile, std::make_shared<VolumeMetadata>(i->getMetadata()));
          tempVol->create();
          compute(i, s, tempVol, op);
          ResizeParams rp(ResizeParams::RM_Resample, Vec3ui64(0,0,0), 0);
          m_transformer.transform(tempVol, o, rp);
        }
        remove(tmpFile.c_str());
        return;
      }

      switch (op) {
        case SO_add : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_add>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_add>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_add>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_add>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_add>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_add>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_add>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_add>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_add>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_add>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case SO_sub : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_sub>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_sub>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_sub>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_sub>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_sub>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_sub>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_sub>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_sub>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_sub>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_sub>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case SO_mul : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_mul>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_mul>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_mul>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_mul>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_mul>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_mul>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_mul>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_mul>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_mul>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_mul>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case SO_div : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_div>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_div>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_div>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_div>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_div>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_div>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_div>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_div>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_div>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_div>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case SO_max : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_max>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_max>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_max>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_max>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_max>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_max>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_max>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_max>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_max>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_max>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case SO_min : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_min>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_min>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_min>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_min>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_min>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_min>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_min>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_min>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_min>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_min>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
        case SO_pow : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              computeS<float,SO_pow>(i,s,o); break;
            case DataType::TN_DOUBLE  :
              computeS<double,SO_pow>(i,s,o); break;
            case DataType::TN_UINT64  :
              computeS<uint64_t,SO_pow>(i,s,o); break;
            case DataType::TN_UINT32  :
              computeS<uint32_t,SO_pow>(i,s,o); break;
            case DataType::TN_UINT16  :
              computeS<uint16_t,SO_pow>(i,s,o);break;
            case DataType::TN_UINT8   :
              computeS<uint8_t,SO_pow>(i,s,o); break;
            case DataType::TN_INT64   :
              computeS<int64_t,SO_pow>(i,s,o); break;
            case DataType::TN_INT32   :
              computeS<int32_t,SO_pow>(i,s,o); break;
            case DataType::TN_INT16   :
              computeS<int16_t,SO_pow>(i,s,o); break;
            case DataType::TN_INT8    :
              computeS<int8_t,SO_pow>(i,s,o); break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't perform computation";
              throw VolumeError(error.str());
            }
          }
        } break;
      }
    }
    
    double VolumeProcessor::reduce(VolumePtr v, uint32_t component, ReduceOp op) {
      const DataType& iDT = v->getMetadata()->getType();
      
      switch (op) {
        case RO_add : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              return reduce<float,RO_add>(v, component);
            case DataType::TN_DOUBLE  :
              return reduce<double,RO_add>(v, component);
            case DataType::TN_UINT64  :
              return reduce<uint64_t,RO_add>(v, component);
            case DataType::TN_UINT32  :
              return reduce<uint32_t,RO_add>(v, component);
            case DataType::TN_UINT16  :
              return reduce<uint16_t,RO_add>(v, component);
            case DataType::TN_UINT8   :
              return reduce<uint8_t,RO_add>(v, component);
            case DataType::TN_INT64   :
              return reduce<int64_t,RO_add>(v, component);
            case DataType::TN_INT32   :
              return reduce<int32_t,RO_add>(v, component);
            case DataType::TN_INT16   :
              return reduce<int16_t,RO_add>(v, component);
            case DataType::TN_INT8    :
              return reduce<int8_t,RO_add>(v, component);
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't reduce";
              throw VolumeError(error.str());
            }
          }
        }
        case RO_max : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              return reduce<float,RO_max>(v, component);
            case DataType::TN_DOUBLE  :
              return reduce<double,RO_max>(v, component);
            case DataType::TN_UINT64  :
              return reduce<uint64_t,RO_max>(v, component);
            case DataType::TN_UINT32  :
              return reduce<uint32_t,RO_max>(v, component);
            case DataType::TN_UINT16  :
              return reduce<uint16_t,RO_max>(v, component);
            case DataType::TN_UINT8   :
              return reduce<uint8_t,RO_max>(v, component);
            case DataType::TN_INT64   :
              return reduce<int64_t,RO_max>(v, component);
            case DataType::TN_INT32   :
              return reduce<int32_t,RO_max>(v, component);
            case DataType::TN_INT16   :
              return reduce<int16_t,RO_max>(v, component);
            case DataType::TN_INT8    :
              return reduce<int8_t,RO_max>(v, component);
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't reduce";
              throw VolumeError(error.str());
            }
          }
        }
        case RO_min : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              return reduce<float,RO_min>(v, component);
            case DataType::TN_DOUBLE  :
              return reduce<double,RO_min>(v, component);
            case DataType::TN_UINT64  :
              return reduce<uint64_t,RO_min>(v, component);
            case DataType::TN_UINT32  :
              return reduce<uint32_t,RO_min>(v, component);
            case DataType::TN_UINT16  :
              return reduce<uint16_t,RO_min>(v, component);
            case DataType::TN_UINT8   :
              return reduce<uint8_t,RO_min>(v, component);
            case DataType::TN_INT64   :
              return reduce<int64_t,RO_min>(v, component);
            case DataType::TN_INT32   :
              return reduce<int32_t,RO_min>(v, component);
            case DataType::TN_INT16   :
              return reduce<int16_t,RO_min>(v, component);
            case DataType::TN_INT8    :
              return reduce<int8_t,RO_min>(v, component);
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't reduce";
              throw VolumeError(error.str());
            }
          }
        }
        case RO_avg : {
          switch (iDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              return reduce<float,RO_avg>(v, component);
            case DataType::TN_DOUBLE  :
              return reduce<double,RO_avg>(v, component);
            case DataType::TN_UINT64  :
              return reduce<uint64_t,RO_avg>(v, component);
            case DataType::TN_UINT32  :
              return reduce<uint32_t,RO_avg>(v, component);
            case DataType::TN_UINT16  :
              return reduce<uint16_t,RO_avg>(v, component);
            case DataType::TN_UINT8   :
              return reduce<uint8_t,RO_avg>(v, component);
            case DataType::TN_INT64   :
              return reduce<int64_t,RO_avg>(v, component);
            case DataType::TN_INT32   :
              return reduce<int32_t,RO_avg>(v, component);
            case DataType::TN_INT16   :
              return reduce<int16_t,RO_avg>(v, component);
            case DataType::TN_INT8    :
              return reduce<int8_t,RO_avg>(v, component);
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid data format ("<< iDT << ") in volume, can't reduce";
              throw VolumeError(error.str());
            }
          }
        }
      }
    }
  
  }
}
/*
 The MIT License
 
 Copyright (c) 2014 HPC Group, Univeristy Duisburg-Essen
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
 */
