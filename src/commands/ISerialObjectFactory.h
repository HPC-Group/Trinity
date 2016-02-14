#pragma once
#include <istream>
#include <ostream>
#include <memory>

#include "ISerialObject.h"

namespace trinity {
namespace commands {

// abstract factory
class ISerialObjectFactory
{
public:
    ISerialObjectFactory(){}
    virtual ~ISerialObjectFactory(){}
    
    static std::unique_ptr<ISerialObject> create();
};
}
}