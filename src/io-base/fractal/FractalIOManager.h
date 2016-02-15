#pragma once

#include <IOManager.h>
#include <FractalIO.h>

/**
 OLD FILE - use it only as an inspiration
 */

namespace Trinity {
  namespace IO {
    
    class FractalIOManager : public IOManager {
    public:
      virtual std::shared_ptr<IOInterface> openFile(uint32_t id) const {
        switch (id) {
          case 3 : return std::make_shared<FractalIO>(64,64,64,64,64,64);
          case 4 : return std::make_shared<FractalIO>(128,128,128,128,128,128);
          case 5 : return std::make_shared<FractalIO>(256,256,256,256,256,256);
          case 6 : return std::make_shared<FractalIO>(512,512,512,32,32,32);
          case 7 : return std::make_shared<FractalIO>(1024,1024,1024,32,32,32);
          case 8 : return std::make_shared<FractalIO>(2048,2048,2048,32,32,32);
          case 9 : return std::make_shared<FractalIO>(4096,4096,4096,32,32,32);
          default : throw IDNotFoundException(); break;
        }
      }
      
      virtual DataList listData(uint32_t dirID = 0) const {
        DataList l;
        switch (dirID) {
          case 0 : {
            l.push_back(DataID("Flat Data", 1, DataID::Directory));
            l.push_back(DataID("Bricked Data", 2, DataID::Directory));
            break;
          }
          case 1 : {
            l.push_back(DataID("64^3",  3, DataID::Dataset));
            l.push_back(DataID("128^3", 4, DataID::Dataset));
            l.push_back(DataID("256^3", 5, DataID::Dataset));
            break;
          }
          case 2 : {
            l.push_back(DataID("512^3 (32^3 bricks)",  6, DataID::Dataset));
            l.push_back(DataID("1024^3 (32^3 bricks)", 7, DataID::Dataset));
            l.push_back(DataID("2048^3 (32^3 bricks)", 8, DataID::Dataset));
            l.push_back(DataID("4096^3 (32^3 bricks)", 9, DataID::Dataset));
            break;
          }
          default : throw IDNotFoundException(); break;
        }
        return l;
      }
    };
    
  }
}
