#pragma once
#include <istream>
#include <ostream>

namespace trinity {
namespace common {

class ISerializable
{
public:
    ISerializable(){}
    virtual ~ISerializable(){}
    
    virtual void serialize(std::ostream& stream) = 0;
    virtual void deserialize(std::istream& stream) = 0;
};
}
}