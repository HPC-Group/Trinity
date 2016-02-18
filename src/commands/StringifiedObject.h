#pragma once
#include <istream>
#include <ostream>
#include <sstream>

#include "ISerialObject.h"
#include "Vcl.h"

namespace trinity {
namespace commands {

class StringifiedObject : public ISerialObject
{
public:
    StringifiedObject(){}
    virtual ~StringifiedObject(){}
    
    // dmc: I'd suggest this way of declaring methods in interface implementations
	// void append(const std::string& key, float value) override;
    virtual void append(const std::string& key, float value);
    virtual void append(const std::string& key, int value);
    virtual void append(const std::string& key, const std::string& value);
    virtual void append(const std::string& key, const ISerializable* obj);
    
    virtual float getFloat(const std::string& key);
    virtual int getInt(const std::string& key);
    virtual std::string getString(const std::string& key);
    
    virtual void writeTo(std::ostream& stream);
    virtual void readFrom(std::istream& stream);
    
private:
    std::stringstream m_stream;
    Vcl m_vcl; //dmc: multiple copies of immutable object? see Vcl.h
};
}
}