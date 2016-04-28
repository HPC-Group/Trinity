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



// compile this lister with the VALIDATE_FILES to check a
// file not only for the proper extension but also validate
// the MD5 sum. This provides much better error detection but
// can take a significant time for large files (hours for TB sized files)

// #define VALIDATE_FILES

using namespace trinity;
using namespace Core::IO::FileTools;
using namespace Core::StringTools;

trinity::UVFListData::UVFListData(const std::string& basePath)
    : m_basePath(basePath)
{}

bool UVFListData::stripListerID(std::string& id) const {
  if (id == UVFDataPrefix) {
    id = m_basePath;
    return true;
  }
  
  if (id.find(UVFDataPrefix+"@") == 0) {
    id = id.substr((UVFDataPrefix+"@").length());
    if (id.empty())
      id = m_basePath;
    return true;
  } else {
    return false;
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
    if (!isDirectory(path)) return ioDataVec;
    
    std::vector<std::string> names = getDirContents(path);
    for (const std::string& name : names) {
      // TODO: replace the first param of the IOData constructor
      // (getFilename(name)) by the human readable description from the
      // uvf-file
      if (ToLowerCase(getExt(name)) == "uvf")
        ioDataVec.push_back(IOData(getFilename(name),
                                   UVFDataPrefix+"@"+name,
                                   IOData::DataType::Dataset));
    }
    std::vector<std::string> dirs = getSubDirList(path);
    for (const std::string& dir : dirs) {
      if (dir == "." || dir == ".." || dir == "") continue;
      
      ioDataVec.push_back(IOData(getFilename(dir),
                                 UVFDataPrefix+"@"+dir,
                                 IOData::DataType::Directory));
    }
    
  }
  return ioDataVec;
}

std::unique_ptr<trinity::IIO> UVFListData::createIO(const std::string& fileId) const {
  return std::unique_ptr<UVFIO>(new UVFIO(fileId, *this));
}

std::string UVFListData::getRoot() const {
    return "UVFData";
}