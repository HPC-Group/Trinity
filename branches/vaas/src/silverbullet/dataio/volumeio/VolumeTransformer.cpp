#include "silverbullet/dataio/volumeio/VolumeTransformer.h"

#include <sstream>
#include <limits>
#include "silverbullet/math/Vectors.h"

using namespace Core::Math;

namespace DataIO {
  namespace VolumeIO {
    
    template <typename T>
    static void compSequMinMax(VolumePtr v, double& min, double& max) {
      const Vec3ui64& s = v->getMetadata()->getSize();
      const uint32_t iCompCount = v->getMetadata()->getComponents();

      T* data = (T*)v->getContinousMemoryPointer();
      uint64_t count = s.volume()*iCompCount;
      
      for (uint64_t p = 0; p<count;++p) {
        T val = *data;
        max = std::max(max, double(val));
        min = std::min(min, double(val));
        data++;
      }
      
    }
    
    template <typename T>
    static void compRamMinMax(VolumePtr v, double& min, double& max) {
      const Vec3ui64& s = v->getMetadata()->getSize();
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      
      for (uint64_t z = 0; z<s.z;++z) {
        for (uint64_t y = 0; y<s.y ;++y) {
          for (uint64_t x = 0; x<s.x ;++x) {
            for (uint32_t c = 0; c<iCompCount ;++c) {
              T val = v->getValue<T>(Vec3ui64(x,y,z),c);
              max = std::max(max, double(val));
              min = std::min(min, double(val));
            }
          }
        }
      }
    }

    template <typename T>
    static void compMinMax(VolumePtr v, double& min, double& max) {
      if (v->supportsContinousMemoryPointer())
        compSequMinMax<T>(v, min, max);
      else
        compRamMinMax<T>(v, min, max);
    }
    
    
    template <typename T, typename U>
    static void quantizeSequ(VolumePtr v, VolumePtr w, double offset,
                             double ratio, double scale) {
      const Vec3ui64& s = v->getMetadata()->getSize();
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      
      T* dataIn = (T*)v->getContinousMemoryPointer();
      U* dataOut = (U*)w->getContinousMemoryPointer();
      
      uint64_t count = s.volume()*iCompCount;
      
      for (uint64_t p = 0; p<count;++p) {
        double val = double(*dataIn);
        val = ((val-offset)/ratio)*scale;
        *dataOut = T(val);

        dataIn++;
        dataOut++;
      }
      
    }
    
    template <typename T, typename U>
    static void quantizeRam(VolumePtr v, VolumePtr w, double offset,
                            double ratio, double scale,
                            const Vec3ui64& iOffset, const Vec3ui64& oOffset,
                            const Vec3ui64& processSize) {

      const uint32_t iCompCount = v->getMetadata()->getComponents();
      
      for (uint64_t z = 0; z<processSize.z;++z) {
        for (uint64_t y = 0; y<processSize.y ;++y) {
          for (uint64_t x = 0; x<processSize.x ;++x) {
            for (uint32_t c = 0; c<iCompCount ;++c) {
              double val = double(v->getValue<T>(iOffset+Vec3ui64(x,y,z),c));
              val = ((val-offset)/ratio)*scale;
              w->setValue<U>(oOffset+Vec3ui64(x,y,z),c, U(val));
            }
          }
        }
      }
    }
    
    template <typename T, typename U>
    static void quantize(VolumePtr v, VolumePtr w, double offset,
                                 double ratio, double scale,
                                 const Vec3ui64& iOffset,
                                 const Vec3ui64& oOffset,
                                 const Vec3ui64& processSize) {
      if (v->supportsContinousMemoryPointer() &&
          w->supportsContinousMemoryPointer() &&
          iOffset == Vec3ui64(0,0,0) &&
          oOffset == Vec3ui64(0,0,0) &&
          v->getMetadata()->getSize() == w->getMetadata()->getSize() &&
          processSize == w->getMetadata()->getSize())
        quantizeSequ<T,U>(v, w, offset, ratio, scale);
      else
        quantizeRam<T,U>(v, w, offset, ratio, scale, iOffset,
                         oOffset, processSize);
    }
    
    template <typename T>
    static void copySequ(VolumePtr v, VolumePtr w) {
      const Vec3ui64& s = v->getMetadata()->getSize();
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      uint64_t count = s.volume()*iCompCount;
      
      T* dataIn = (T*)v->getContinousMemoryPointer();
      T* dataOut = (T*)w->getContinousMemoryPointer();
      
      std::copy ( dataIn, dataIn+count, dataOut);
    }
    
    template <typename T>
    static void copyRam(VolumePtr v, VolumePtr w,
                        const Vec3ui64& iOffset, const Vec3ui64& oOffset,
                        const Vec3ui64& processSize) {
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      
      for (uint64_t z = 0; z<processSize.z;++z) {
        for (uint64_t y = 0; y<processSize.y ;++y) {
          for (uint64_t x = 0; x<processSize.x ;++x) {
            for (uint32_t c = 0; c<iCompCount ;++c) {
              T val = v->getValue<T>(iOffset+Vec3ui64(x,y,z),c);
              w->setValue<T>(oOffset+Vec3ui64(x,y,z),c, val);
            }
          }
        }
      }
    }
    
    
    template <typename T>
    static void copyVolume(VolumePtr v, VolumePtr w,
                           const Vec3ui64& iOffset, const Vec3ui64& oOffset,
                           const Vec3ui64& processSize ) {
      if (v->supportsContinousMemoryPointer() &&
          w->supportsContinousMemoryPointer() &&
          iOffset == Vec3ui64(0,0,0) &&
          oOffset == Vec3ui64(0,0,0) &&
          v->getMetadata()->getSize() == w->getMetadata()->getSize() &&
          processSize == w->getMetadata()->getSize())
        copySequ<T>(v, w);
      else
        copyRam<T>(v, w, iOffset, oOffset, processSize);
    }

    template <typename T>
    static void fillSequ(VolumePtr v, T value) {
      const Vec3ui64& s = v->getMetadata()->getSize();
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      uint64_t count = s.volume()*iCompCount;
      T* data = (T*)v->getContinousMemoryPointer();
      std::fill ( data, data+count, value );
    }
    
    template <typename T>
    static void fillRam(VolumePtr v, T value) {
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      const Vec3ui64& s = v->getMetadata()->getSize();
      
      for (uint64_t z = 0; z<s.z;++z) {
        for (uint64_t y = 0; y<s.y ;++y) {
          for (uint64_t x = 0; x<s.x ;++x) {
            for (uint32_t c = 0; c<iCompCount ;++c) {
              v->setValue<T>(Vec3ui64(x,y,z),c, value);
            }
          }
        }
      }
    }
    
    template <typename T>
    static void fillVolume(VolumePtr v, T value) {
      if (v->supportsContinousMemoryPointer())
        fillSequ<T>(v,value);
      else
        fillRam<T>(v,value);
    }
    
    template <typename T>
    static void resample(VolumePtr v, VolumePtr w,
                         const Vec3ui64& region,
                         const Vec3d& stepping) {
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      const Vec3ui64& iSize = v->getMetadata()->getSize();
      const Vec3ui64& oSize = w->getMetadata()->getSize();
      const double regionSize = double(region.volume());
      
      for (uint64_t z = 0; z<oSize.z;++z) {
        for (uint64_t y = 0; y<oSize.y ;++y) {
          for (uint64_t x = 0; x<oSize.x ;++x) {
            for (uint32_t c = 0; c<iCompCount ;++c) {
              
              double acc = 0;
              Vec3ui64 base = Vec3ui64(Vec3d(x,y,z)*stepping);
              base.StoreMin(iSize-1);
              
              for (uint64_t rz = 0; rz<region.z;++rz) {
                for (uint64_t ry = 0; ry<region.y ;++ry) {
                  for (uint64_t rx = 0; rx<region.x ;++rx) {
                    const Vec3ui64 pos = base + Vec3ui64(rx,ry,rz);
                    acc += double(v->getValue<T>(pos,c));
                  }
                }
              }
              
              w->setValue<T>(Vec3ui64(x,y,z),c, T(acc/regionSize));
            }
          }
        }
      }
    }
    
    template <typename T, typename U>
    static void quantizeResampleTempl(VolumePtr v, VolumePtr w, double offset,
                                      double ratio, double scale,
                                      const Vec3ui64& region,
                                      const Vec3d& stepping) {
      const uint32_t iCompCount = v->getMetadata()->getComponents();
      const Vec3ui64& iSize = v->getMetadata()->getSize();
      const Vec3ui64& oSize = w->getMetadata()->getSize();
      const double regionSize = double(region.volume());
      
      for (uint64_t z = 0; z<oSize.z;++z) {
        for (uint64_t y = 0; y<oSize.y ;++y) {
          for (uint64_t x = 0; x<oSize.x ;++x) {
            for (uint32_t c = 0; c<iCompCount ;++c) {
              
              double acc = 0;
              Vec3ui64 base = Vec3ui64(Vec3d(x,y,z)*stepping);
              base.StoreMin(iSize-1);
              
              for (uint64_t rz = 0; rz<region.z;++rz) {
                for (uint64_t ry = 0; ry<region.y ;++ry) {
                  for (uint64_t rx = 0; rx<region.x ;++rx) {
                    const Vec3ui64 pos = base + Vec3ui64(rx,ry,rz);
                    acc += double(v->getValue<T>(pos,c));
                  }
                }
              }
              
              double val = (((acc/regionSize)-offset)/ratio)*scale;
              
              w->setValue<T>(Vec3ui64(x,y,z),c, T(val));
            }
          }
        }
      }
    }
    
    
    void VolumeTransformer::transform(VolumePtr inputVolume,
                                    VolumePtr outputVolume,
                                    const ResizeParams& rp) {
      assert(inputVolume);
      assert(outputVolume);
      if (inputVolume == outputVolume) {
        reporter("Input and output are the same. Done!", -1);
        return;
      }

      /*
      std::stringstream ss;
      ss << "Transforming\n Input Volume: " << *(inputVolume)
      << "\nOutput Volume: " << *(outputVolume);
      reporter(ss.str(), -1);
      */

      if (inputVolume->getMetadata()->getComponents() !=
          outputVolume->getMetadata()->getComponents()) {
        std::stringstream error;
        error << "changing the component count ist not impemented yet";
        // TODO: implement this
        throw VolumeError(error.str());
      }
      
      const DataType& iDT = inputVolume->getMetadata()->getType();
      const DataType& oDT = outputVolume->getMetadata()->getType();
      
      double offset=0, ratio=1;
      double max=-1*std::numeric_limits<double>::max();
      double min=std::numeric_limits<double>::max();
      double maxScale = 1;
      
      if (iDT != oDT && !oDT.m_float) {
        reporter("Need to Quantize, computing max/min", -1);
        switch (iDT.getTypeID() ) {
          case DataType::TN_FLOAT   :
            compMinMax<float>(inputVolume, min, max);
            break;
          case DataType::TN_DOUBLE  :
            compMinMax<double>(inputVolume, min, max);
            break;
          case DataType::TN_UINT64  :
            compMinMax<uint64_t>(inputVolume, min, max);
            break;
          case DataType::TN_UINT32  :
            compMinMax<uint32_t>(inputVolume, min, max);
            break;
          case DataType::TN_UINT16  :
            compMinMax<uint16_t>(inputVolume, min, max);
            break;
          case DataType::TN_UINT8   :
            compMinMax<uint8_t>(inputVolume, min, max);
            break;
          case DataType::TN_INT64   :
            compMinMax<int64_t>(inputVolume, min, max);
            break;
          case DataType::TN_INT32   :
            compMinMax<int32_t>(inputVolume, min, max);
            break;
          case DataType::TN_INT16   :
            compMinMax<int16_t>(inputVolume, min, max);
            break;
          case DataType::TN_INT8    :
            compMinMax<int8_t>(inputVolume, min, max);
            break;
          case DataType::TN_UNKNOWN : {
            std::stringstream error;
            error << "invalid data format ("<< iDT << ") in input volume, can't quantize";
            throw VolumeError(error.str());
          }
        }
        
        offset = min;

        // if the file contains only one value -> avoid singularity
        // OR
        // if input is no float type and the shifted max still fits
        // into the target range -> do nothing
        if (max == min ||
            (!iDT.m_float && max-min <= oDT.getMax()) ) {
          ratio = 1;
          maxScale = 1;
        } else {
          ratio = max-min;
          
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              maxScale = 1;
              break;
            case DataType::TN_DOUBLE  :
              maxScale = 1;
              break;
            case DataType::TN_UINT64  :
              maxScale = std::numeric_limits<uint64_t>::max();
              break;
            case DataType::TN_UINT32  :
              maxScale = std::numeric_limits<uint32_t>::max();
              break;
            case DataType::TN_UINT16  :
              maxScale = std::numeric_limits<uint16_t>::max();
              break;
            case DataType::TN_UINT8   :
              maxScale = std::numeric_limits<uint8_t>::max();
              break;
            case DataType::TN_INT64   :
              maxScale = double(std::numeric_limits<int64_t>::max());
              break;
            case DataType::TN_INT32   :
              maxScale = std::numeric_limits<int32_t>::max();
              break;
            case DataType::TN_INT16   :
              maxScale = std::numeric_limits<int16_t>::max();
              break;
            case DataType::TN_INT8    :
              maxScale = std::numeric_limits<int8_t>::max();
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("
              << oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
          
        }
        
      }
      
      const Vec3ui64& iSize = inputVolume->getMetadata()->getSize();
      const Vec3ui64& oSize = outputVolume->getMetadata()->getSize();

      if (iSize == oSize) {
        std::stringstream ss;
        ss << "Input and output size are identical quantize/copy only ("  << offset << " " << ratio << " " << maxScale<< ")";
        reporter(ss.str(), -1);
        quantizeOrCopy(inputVolume, outputVolume, offset, ratio, maxScale,
                       Vec3ui64(0,0,0), Vec3ui64(0,0,0), oSize);
      } else {
        if (rp.resizeMethod == ResizeParams::RM_PadCrop) {
          reporter("Input and output size are not identical,"
                   " quantize/copy + crop/pad.", -1);
          
         
          // padding at least one dimension?
          if (oSize.x > iSize.x ||
              oSize.y > iSize.y ||
              oSize.z > iSize.z) {
            std::stringstream ss;
            ss << "Filling padded area with "  << rp.padValue;
            reporter(ss.str(), -1);
            
            // TODO: improve performance, instead of filling
            //       the entire volume we only need to fill
            //       the parts that are bigger than the input
            //       everything else is overriden anyhow            
            switch (oDT.getTypeID() ) {
              case DataType::TN_FLOAT   :
                fillVolume<float>(outputVolume, float(rp.padValue));
                break;
              case DataType::TN_DOUBLE  :
                fillVolume<double>(outputVolume, rp.padValue);
                break;
              case DataType::TN_UINT64  :
                fillVolume<uint64_t>(outputVolume, uint64_t(rp.padValue));
                break;
              case DataType::TN_UINT32  :
                fillVolume<uint32_t>(outputVolume, uint32_t(rp.padValue));
                break;
              case DataType::TN_UINT16  :
                fillVolume<uint16_t>(outputVolume, uint16_t(rp.padValue));
                break;
              case DataType::TN_UINT8   :
                fillVolume<uint8_t>(outputVolume, uint8_t(rp.padValue));
                break;
              case DataType::TN_INT64   :
                fillVolume<int64_t>(outputVolume, int64_t(rp.padValue));
                break;
              case DataType::TN_INT32   :
                fillVolume<int32_t>(outputVolume, int32_t(rp.padValue));
                break;
              case DataType::TN_INT16   :
                fillVolume<int16_t>(outputVolume, int16_t(rp.padValue));
                break;
              case DataType::TN_INT8    :
                fillVolume<int8_t>(outputVolume, int8_t(rp.padValue));
                break;
              case DataType::TN_UNKNOWN : {
                std::stringstream error;
                error << "invalid output volume data format (" << oDT
                << "), can't fill";
                throw VolumeError(error.str());
              }
            }
          }
          
          
          Vec3ui64 size, iPos, oPos;
          
          if (iSize.x <= oSize.x) {
            // padding
            iPos.x = 0;
            oPos.x = std::min(rp.pos.x, oSize.x);
            size.x = std::min(iSize.x, oSize.x-oPos.x);
          } else {
            // cropping
            iPos.x = std::min(rp.pos.x, iSize.x);
            oPos.x = 0;
            size.x = std::min(oSize.x, iSize.x-iPos.x);
          }

          if (iSize.y <= oSize.y) {
            // padding
            iPos.y = 0;
            oPos.y = std::min(rp.pos.y, oSize.y);
            size.y = std::min(iSize.y, oSize.y-oPos.y);
          } else {
            // cropping
            iPos.y = std::min(rp.pos.y, iSize.y);
            oPos.y = 0;
            size.y = std::min(oSize.y, iSize.y-iPos.y);
          }

          if (iSize.z <= oSize.z) {
            // padding
            iPos.z = 0;
            oPos.z = std::min(rp.pos.z, oSize.z);
            size.z = std::min(iSize.z, oSize.z-oPos.z);
          } else {
            // cropping
            iPos.z = std::min(rp.pos.z, iSize.z);
            oPos.z = 0;
            size.z = std::min(oSize.z, iSize.z-iPos.z);
          }
          
          if (size.volume() == 0) {
            reporter("Crop/Pad region is empty, done!", -1);
          } else {
            std::stringstream ss;
            ss << "Transferring (" << size << ") brick of values to position "
            << oPos << " reading at offset " << iPos;
            reporter(ss.str(), -1);
            
            quantizeOrCopy(inputVolume, outputVolume,
                           offset, ratio, maxScale, iPos, oPos, size);
          }
          
        } else {
          reporter("Input and output size are not identical,"
                   " quantize/copy + resample.", -1);
          
          Vec3ui64 region = iSize/oSize;
          Vec3d stepping = Vec3d(iSize)/Vec3d(oSize);
          region.StoreMax(Vec3ui64(1,1,1));
          
          quantizeResample(inputVolume, outputVolume, offset, ratio,
                           maxScale, region, stepping);
        }
      }
      // TODO: do something useful to "used bits"
    }
    
    void VolumeTransformer::quantizeOrCopy(VolumePtr inputVolume,
                                         VolumePtr outputVolume,
                                         double offset, double ratio,
                                         double maxScale,
                                         const Vec3ui64& iOffset,
                                         const Vec3ui64& oOffset,
                                         const Vec3ui64& processSize) {
      const DataType& iDT = inputVolume->getMetadata()->getType();
      const DataType& oDT = outputVolume->getMetadata()->getType();

      switch (iDT.getTypeID() ) {
        case DataType::TN_FLOAT   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              copyVolume<float>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<float,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<float,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<float,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<float,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<float,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<float,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<float,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<float,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<float,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_DOUBLE  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<double,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              copyVolume<double>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<double,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<double,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<double,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<double,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<double,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<double,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<double,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<double,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT64  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<uint64_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<uint64_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              copyVolume<uint64_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<uint64_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<uint64_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<uint64_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<uint64_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<uint64_t,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<uint64_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<uint64_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT32  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<uint32_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<uint32_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<uint32_t,uint64_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              copyVolume<uint32_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<uint32_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<uint32_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<uint32_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<uint32_t,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<uint32_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<uint32_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT16  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<uint16_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<uint16_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<uint16_t,uint64_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<uint16_t,uint16_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              copyVolume<uint16_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<uint16_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<uint16_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<uint16_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<uint16_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<uint16_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT8   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<uint8_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<uint8_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<uint8_t,uint64_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<uint8_t,uint16_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<uint8_t,uint16_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              copyVolume<uint8_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<uint8_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<uint8_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<uint8_t,int16_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<uint8_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT64   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<int64_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<int64_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<int64_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<int64_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<int64_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<int64_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              copyVolume<int64_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<int64_t,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<int64_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<int64_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT32   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<int32_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<int32_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<int32_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<int32_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<int32_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<int32_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<int32_t,int64_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              copyVolume<int32_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<int32_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<int32_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT16   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<int16_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<int16_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<int16_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<int16_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<int16_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<int16_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<int16_t,int64_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<int16_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              copyVolume<int16_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              quantize<int16_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT8    : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantize<int8_t,float>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_DOUBLE  :
              quantize<int8_t,double>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT64  :
              quantize<int8_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT32  :
              quantize<int8_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT16  :
              quantize<int8_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UINT8   :
              quantize<int8_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT64   :
              quantize<int8_t,int64_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT32   :
              quantize<int8_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT16   :
              quantize<int8_t,int16_t>(inputVolume, outputVolume, 0, 1, 1, iOffset, oOffset, processSize);
              break;
            case DataType::TN_INT8    :
              copyVolume<int8_t>(inputVolume, outputVolume, iOffset, oOffset, processSize);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UNKNOWN : {
          std::stringstream error;
          error << "invalid input volume data format ("<< iDT << "), can't quantize";
          throw VolumeError(error.str());
        }
      }
    }
    
    void VolumeTransformer::quantizeResample(VolumePtr inputVolume,
                                           VolumePtr outputVolume,
                                           double offset, double ratio,
                                           double maxScale,
                                           const Vec3ui64& region,
                                           const Vec3d& stepping) {
      const DataType& iDT = inputVolume->getMetadata()->getType();
      const DataType& oDT = outputVolume->getMetadata()->getType();
      
      switch (iDT.getTypeID() ) {
        case DataType::TN_FLOAT   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              resample<float>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<float,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<float,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<float,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<float,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<float,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<float,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<float,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<float,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<float,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_DOUBLE  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<double,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              resample<double>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<double,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<double,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<double,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<double,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<double,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<double,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<double,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<double,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT64  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<uint64_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<uint64_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              resample<uint64_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<uint64_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<uint64_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<uint64_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<uint64_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<uint64_t,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<uint64_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<uint64_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT32  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<uint32_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<uint32_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<uint32_t,uint64_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT32  :
              resample<uint32_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<uint32_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<uint32_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<uint32_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<uint32_t,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<uint32_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<uint32_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT16  : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<uint16_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<uint16_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<uint16_t,uint64_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<uint16_t,uint16_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT16  :
              resample<uint16_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<uint16_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<uint16_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<uint16_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<uint16_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<uint16_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UINT8   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<uint8_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<uint8_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<uint8_t,uint64_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<uint8_t,uint16_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<uint8_t,uint16_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT8   :
              resample<uint8_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<uint8_t,int64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<uint8_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<uint8_t,int16_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<uint8_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT64   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<int64_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<int64_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<int64_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<int64_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<int64_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<int64_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              resample<int64_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<int64_t,int32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<int64_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<int64_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT32   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<int32_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<int32_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<int32_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<int32_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<int32_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<int32_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<int32_t,int64_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT32   :
              resample<int32_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<int32_t,int16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<int32_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT16   : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<int16_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<int16_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<int16_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<int16_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<int16_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<int16_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<int16_t,int64_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<int16_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT16   :
              resample<int16_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_INT8    :
              quantizeResampleTempl<int16_t,int8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("<< oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_INT8    : {
          switch (oDT.getTypeID() ) {
            case DataType::TN_FLOAT   :
              quantizeResampleTempl<int8_t,float>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_DOUBLE  :
              quantizeResampleTempl<int8_t,double>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_UINT64  :
              quantizeResampleTempl<int8_t,uint64_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT32  :
              quantizeResampleTempl<int8_t,uint32_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT16  :
              quantizeResampleTempl<int8_t,uint16_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_UINT8   :
              quantizeResampleTempl<int8_t,uint8_t>(inputVolume, outputVolume, offset, ratio, maxScale, region, stepping);
              break;
            case DataType::TN_INT64   :
              quantizeResampleTempl<int8_t,int64_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT32   :
              quantizeResampleTempl<int8_t,int32_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT16   :
              quantizeResampleTempl<int8_t,int16_t>(inputVolume, outputVolume, 0, 1, 1, region, stepping);
              break;
            case DataType::TN_INT8    :
              resample<int8_t>(inputVolume, outputVolume, region, stepping);
              break;
            case DataType::TN_UNKNOWN : {
              std::stringstream error;
              error << "invalid output volume data format ("
              << oDT << "), can't quantize & resample";
              throw VolumeError(error.str());
            }
          }
        } break;
        case DataType::TN_UNKNOWN : {
          std::stringstream error;
          error << "invalid input volume data format ("
          << iDT << "), can't quantize & resample";
          throw VolumeError(error.str());
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
