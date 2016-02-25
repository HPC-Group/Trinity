#pragma once

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

namespace trinity {
namespace commands {

class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual void serialize(ISerialWriter& writer) const = 0;
    virtual void deserialize(const ISerialReader& reader) = 0;
};
}
}