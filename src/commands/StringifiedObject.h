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
    void append(const std::string& key, float value) override;
    void append(const std::string& key, int value) override;
    void append(const std::string& key, const std::string& value) override;
    void append(const std::string& key, const ISerializable& obj) override;
    
    float getFloat(const std::string& key) override;
    int getInt(const std::string& key) override;
    std::string getString(const std::string& key) override;
    void getSerializable(const std::string& key, ISerializable& obj) override;
    
    void writeTo(std::ostream& stream) override;
    void readFrom(std::istream& stream) override;
    
private:
    std::stringstream m_stream;
};
}
}