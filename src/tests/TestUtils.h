#pragma once

#include "commands/ISerializable.h"
#include "commands/SerializerFactory.h"

namespace trinity {
namespace testing {

template <typename T> T writeAndRead(const T& obj) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    obj.serialize(*writer);
    T result;
    auto serialized = writer->write();
    auto reader = ISerializerFactory::defaultFactory().createReader(serialized);
    result.deserialize(*reader);
    return result;
}
}
}