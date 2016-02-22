#pragma once
#include <istream>
#include <memory>
#include <ostream>

#include "ISerialObject.h"
#include "StringifiedObject.h"

namespace trinity {
namespace commands {

// abstract factory
class ISerialObjectFactory {
public:
    ISerialObjectFactory() {}
    virtual ~ISerialObjectFactory() {}

    static std::unique_ptr<ISerialObject> create() { return std::unique_ptr<ISerialObject>(new StringifiedObject); }
};
}
}