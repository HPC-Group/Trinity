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

template <typename HandlerType, typename RequestType, typename ParentType>
typename RequestType::ReplyType handleRequest(const RequestType& request, ParentType* parent) {
    HandlerType handler(request, parent);
    auto reply = handler.execute();
    return static_cast<const typename RequestType::ReplyType&>(*reply);
}
}
}