#pragma once

#include <cstdint>
#include <list>
#include <string>
#include <vector>

class Dataset;

/// Interface for Datasets which are actually backed by files
class FileBackedDataset {
public:
  virtual std::string Filename() const=0;
  /// A user-visible name for your format.  This might get displayed in UI
  /// elements; e.g. the GUI might ask if the user wants to use the "Name()
  /// reader" to open a particular file.
  virtual const char* Name() const { return "Generic"; }

  virtual bool CanRead(const std::string&, const std::vector<int8_t>&) const=0;
  /// Use to verify checksum.  Default implementation says the checksum is
  /// always valid.
  virtual bool Verify(const std::string&) const;
  virtual Dataset* Create(const std::string&, uint64_t, bool) const=0;

  /// @return a list of file extensions readable by this format
  virtual std::list<std::string> Extensions() const=0;
};
