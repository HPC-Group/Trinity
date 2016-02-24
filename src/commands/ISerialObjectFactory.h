#pragma once

#include "commands/JsonObject.h"
#include "commands/StringifiedObject.h"

#include <memory>

namespace trinity {
namespace commands {

// abstract factory
class ISerialObjectFactory {
public:
    virtual ~ISerialObjectFactory() {}

    static std::unique_ptr<ISerialObject> create() { return std::unique_ptr<ISerialObject>(new StringifiedObject); }
     //static std::unique_ptr<ISerialObject> create() { return std::unique_ptr<ISerialObject>(new JsonObject); }
};
}
}