#pragma once

#include <string>
#include <vector>
#include <memory>

#include "mocca/base/Memory.h"

namespace trinity {

class ISerializable;
class ISerialReader {
public:
    virtual ~ISerialReader() {}

    virtual float getFloat(const std::string& key) const = 0;
    virtual double getDouble(const std::string& key) const = 0;
    virtual uint8_t getUInt8(const std::string& key) const = 0;
    virtual int32_t getInt32(const std::string& key) const = 0;
    virtual uint32_t getUInt32(const std::string& key) const = 0;
    virtual int64_t getInt64(const std::string& key) const = 0;
    virtual uint64_t getUInt64(const std::string& key) const = 0;
    virtual bool getBool(const std::string& key) const = 0;
    virtual std::string getString(const std::string& key) const = 0;
    template <typename T> T getSerializable(const std::string& key) const {
        T prototype;
        getSerializableImpl(key, prototype);
        return prototype;
    }
    template <typename T> std::unique_ptr<T> getSerializablePtr(const std::string& key) const {
        auto prototype = mocca::make_unique<T>();
        getSerializableImpl(key, *prototype);
        return prototype;
    }
    
    template <typename T> std::vector<T> getSerializableVec(const std::string& key) const {
        T prototype;
        auto tmp = getSerializableVecImpl(key, prototype);
        std::vector<T> result;
        for (auto& elem : tmp) {
            result.push_back(*static_cast<T*>(elem.get()));
        }
        return result;
    }

    virtual std::vector<float> getFloatVec(const std::string& key) const = 0;
    virtual std::vector<int32_t> getInt32Vec(const std::string& key) const = 0;
    virtual std::vector<uint8_t> getUInt8Vec(const std::string& key) const = 0;
    virtual std::vector<bool> getBoolVec(const std::string& key) const = 0;
    virtual std::vector<std::string> getStringVec(const std::string& key) const = 0;

private:
    virtual void getSerializableImpl(const std::string& key, ISerializable& prototype) const = 0;
    virtual std::vector<std::unique_ptr<ISerializable>> getSerializableVecImpl(const std::string& key, const ISerializable& prototype) const = 0;
};
}