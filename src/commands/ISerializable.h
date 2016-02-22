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

    virtual VclType getType() const = 0;
    virtual void serialize(ISerialObject& serial) const = 0;
    virtual void deserialize(ISerialObject& serial) = 0;
    virtual std::string toString() const;
};

std::ostream& operator<<(std::ostream& os, const ISerializable& obj);
}
}