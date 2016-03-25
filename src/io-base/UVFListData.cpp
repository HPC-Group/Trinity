#include <string>

#include "io-base/UVFListData.h"
#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"
#include "silverbullet/io/FileTools.h"
#include "silverbullet/base/StringTools.h"

#define UVFDataRoot std::string("UVFData")


// compile this lister with the VALIDATE_FILES to check a
// file not only for the proper extension but also validate
// the MD5 sum. This provides much better error detection but
// can take a significant time for large files (hours for TB sized files)

// #define VALIDATE_FILES

using namespace trinity;
using namespace Core::IO::FileTools;
using namespace Core::StringTools;

bool UVFListData::stripListerID(std::string& id) const {
  if (id.find(UVFDataRoot+"@") != 0) {

    if (id.find(UVFDataRoot) != 0)
      return false;
    else {
      id = id.substr(UVFDataRoot.length());
      return true;
    }
  
  } else {
    id = id.substr(UVFDataRoot.length());
    return true;
  }
}

bool UVFListData::containsIOData(const std::string& fileOrDirID) const {
  std::string path = fileOrDirID;
  
  // check for prefix
  if (!stripListerID(path)) {
    return false;
  } else {
    // it must be either a directory or have uvf as file extension
    // TODO: try to open the file and see what happens AND
    // in addition if VALIDATE_FILES is set compute the checksum
    return isDirectory(path) || ToLowerCase(getExt(path)) == "uvf";
  }
}

std::vector<IOData> UVFListData::listData(const std::string& dirID) const {
  std::string path = dirID;
  std::vector<IOData> ioDataVec;
  
  if (stripListerID(path)) {
    std::vector<std::string> names = getDirContents(path);
    
    for (const std::string& name : names) {
      if (isDirectory(name)) {
        ioDataVec.push_back(IOData(getFilename(name),
                                   UVFDataRoot+"@"+name,
                                   IOData::DataType::Directory));
      } else {
        // TODO: replace the first param of the IOData constructor
        // (getFilename(name)) by the human readable description from the
        // uvf-file
        if (ToLowerCase(getExt(path)) == "uvf")
          ioDataVec.push_back(IOData(getFilename(name),
                                     UVFDataRoot+"@"+name,
                                     IOData::DataType::Dataset));
      }
    }
  }
  return ioDataVec;
}

