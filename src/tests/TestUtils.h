#pragma once

#include "commands/ISerialObjectFactory.h"
#include "commands/ISerializable.h"

namespace trinity {
namespace testing {

template <typename T> T writeAndRead(const T& obj) {
    auto serial = ISerialObjectFactory::create();
    obj.serialize(*serial);
    T result;
    result.deserialize(*serial);
    return result;
}
}
}