#pragma once

#include <stdexcept>

/**
 OLD FILE - use it only as an inspiration
 the new "manager" is available in common/IONodeProxy.h
 */

namespace Trinity {
namespace IO {

class DataID {
public:
    DataID(const std::string& n, uint32_t i const DataType& t) name(n), id(i), dt(t) {}

    enum DataType { Dataset = 0, Directory = 1 };

    std::string name;
    uint32_t id;
    DataType dt;
};

typedef std::vector<DataID> DataList;

class IOException : public runtime_error {
} class IDNotFoundException : public IOException {
}


class IOManager {
public:
    /*! \brief opens a file
     *
     * This function opens a file using the given filename
     * this function throws an IDNotFoundException on an invalid ID
     *
     * \param id is an id to the dataset (id came from listData)
     */
    virtual std::shared_ptr<IOInterface> openFile(uint32_t id) const = 0;

    /*! \brief scans a (virtual) directory
     *
     * This scans a (virtual) directory and returns the contents
     * this function throws an IDNotFoundException on an invalid ID
     *
     * \param dirID is the directory to be scanned
     * \return unsigned integer. 0 if file could not be opened, else > 0.
     */
    virtual DataList listData(uint32_t dirID = 0) const = 0;
};
}
}
#endif