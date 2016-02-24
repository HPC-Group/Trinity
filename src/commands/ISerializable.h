#pragma once

#include "ISerialObject.h"

#include <istream>
#include <ostream>
#include <sstream>
#include <string>

namespace trinity {
namespace commands {

class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual void serialize(ISerialObject& serial) const = 0;
    virtual void deserialize(const ISerialObject& serial) = 0;
};
}
}