#include "QVisVolumeMetadata.h"

#include <sstream>
#include <fstream>
#include "VolumeError.h"

#include <core/StringTools.h>
#include <core/IO/FileTools.h>
#include <core/IO/KeyValueFileParser.h>

using namespace Core::IO;

namespace DataIO {
  namespace VolumeIO {

    QVisVolumeMetadata::QVisVolumeMetadata(const std::string& datFilename) :
      VolumeMetadata(),
      m_rawFilename("")
    {
      parseDatfile(datFilename);
    }

    QVisVolumeMetadata::QVisVolumeMetadata(const VolumeMetadata& data,
                                          const std::string& rawFilename) :
      VolumeMetadata(data),
      m_rawFilename(rawFilename)
    {
    }

    const std::string QVisVolumeMetadata::toString() const {
      std::stringstream result;
      result << "raw file: " << m_rawFilename << ", " << VolumeMetadata::toString();
      return result.str();
    }

    void QVisVolumeMetadata::parseDatfile(const std::string& datFilename) {
      m_rawFilename = "";
      m_size = Core::Math::Vec3ui64(0,0,0);
      m_dataType = DataType();
      m_usedbits = m_dataType.m_bytes*8;
      m_components = 1;
      m_aspect = Core::Math::Vec3d(1,1,1);
      m_transformation = Core::Math::Mat4d();


      KeyValueFileParser parser(datFilename);

      if (parser.FileReadable())  {
        KeyValPair* format = parser.GetData("FORMAT");
        if (format == NULL) {
          std::stringstream error;
          error << "parsing invalid QVis file"<< datFilename <<", file does not contain any useful data";
          throw VolumeError(error.str());
        } else {
          // The "CHAR" here is correct; QVis cannot store signed 8bit data.
          if(format->strValueUpper == "CHAR" ||
             format->strValueUpper == "UCHAR" ||
             format->strValueUpper == "BYTE") {
            m_dataType = DataType(false, false, 1);
            m_components = 1;
          } else if (format->strValueUpper == "SHORT") {
            m_dataType = DataType(true, false, 2);
            m_components = 1;
          } else if (format->strValueUpper == "USHORT") {
            m_dataType = DataType(false, false, 2);
            m_components = 1;
          } else if (format->strValueUpper == "UINT") {
            m_dataType = DataType(false, false, 4);
            m_components = 1;
          } else if (format->strValueUpper == "UINT64") {
            m_dataType = DataType(false, false, 8);
            m_components = 1;
          } else if (format->strValueUpper == "INT") {
            m_dataType = DataType(true, false, 4);
            m_components = 1;
          } else if (format->strValueUpper == "INT64") {
            m_dataType = DataType(true, false, 8);
            m_components = 1;
          } else if (format->strValueUpper == "FLOAT") {
            m_dataType = DataType(true, true, 4);
            m_components = 1;
          } else if (format->strValueUpper == "DOUBLE") {
            m_dataType = DataType(true, true, 8);
            m_components = 1;
          } else if (format->strValueUpper == "UCHAR4") {
            m_dataType = DataType(false, false, 1);
            m_components = 4;
          } else if (format->strValueUpper == "UCHAR3") {
            m_dataType = DataType(false, false, 1);
            m_components = 3;
          } else if(format->strValueUpper == "USHORT3") {
            m_dataType = DataType(false, false, 2);
            m_components = 3;
          } else if(format->strValueUpper == "USHORT4") {
            m_dataType = DataType(false, false, 2);
            m_components = 4;
          }
        }

        KeyValPair* objectfilename = parser.GetData("OBJECTFILENAME");
        if (objectfilename == NULL) {
          std::stringstream error;
          error << "parsing invalid QVis file "<< datFilename <<", OBJECTFILENAME absent";
          throw VolumeError(error.str());
        } else {
          m_rawFilename = objectfilename->strValue;
        }
        KeyValPair* resolution = parser.GetData("RESOLUTION");
        if (resolution == NULL || resolution->vuiValue.size() != 3) {
          std::stringstream error;
          error << "parsing invalid QVis file "<< datFilename <<", RESOLUTION malformed or absent";
          throw VolumeError(error.str());
        } else {
          m_size = Core::Math::Vec3ui64(resolution->vuiValue);
        }

        KeyValPair* endianess = parser.GetData("ENDIANESS");
        if (endianess && endianess->strValueUpper == "BIG") {
          std::stringstream error;
          error << "unable to load big endian files";
          throw VolumeError(error.str());
        }


        KeyValPair* sliceThickness = parser.GetData("SLICETHICKNESS");
        if (sliceThickness == NULL || sliceThickness->vuiValue.size() != 3) {
          std::stringstream error;
          error << "parsing invalid QVis file"<< datFilename <<", SLICETHICKNESS malformed or absent";
          throw VolumeError(error.str());
        } else {
          m_aspect = Core::Math::Vec3d(sliceThickness->vfValue);
          m_aspect = m_aspect / m_aspect.maxVal();
        }

      } else {
        std::stringstream error;
        error << "unable to open volume file " << datFilename;
        throw VolumeError(error.str());
      }

      m_usedbits = m_dataType.m_bytes*8;  // QVis does not contain any information about how many bits are actually used
      m_rawFilename = FileTools::GetPath(datFilename) + m_rawFilename;
    }

    void QVisVolumeMetadata::save(const std::string& datFilename) {
      std::ofstream file(datFilename);

      if (!file.is_open()) {
        std::stringstream error;
        error << "unable to open volume file " << datFilename;
        throw VolumeError(error.str());
      }

      std::string relName = Core::IO::FileTools::Abs2RelPath(datFilename, m_rawFilename);
      Core::StringTools::ReplaceAll(relName, "\\", "/"); // be nice to linux :-)


      file << "ObjectFileName:\t" << relName << std::endl;


      file << "Resolution:\t" << m_size.x << " " << m_size.y << " " << m_size.z << std::endl;
      file << "SliceThickness:\t" << m_aspect.x << " " << m_aspect.y << " " << m_aspect.z << std::endl;
      file << "TaggedFileName:\t---" << std::endl;


      if (m_dataType.m_bytes != 1 && m_dataType.m_bytes != 2 && m_dataType.m_bytes != 4 && m_dataType.m_bytes != 8) {
        std::stringstream error;
        error << "invalid qvis format: " << m_dataType.m_bytes*8 << " bit data not supported";
        throw VolumeError(error.str());
      }

      if (m_dataType.m_float) {
        if (m_components != 1) {
          std::stringstream error;
          error << "invalid qvis format: float with more than one component";
          throw VolumeError(error.str());
        }
        switch (m_dataType.m_bytes) {
          case 4 : file << "Format:\tFLOAT" << std::endl; break;
          case 8 : file << "Format:\tDOUBLE" << std::endl; break;
          default : {
            std::stringstream error;
            error << "invalid qvis format: float with "<< m_dataType.m_bytes*8<< " bit";
            throw VolumeError(error.str());
          }
        }
      } else {
        if (m_components != 1 && m_components != 3 && m_components != 4) {
          std::stringstream error;
          error << "invalid qvis format: " << m_components << " component data";
          throw VolumeError(error.str());
        }

        if (m_components != 1 && m_dataType.m_bytes > 2) {
          std::stringstream error;
          error << "invalid qvis format: " << m_components
                << " component data only supported for byte and short";
          throw VolumeError(error.str());
        }

        std::string postfix = "";
        if (m_components == 3 || m_components == 4) {
          postfix = Core::StringTools::ToString(m_components);
        }

        if (m_dataType.m_signed) {
          switch (m_dataType.m_bytes) {
            case 1 : file << "Format:\tCHAR" << postfix << std::endl; break;
            case 2 : file << "Format:\tSHORT" << postfix << std::endl; break;
            case 4 : file << "Format:\tINT" << std::endl; break;
            case 8 : file << "Format:\tINT64" << std::endl; break;
          }
        } else {
          switch (m_dataType.m_bytes) {
            case 1 : file << "Format:\tUCHAR" << postfix << std::endl; break;
            case 2 : file << "Format:\tUSHORT" << postfix << std::endl; break;
            case 4 : file << "Format:\tUINT" << std::endl; break;
            case 8 : file << "Format:\tUINT64" << std::endl; break;
          }
        }
      }


      file << "ObjectType:\tTEXTURE_VOLUME_OBJECT" << std::endl;
      file << "ObjectModel:\tRGBA" << std::endl;
      file << "GridType:\tEQUIDISTANT" << std::endl;
      file << "Endianness:\tLITTLE" << std::endl;
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
