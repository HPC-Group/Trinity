#include <SilverBulletBase.h>
#include <Core/IO/FileTools.h>
#include <Core/StringTools.h>
#include <DataIO/VolumeIO/VolumeError.h>
#include <DataIO/VolumeIO/QVisVolume.h>
#include <DataIO/VolumeIO/RAWVolume.h>
#include <DataIO/VolumeIO/VolumeProcessor.h>
#include <DataIO/VolumeIO/IV3DTFMapper.h>
#include <DataIO/VolumeIO/HVSTFMapper.h>

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
  
  if (argc != 4 && argc != 3) {
    cout << "Need two or three parameters but received " << argc-1 << endl << endl;
    cout << "Usage: " << FileTools::GetFilename(argv[0]) << " TF-File QVisVolume [premultiply]" << endl;
    return EXIT_FAILURE;
  }
  
  try {
    MyVolumeProcessor m("./");
    TextfileTFMapperPtr mapper = nullptr;
    
    cout << endl<< "Loading input volume " << argv[2] << endl;
    auto inVol = std::make_shared<QVisVolume>(argv[2]);
    inVol->load(false);

    cout << "Loading transfer function " << argv[1] << endl;
    std::string ext = StringTools::ToLowerCase(FileTools::GetExt(argv[1]));
    if (ext == "tf") {
      HVSTFMapperPtr hvsmapper = std::make_shared<HVSTFMapper>();
      if (!hvsmapper->load(argv[1])) {
        throw VolumeError("Unable to load transfer function");
      }
      mapper = hvsmapper;
    } else {
      if (ext == "1dt") {
        IV3DTFMapperPtr iv3dmapper = std::make_shared<IV3DTFMapper>();
        cout << endl<< "Finding max value in volume to properly scale the ImageVis3D TF" << endl;
        double max = m.reduce(inVol, 0, VolumeProcessor::RO_max);

        cout << endl<< "max is " << max << endl;

        if (!iv3dmapper->load(argv[1], uint64_t(max))) {
          throw VolumeError("Unable to load transfer function");
        }
        mapper = iv3dmapper;
      } else
        throw VolumeError("Unable to load transfer function, unknown extension");
    }

    if (argc == 4 && std::string(argv[3]) == "premultiply") {
      cout << endl<< "pre-multiplying transfer function"<< endl;
      mapper->premultiply();
    }

    
    VolumeMetadataPtr md = std::make_shared<VolumeMetadata>(inVol->getMetadata()->getSize(),
                                                            DataType(true, true, 4),
                                                            32,
                                                            4,
                                                            inVol->getMetadata()->getAspectRatio(),
                                                            inVol->getMetadata()->getTransformation());

    stringstream outName;
    outName << "out-" << FileTools::RemoveExt(FileTools::GetFilename(argv[2])) << "-" << inVol->getMetadata()->getSize().x << "x"
            << inVol->getMetadata()->getSize().y << "x" << inVol->getMetadata()->getSize().z << "-float.raw";

    cout << endl<< "Creating output volume " << outName.str() << endl;
    auto outVol = std::make_shared<RawVolume>(outName.str(), md);
    outVol->create();
   
    cout << endl<< "Applying TF to volume" << endl;
    m.map(inVol, outVol, mapper);
    outVol->save();

    md = std::make_shared<VolumeMetadata>(inVol->getMetadata()->getSize(),
                                                            DataType(false, false, 1),
                                                            8,
                                                            4,
                                                            inVol->getMetadata()->getAspectRatio(),
                                                            inVol->getMetadata()->getTransformation());

    stringstream outName2;
    outName2 << "out-" << FileTools::RemoveExt(FileTools::GetFilename(argv[2])) << "-" << inVol->getMetadata()->getSize().x << "x"
            << inVol->getMetadata()->getSize().y << "x" << inVol->getMetadata()->getSize().z << "-8bit";

    cout << endl<< "Creating output volume 8bit " << outName2.str() << endl;
    auto outVol2 = std::make_shared<QVisVolume>(outName2.str()+".dat", outName2.str()+".raw", md);
    outVol2->create();
   
    cout << endl<< "Applying TF to volume" << endl;
    m.map(inVol, outVol2, mapper);
    outVol2->save();




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
