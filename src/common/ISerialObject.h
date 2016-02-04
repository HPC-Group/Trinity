#pragma once
#include <istream>
#include <ostream>

namespace trinity {
namespace common {

class ISerialObject
{
public:

    
    virtual void append(const std::string& key, float value) = 0;
    virtual void append(const std::string& key, int value) = 0;
    virtual void append(const std::string& key, const std::string& value) = 0;
    
    virtual float getFloat(const std::string& key) = 0;
    virtual int getInt(const std::string& key) = 0;
    virtual std::string getString(const std::string& key) = 0;
    
    virtual void writeTo(std::ostream& stream) = 0;
    virtual void readFrom(std::istream& stream) = 0;
};
}
}