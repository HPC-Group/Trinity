#ifndef TEXTFILETFMAPPER_H
#define TEXTFILETFMAPPER_H

#include <fstream>

#include <SilverBulletBase.h>
#include "Mapper.h"

namespace DataIO {
  namespace VolumeIO {
    class TextfileTFMapper : public Mapper {
    public:
      
      virtual uint32_t getComponents() const {return 4;}
      
      size_t getSize() const {return m_pvColorData.size();}
      
      void premultiply() {
        for(size_t i=0;i<m_pvColorData.size();++i){
          m_pvColorData[i][0] *= m_pvColorData[i][3];
          m_pvColorData[i][1] *= m_pvColorData[i][3];
          m_pvColorData[i][2] *= m_pvColorData[i][3];
        }
      }

      void resample(size_t iTargetSize) {
        size_t iSourceSize = m_pvColorData.size();
        
        if (iTargetSize == iSourceSize) return;
        
        std::vector<std::vector<double>> vTmpColorData(iTargetSize);
        
        if (iTargetSize < iSourceSize) {
          // downsample
          size_t iFrom = 0;
          for (size_t i = 0;i<vTmpColorData.size();i++) {
            vTmpColorData[i].resize(4);
            
            size_t iTo = iFrom + iSourceSize/iTargetSize;
            for(size_t c=0;c<4;++c){
              vTmpColorData[i][c] = 0;
            }
            for (size_t j = iFrom;j<iTo;j++) {
              for(size_t c=0;c<4;++c){
                vTmpColorData[i][c] += m_pvColorData[j][c];
              }
            }
            for(size_t c=0;c<4;++c){
              vTmpColorData[i][c] /= double(iTo-iFrom);
            }
            
            
            iTargetSize -= 1;
            iSourceSize -= iTo-iFrom;
            
            iFrom = iTo;
          }
        } else {
          // upsample
          for (size_t i = 0;i<vTmpColorData.size();i++) {
            vTmpColorData[i].resize(4);
            double fPos = double(i) * double(iSourceSize-1)/double(iTargetSize);
            size_t iFloor = size_t(floor(fPos));
            size_t iCeil  = std::min(iFloor+1, m_pvColorData.size()-1);
            double fInterp = fPos - double(iFloor);
            
            for(size_t c=0;c<4;++c){
              vTmpColorData[i][c] = m_pvColorData[iFloor][c] * (1-fInterp) + m_pvColorData[iCeil][c] * fInterp;
            }
          }
          
        }
        
        m_pvColorData = vTmpColorData;
      }
      
    protected:
      bool loadStream(std::ifstream& tf) {
        for(size_t i=0;i<m_pvColorData.size();++i){
          m_pvColorData[i].resize(4);
          for(size_t j=0;j<4;++j){
            tf >> m_pvColorData[i][j];
          }
        }
        return tf.good();
      }
      
      std::vector<std::vector<double>> m_pvColorData;
      
      virtual std::vector<double> mapUint16Value(uint16_t v){
        assert(v<m_pvColorData.size());
        return m_pvColorData[v];
      }
      
      virtual std::vector<double> mapUint8Value(uint8_t v){
        assert(v<m_pvColorData.size());
        return m_pvColorData[v];
      }
    };
    
    typedef std::shared_ptr<TextfileTFMapper> TextfileTFMapperPtr;
  }
}

#endif // TEXTFILETFMAPPER_H

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
