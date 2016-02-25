#pragma once

#include "commands/ISerializable.h"
#include "commands/SerializerFactory.h"

namespace trinity {
namespace testing {

template <typename T> T writeAndRead(const T& obj) {
    auto writer = commands::ISerializerFactory::defaultFactory().createWriter();
    obj.serialize(*writer);
    T result;
    auto serialized = writer->write();
    auto reader = commands::ISerializerFactory::defaultFactory().createReader(serialized);
    result.deserialize(*reader);
    return result;
}
}
}