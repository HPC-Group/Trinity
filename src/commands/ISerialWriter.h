#pragma once

#include "mocca/base/ByteArray.h"

#include <string>
#include <vector>
#include <memory>

namespace trinity {

class ISerializable;
class ISerialWriter {
public:
    virtual ~ISerialWriter() {}

    virtual void appendFloat(const std::string& key, float value) = 0;
    virtual void appendInt(const std::string& key, int32_t value) = 0;
    virtual void appendInt(const std::string& key, uint32_t value) = 0;
    virtual void appendInt(const std::string& key, int64_t value) = 0;
    virtual void appendInt(const std::string& key, uint64_t value) = 0;
    virtual void appendBool(const std::string& key, bool value) = 0;
    virtual void appendString(const std::string& key, const std::string& value) = 0;
    virtual void appendObject(const std::string& key, const ISerializable& obj) = 0;

    virtual void appendFloatVec(const std::string& key, const std::vector<float>& vec) = 0;
    virtual void appendIntVec(const std::string& key, const std::vector<int32_t>& vec) = 0;
    virtual void appendBoolVec(const std::string& key, const std::vector<bool>& vec) = 0;
    virtual void appendStringVec(const std::string& key, const std::vector<std::string>& vec) = 0;
    virtual void appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) = 0;

    virtual mocca::ByteArray write() const = 0;
};
}