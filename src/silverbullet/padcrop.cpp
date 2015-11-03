#include <SilverBulletBase.h>
#include <Core/IO/FileTools.h>
#include <Core/StringTools.h>
#include <DataIO/VolumeIO/VolumeError.h>
#include <DataIO/VolumeIO/QVisVolume.h>
#include <DataIO/VolumeIO/VolumeProcessor.h>

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // cout
#include <fstream> // ifstream

using namespace Core::Math;
using namespace Core::IO;
using namespace Core;
using namespace DataIO;
using namespace DataIO::VolumeIO;
using namespace std;


class MyVolumeProcessor : public VolumeProcessor {
public:
  MyVolumeProcessor(const std::string& tmpDir) :
  VolumeProcessor(tmpDir) {}
protected:
  virtual void reporter(const std::string& desc, float progress) {
  if (progress == -1)
    std::cout << desc << std::endl;
  else
    std::cout << '\r' << desc << " (" << progress*100<< "%)" << "       ";
  }
};


int main(int argc, char** argv) {
  
  if (argc != 5 && argc != 8) {
    cout << "Need four or seven parameters but received " << argc-1 << endl << endl;
    cout << "Usage: " << FileTools::GetFilename(argv[0]) << " input-volume sizex sizey sizez [offsetx, offsety, offsetz]" << endl;
    return EXIT_FAILURE;
  }
  
  try {
    MyVolumeProcessor m("./");
    
    cout << endl<< "Loading input volume " << argv[1] << endl;
    auto inVol = std::make_shared<QVisVolume>(argv[1]);
    inVol->load(false);

    Vec3ui64 size(StringTools::FromString<int>(argv[2]),
                  StringTools::FromString<int>(argv[3]),
                  StringTools::FromString<int>(argv[4]));

    Vec3ui64 pos;
    if (argc == 8) {
      pos.x = StringTools::FromString<int>(argv[5]);
      pos.y = StringTools::FromString<int>(argv[6]);
      pos.z = StringTools::FromString<int>(argv[7]);
    }

    VolumeMetadataPtr md = std::make_shared<VolumeMetadata>(size,
                                                            inVol->getMetadata()->getType(),
                                                            inVol->getMetadata()->getUsedBits(),
                                                            inVol->getMetadata()->getComponents(),
                                                            inVol->getMetadata()->getAspectRatio(),
                                                            inVol->getMetadata()->getTransformation());

    stringstream outName;
    outName << "out-" << FileTools::RemoveExt(FileTools::GetFilename(argv[1])) << "-" << size.x << "x"
            << size.y << "x" << size.z;
    
    if (pos != Vec3ui64(0,0,0)) {
      outName << "-" << pos.x << "x" << pos.y << "x" << pos.z;
    }

    cout << endl<< "Creating output volumes " << outName.str() << ".dat" << " & " << outName.str() << ".raw" << endl;
    auto outVol = std::make_shared<QVisVolume>(outName.str()+".dat", outName.str()+".raw", md);
    outVol->create();
   
    ResizeParams rp = ResizeParams(ResizeParams::RM_PadCrop,
                                   pos, 0);

    m.transform(inVol, outVol, rp);

    outVol->save();
  } catch (const VolumeError& e) {
    cerr << "ERROR " << e.what() << endl;
    return EXIT_FAILURE;
  }
  
  return EXIT_SUCCESS;
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
