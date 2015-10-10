#include <SilverBulletBase.h>
#include <Core/IO/FileTools.h>
#include <DataIO/VolumeIO/VolumeError.h>
#include <DataIO/VolumeIO/QVisVolume.h>

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream> // cout
#include <fstream> // ifstream

using namespace DataIO::VolumeIO;
using namespace std;
using namespace Core::IO;


int main(int argc, char** argv) {
  
  if (argc != 2) {
    cout << "Need one parameter but received " << argc-1 << endl << endl;
    cout << "Usage: " << FileTools::GetFilename(argv[0]) << " input-volume" << endl;
    return EXIT_FAILURE;
  }
  
  try {
    auto inVol = std::make_shared<QVisVolume>(argv[1]);
    inVol->load(false);
    cout << *inVol << endl;
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