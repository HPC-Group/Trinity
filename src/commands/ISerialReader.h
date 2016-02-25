#pragma once

#include <string>

namespace trinity {
namespace commands {

class ISerializable;
class ISerialReader {
public:
    virtual ~ISerialReader() {}

    virtual float getFloat(const std::string& key) const = 0;
    virtual int getInt(const std::string& key) const = 0;
    virtual std::string getString(const std::string& key) const = 0;
    virtual void getSerializable(const std::string& key, ISerializable& obj) const = 0;
};
}
}