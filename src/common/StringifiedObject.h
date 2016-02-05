#pragma once
#include <istream>
#include <ostream>
#include <sstream>

#include "ISerialObject.h"

namespace trinity {
namespace common {

class StringifiedObject : public ISerialObject
{
public:
    StringifiedObject(){}
    virtual ~StringifiedObject(){}
    
    
    virtual void append(const std::string& key, float value);
    virtual void append(const std::string& key, int value);
    virtual void append(const std::string& key, const std::string& value);
    
    virtual float getFloat(const std::string& key);
    virtual int getInt(const std::string& key);
    virtual std::string getString(const std::string& key);
    
    virtual void writeTo(std::ostream& stream);
    virtual void readFrom(std::istream& stream);
    
private:
    std::stringstream m_stream;
};
}
}