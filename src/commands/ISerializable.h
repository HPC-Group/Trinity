#pragma once

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

#include "mocca/base/Memory.h"

namespace trinity {
namespace commands {

class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual std::unique_ptr<ISerializable> clone() const = 0;
    virtual void serialize(ISerialWriter& writer) const = 0;
    virtual void deserialize(const ISerialReader& reader) = 0;
};

// CRTP for clone operation
template <typename Derived>
class SerializableTemplate : public ISerializable {
public:
    virtual std::unique_ptr<ISerializable> clone() const override {
        return mocca::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};
}
}