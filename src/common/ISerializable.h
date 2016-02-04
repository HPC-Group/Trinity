#pragma once
#include <istream>
#include <ostream>

#include "ISerialObject.h"

namespace trinity {
namespace common {

class ISerializable
{
public:
    ISerializable(){}
    virtual ~ISerializable(){}
    
    virtual void serialize(ISerialObject& serial) = 0;
    virtual void deserialize(ISerialObject& serial) = 0;
};
}
}