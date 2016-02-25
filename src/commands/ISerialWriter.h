#pragma once

#include "mocca/base/ByteArray.h"

#include <string>

namespace trinity {
namespace commands {

class ISerializable;
class ISerialWriter {
public:
    virtual ~ISerialWriter() {}

    virtual void append(const std::string& key, float value) = 0;
    virtual void append(const std::string& key, int value) = 0;
    virtual void append(const std::string& key, const std::string& value) = 0;
    virtual void append(const std::string& key, const ISerializable& obj) = 0;

    virtual mocca::ByteArray write() const = 0;
};
}
}