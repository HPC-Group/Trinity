#ifndef VOLUMEPROCESSOR_H
#define VOLUMEPROCESSOR_H

#include <SilverBulletBase.h>
#include "VolumeTransformer.h"
#include "Mapper.h"

namespace DataIO {
  namespace VolumeIO {
    
    class VolumeProcessor;
    typedef std::shared_ptr<VolumeProcessor> VolumeProcessorPtr;
    
    class VolumeProcessor  {
    public:
      VolumeProcessor(const std::string& tmpDir);
      
      enum VolOp {
        VO_add,
        VO_sub,
        VO_mul,
        VO_div,
        VO_max,
        VO_min
      };

      enum ScalarOp {
        SO_add,
        SO_sub,
        SO_mul,
        SO_div,
        SO_max,
        SO_min,
        SO_pow
      };
      
      enum ReduceOp {
        RO_add,
        RO_max,
        RO_min,
        RO_avg
      };
      
      void transform(VolumePtr inputVolume, VolumePtr outputVolume,
                     const ResizeParams& rp = ResizeParams()) {
        m_transformer.transform(inputVolume, outputVolume, rp);
      }
      void compute(VolumePtr i1, VolumePtr i2, VolumePtr o, VolOp op);
      void compute(VolumePtr i1, double s, VolumePtr o, ScalarOp op);
      double reduce(VolumePtr v, uint32_t component, ReduceOp op);
      void map(VolumePtr i, VolumePtr o, MapperPtr mapper);
      
    protected:

      // create a nested class of the transformer, that simply
      // forwards all reports to the volumeprocessor's reporter
      class MsgVolumeTransformer : public VolumeTransformer {
      public:
        MsgVolumeTransformer(VolumeProcessor* parent=nullptr) :
        VolumeTransformer(),
        m_parent(parent)
        {
        }

        void setParent(VolumeProcessor* parent) {m_parent = parent;}
      protected:
        VolumeProcessor* m_parent;
        virtual void reporter(const std::string& desc, float progress){
          m_parent->reporter(desc, progress);
        }

      };
      
      template <typename T>
      void map(VolumePtr i, VolumePtr o, MapperPtr mapper) {
        const Core::Math::Vec3ui64& iSize = i->getMetadata()->getSize();
        
        for (uint64_t z = 0; z<iSize.z;++z) {
          reporter("Applying map", float(z)/(iSize.z-1));
          for (uint64_t y = 0; y<iSize.y ;++y) {
            for (uint64_t x = 0; x<iSize.x ;++x) {
              T val = i->getValue<T>(Core::Math::Vec3ui64(x,y,z),0);
              const std::vector<double> result = mapper->map<T>(val);
              for (uint32_t c = 0; c<result.size() ;++c) {
                o->setValue<double>(Core::Math::Vec3ui64(x,y,z),c,result[c]);
              }
            }
          }
        }
      }
      
      template <typename T, VolOp op>
      void computeV(VolumePtr i1, VolumePtr i2, VolumePtr o) {
        const Core::Math::Vec3ui64& iSize = i1->getMetadata()->getSize();
        const uint32_t iCompCount = i1->getMetadata()->getComponents();
        
        for (uint64_t z = 0; z<iSize.z;++z) {
          reporter("Executing volume computation", float(z)/(iSize.z-1));
          for (uint64_t y = 0; y<iSize.y ;++y) {
            for (uint64_t x = 0; x<iSize.x ;++x) {
              for (uint32_t c = 0; c<iCompCount ;++c) {
                T val1 = i1->getValue<T>(Core::Math::Vec3ui64(x,y,z),c);
                T val2 = i2->getValue<T>(Core::Math::Vec3ui64(x,y,z),c);
                T result = T(0);
                switch (op) {
                  case VO_add : result = val1+val2; break;
                  case VO_sub : result = val1-val2; break;
                  case VO_mul : result = val1*val2; break;
                  case VO_div : result = val1/val2; break;
                  case VO_max : result = std::max(val1, val2); break;
                  case VO_min : result = std::min(val1, val2); break;
                }
                
                o->setValue<T>(Core::Math::Vec3ui64(x,y,z),c, result);
              }
            }
          }
        }
      }

      template <typename T, ScalarOp op>
      void computeS(VolumePtr i1, double s, VolumePtr o) {
        const Core::Math::Vec3ui64& iSize = i1->getMetadata()->getSize();
        const uint32_t iCompCount = i1->getMetadata()->getComponents();
        
        for (uint64_t z = 0; z<iSize.z;++z) {
          reporter("Executing scalar computation", float(z)/(iSize.z-1));
          for (uint64_t y = 0; y<iSize.y ;++y) {
            for (uint64_t x = 0; x<iSize.x ;++x) {
              for (uint32_t c = 0; c<iCompCount ;++c) {
                T val1 = i1->getValue<T>(Core::Math::Vec3ui64(x,y,z),c);
                T result = T(0);
                switch (op) {
                  case SO_add : result = val1+T(s); break;
                  case SO_sub : result = val1-T(s); break;
                  case SO_mul : result = val1*T(s); break;
                  case SO_div : result = val1/T(s); break;
                  case SO_max : result = std::max(val1, T(s)); break;
                  case SO_min : result = std::min(val1, T(s)); break;
                  case SO_pow : result = T(pow(double(val1), s)); break;
                }
                
                o->setValue<T>(Core::Math::Vec3ui64(x,y,z),c, result);
              }
            }
          }
        }
      }
      
      template <typename T, ReduceOp op>
      double reduce(VolumePtr v, uint32_t component) {
        const Core::Math::Vec3ui64& iSize = v->getMetadata()->getSize();
        double result;
        switch (op) {
          case RO_add : result = 0; break;
          case RO_max : result = -1*std::numeric_limits<double>::max(); break;
          case RO_min : result = std::numeric_limits<double>::max(); break;
          case RO_avg : result = 0; break;
        }
        
        double count = double(iSize.volume());
        
        for (uint64_t z = 0; z<iSize.z;++z) {
          reporter("Executing reduce", float(z)/(iSize.z-1));
          for (uint64_t y = 0; y<iSize.y ;++y) {
            for (uint64_t x = 0; x<iSize.x ;++x) {
              double val = double(v->getValue<T>(Core::Math::Vec3ui64(x,y,z),component));
              switch (op) {
                case RO_add : result += val; break;
                case RO_max : result = std::max(result, val); break;
                case RO_min : result = std::min(result, val); break;
                case RO_avg : result += val/count; break;
              }
            }
          }
        }
        return result;
      }

      
      MsgVolumeTransformer m_transformer;
      std::string m_tmpDir;
      
      virtual void reporter(const std::string& desc, float progress) {}
      friend class MsgVolumeTransformer;
    };
  }
}

#endif // VOLUMEPROCESSOR_H

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
