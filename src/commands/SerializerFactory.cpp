#include "commands/SerializerFactory.h"

#include "commands/JsonReader.h"
#include "commands/JsonWriter.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity;

const ISerializerFactory& ISerializerFactory::defaultFactory() {
    static JsonSerializerFactory factory;
    return factory;
}

std::unique_ptr<ISerialWriter> JsonSerializerFactory::createWriter() const {
    return mocca::make_unique<JsonWriter>();
}

std::unique_ptr<ISerialReader> JsonSerializerFactory::createReader(const mocca::net::Message &message) const {
    return mocca::make_unique<JsonReader>(message);
}