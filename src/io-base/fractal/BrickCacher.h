#pragma once

#include <fstream>
#include <vector>
#include <string>
#include "silverbullet/io/FileTools.h"
#include "silverbullet/dataio/base/Brick.h"

class BrickCacher {
public:
  BrickCacher(const std::string& dir,
              const std::string& prefix,
              const std::string& ext = "brickCache");
  
  void clear() const;
  
  template <typename T>
  bool getMaxMin(const BrickKey& brickKey,
                 T& min, T& max) const {
    std::string name = genFilename(brickKey);
    if (!Core::IO::FileTools::fileExists(name)) {
      return false;
    } else {
      std::ifstream brickfile (name, std::ios::binary);
      if (!brickfile.is_open()) {
        return false;
      }
      brickfile.read((char*)&min, sizeof(T));
      brickfile.read((char*)&max, sizeof(T));
      brickfile.close();
      return true;
    }
  }
  
  template <typename T>
  bool getBrick(const BrickKey& brickKey,
                std::vector<uint8_t>& data) const {
    
    std::string name = genFilename(brickKey);
    if (!Core::IO::FileTools::fileExists(name)) {
      return false;
    } else {
      std::ifstream brickfile (name, std::ios::binary);
      if (!brickfile.is_open()) {
        return false;
      }
      T min, max;

      brickfile.seekg (0, brickfile.end);
      size_t length = brickfile.tellg();
      brickfile.seekg (0, brickfile.beg);
      data.resize(length-2*sizeof(T));
      
      brickfile.read((char*)&min, sizeof(T));
      brickfile.read((char*)&max, sizeof(T));
      brickfile.read((char*)data.data(), data.size());
      brickfile.close();
      return true;
    }
  }
  
  template <typename T>
  bool setBrick(const BrickKey& brickKey,
                const std::vector<uint8_t>& data) const {
    
    std::string name = genFilename(brickKey);
    if (Core::IO::FileTools::fileExists(name)) {
      return false;
    } else {
      std::ofstream brickfile (name, std::ios::binary);
      if (!brickfile.is_open()) {
        return false;
      }
      
      T* typedData = (T*)data.data();
      size_t typedLength = data.size() / sizeof(T);
      
      if (typedLength > 0) {
        T min = *typedData;
        T max = *typedData;
        
        for (size_t i = 1;i<typedLength;++i) {
          if (*typedData < min) min = *typedData;
          if (*typedData > max) max = *typedData;
          typedData++;
        }
        
        brickfile.write((char*)&min, sizeof(T));
        brickfile.write((char*)&max, sizeof(T));
        brickfile.write((char*)data.data(), data.size());
      }
      brickfile.close();
      
      return true;
    }
  }
  
private:
  std::string m_dir;
  std::string m_prefix;
  std::string m_ext;
  
  std::string genFilename(const BrickKey& brickKey) const;
};
