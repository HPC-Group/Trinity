#include <SilverBulletBase.h>
#include <Core/IO/FileTools.h>
#include <Core/StringTools.h>
#include <DataIO/GeometryIO/VTKGeometry.h>

#include <DataIO/VolumeIO/VolumeError.h>
#include <DataIO/VolumeIO/QVisVolume.h>

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // cout
#include <fstream>  // ifstream

using namespace DataIO::GeometryIO;
using namespace Core::Math;
using namespace DataIO;
using namespace DataIO::VolumeIO;
using namespace std;


int main(int argc, char** argv) {
  
  if (argc != 8+1 && argc != 3) {
    cout << "Need eight or two parameter but received " << argc-1
              << endl << endl;
    cout << "Usage (for info): " << endl << "  " << Core::IO::FileTools::GetFilename(argv[0])
              << " inputFile component" << endl;
    cout << "Usage (for splatting): " << endl << "  " << Core::IO::FileTools::GetFilename(argv[0])
              << " inputFile component startOrbit endOrbit outputFile sizeX sizeY sizeZ" << endl;
    return EXIT_FAILURE;
  }

  string inputFile = argv[1];
  uint32_t component = Core::StringTools::FromString<uint32_t>(argv[2]);

  if(argc == 3) {
    try {    
      cout << "Loading input dataset " << inputFile << endl;
      auto vtkData = make_shared<VTKGeometry>(inputFile);
      vtkData->load(false);
      
      cout << "Input Dataset info: " << endl
                << vtkData->toString() << endl;
      const GeometryMetadataPtr& md = vtkData->getMetadata();
      uint64_t iSplatCount = md->getNumPoints();
      double minValue = vtkData->getAttribute<double>(0, component);
      double maxValue = minValue;

      for (uint64_t i = 1;i<iSplatCount;++i) {
        
        double value = vtkData->getAttribute<double>(i, component);
        
        minValue = std::min(minValue, value);
        maxValue = std::max(maxValue, value);
      }
      
      cout << "  Max Value: " << minValue << endl;
      cout << "  Min Value: " << maxValue << endl;

      return EXIT_SUCCESS;
    } catch (const GeometryError& e) {
      cerr << "Geometry File ERROR " << e.what() << endl;
      return EXIT_FAILURE;
    }    
  }
  uint32_t startOrbit = Core::StringTools::FromString<uint32_t>(argv[3]);
  uint32_t endOrbit = Core::StringTools::FromString<uint32_t>(argv[4]);
  string outputFile = argv[5];
  uint32_t sizeX = Core::StringTools::FromString<uint32_t>(argv[6]);
  uint32_t sizeY = Core::StringTools::FromString<uint32_t>(argv[7]);
  uint32_t sizeZ = Core::StringTools::FromString<uint32_t>(argv[8]);
  
  Vec3ui64 volSize(sizeX,sizeY,sizeZ);
  
  VolumeMetadataPtr md = make_shared<VolumeMetadata>(volSize,
                                                     DataType(true, true, 4),
                                                     16, 1);
  
  
  try {
    cout << "Loading input dataset " << inputFile << endl;
    auto vtkData = make_shared<VTKGeometry>(inputFile);
    vtkData->load(false);
    
    cout << "Input Dataset info: " << endl
              << vtkData->toString() << endl;

    cout << endl<< "Creating output volume " << outputFile << ".dat" << endl;
    auto outVol = make_shared<QVisVolume>(outputFile+".dat", outputFile+".raw", md);
    outVol->create();
    
    const GeometryMetadataPtr& md = vtkData->getMetadata();
    
    uint64_t iSplatCount = md->getNumPoints();
    cout << "Processing " << iSplatCount << " points." << endl;
    
    cout << "Computing value range ..."<< endl;
    
    Vec3f point;
    vtkData->getPoint(0, point);
    point.z = vtkData->getAttribute<float>(0, 1);

    Vec3f max=point, min=point;
    double minValue = vtkData->getAttribute<double>(0, component);
    double maxValue = minValue;

    // compute max/min of positions
    for (uint64_t i = 1;i<iSplatCount;++i) {
      uint32_t orbit = vtkData->getAttribute<uint32_t>(i, 2);
      if (orbit < startOrbit || orbit > endOrbit) continue;
      
      vtkData->getPoint(i, point);
      point.z = vtkData->getAttribute<float>(i, 1);      
      max.StoreMax(point);
      min.StoreMin(point);
    }
    
    // compute max/min of data values
    if (!md->hasPointData())
      iSplatCount = md->getNumPrimitives(); // for primitive data iterate over primitves

    for (uint64_t i = 1;i<iSplatCount;++i) {
      uint32_t orbit = vtkData->getAttribute<uint32_t>(i, 2);
      if (orbit < startOrbit || orbit > endOrbit) continue;
      double value = vtkData->getAttribute<double>(i, component);      
      minValue = std::min(minValue, value);
      maxValue = std::max(maxValue, value);
    }

    // hack: for the vis contest data to make sure we cover the entire sphere
    max.StoreMax(Vec3f(180,90,-1000));
    min.StoreMin(Vec3f(-180,-90,1000));

    cout << "  Max: " << max << endl;
    cout << "  Min: " << min << endl;
    cout << "  Max Value: " << minValue << endl;
    cout << "  Min Value: " << maxValue << endl;
    
    Vec3f bias=min;
    Vec3f scale=max-min;
    
    double valueBias = minValue;
    double valueScale = maxValue-minValue;
    
    cout << "Splatting ..."<< endl;
    for (uint64_t i = 0;i<iSplatCount;++i) {
      uint32_t orbit = vtkData->getAttribute<uint32_t>(i, 2);
      if (orbit < startOrbit || orbit > endOrbit) continue;
      
      if (md->hasPointData()) {
        vtkData->getPoint(i, point);
      } else {
        std::vector<uint64_t> prim;
        vtkData->getPrimitive(i,prim);
        vtkData->getPoint(prim[0], point); // simply use the position of the first point of the primitve for now
      }
      point.z = vtkData->getAttribute<float>(i, 1);
      double value = vtkData->getAttribute<double>(i, component);
      
      value = (value - valueBias)/valueScale;
      point = Vec3f(volSize-1)*((point-bias)/scale);
      
      outVol->setValue<float>(Vec3ui64(point), 0, value);
    }
    
    
  } catch (const GeometryError& e) {
    cerr << "Geometry File ERROR " << e.what() << endl;
    return EXIT_FAILURE;
  } catch (const VolumeError& e) {
    cerr << "Volume File ERROR " << e.what() << endl;
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
