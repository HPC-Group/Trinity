#include <string>

#include "io-base/UVFListData.h"
#include "uvf/UVFIO.h"
#include "common/TrinityError.h"
#include "mocca/log/LogManager.h"
#include "silverbullet/io/FileTools.h"
#include "silverbullet/base/StringTools.h"

// TODO Fix this:
// Important Note:
// currently we use the actual path of the file with a prefix (see below)
// as dataset ID, however, this is a huge security issue as a potential
// attacker can send just about any directory with the prefix and then
// browse that directory and open the datasets there. One simple fix of
// this issue would be to call the binary with chroot restricting it to a
// selected dataset directory, a better solution would be to check the
// the given path if it's within the intended directory (but that may be
// nontrivial)

#define UVFDataPrefix std::string("UVFData")
#define UVFSearchRoot std::string(".")


// compile this lister with the VALIDATE_FILES to check a
// file not only for the proper extension but also validate
// the MD5 sum. This provides much better error detection but
// can take a significant time for large files (hours for TB sized files)

// #define VALIDATE_FILES

using namespace trinity;
using namespace Core::IO::FileTools;
using namespace Core::StringTools;

bool UVFListData::stripListerID(std::string& id) const {
  if (id.find(UVFDataPrefix+"@") != 0) {

    if (id == UVFDataPrefix)
      return false;
    else {
      id = UVFSearchRoot;
      return true;
    }
  
  } else {
    id = id.substr(UVFDataPrefix.length());
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
                                   UVFDataPrefix+"@"+name,
                                   IOData::DataType::Directory));
      } else {
        // TODO: replace the first param of the IOData constructor
        // (getFilename(name)) by the human readable description from the
        // uvf-file
        if (ToLowerCase(getExt(path)) == "uvf")
          ioDataVec.push_back(IOData(getFilename(name),
                                     UVFDataPrefix+"@"+name,
                                     IOData::DataType::Dataset));
      }
    }
  }
  return ioDataVec;
}

std::unique_ptr<trinity::IIO> UVFListData::createIO(const std::string& fileId) const {
  return std::unique_ptr<UVFIO>(new UVFIO(fileId, *this));
}


