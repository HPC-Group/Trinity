#include "BrickCacher.h"

using namespace Core::IO::FileTools;


BrickCacher::BrickCacher(const std::string& dir,
                         const std::string& prefix,
                         const std::string& ext) :
m_dir(dir),
m_prefix(prefix),
m_ext(ext)
{
}

void BrickCacher::clear() const {
  std::vector<std::string> names = getDirContents(m_dir);
  for (const std::string& name : names) {
    if (getExt(name) == m_ext &&
        getFilename(name).find(m_prefix) == 0 ) {
      remove(name.c_str());
    }
  }
}

std::string BrickCacher::genFilename(const BrickKey& brickKey) const {
  return m_dir + std::string("/") + m_prefix + std::string("_") +
         brickKey.toFileDesc() + std::string(".") + m_ext;
}