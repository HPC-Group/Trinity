#pragma once

#include <string>
#include <vector>
#include <memory>

namespace trinity {
namespace commands {

class ISerializable;
class ISerialReader {
public:
    virtual ~ISerialReader() {}

    virtual float getFloat(const std::string& key) const = 0;
    virtual int getInt(const std::string& key) const = 0;
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
    
    template <typename T> std::vector<std::unique_ptr<T>> getSerializableVec(const std::string& key) const {
        T prototype;
        auto tmp = getSerializableVecImpl(key, prototype);
        std::vector<std::unique_ptr<T>> result;
        for (auto& elem : tmp) {
            result.push_back(std::unique_ptr<T>(static_cast<T*>(elem.release())));
        }
        return result;
    }

    virtual std::vector<float> getFloatVec(const std::string& key) const = 0;
    virtual std::vector<int> getIntVec(const std::string& key) const = 0;
    virtual std::vector<std::string> getStringVec(const std::string& key) const = 0;

private:
    virtual void getSerializableImpl(const std::string& key, ISerializable& prototype) const = 0;
    virtual std::vector<std::unique_ptr<ISerializable>> getSerializableVecImpl(const std::string& key, const ISerializable& prototype) const = 0;
};
}
}