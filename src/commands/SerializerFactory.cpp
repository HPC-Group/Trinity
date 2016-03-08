#include "commands/SerializerFactory.h"

#include "commands/JsonReader.h"
#include "commands/JsonWriter.h"
#include "commands/SimpleStringReader.h"
#include "commands/SimpleStringWriter.h"

#include "mocca/base/Memory.h"

using namespace trinity;

const ISerializerFactory& ISerializerFactory::defaultFactory() {
    static JsonSerializerFactory factory;
    return factory;
}

std::unique_ptr<ISerialWriter> JsonSerializerFactory::createWriter() const {
    return mocca::make_unique<JsonWriter>();
}

std::unique_ptr<ISerialReader> JsonSerializerFactory::createReader(mocca::ByteArray& data) const {
    data.resetReadPos(); // fixme: not nice
    std::string jsonStr = data.read(data.size());
    return mocca::make_unique<JsonReader>(jsonStr);
}

std::unique_ptr<ISerialWriter> SimpleStringSerializerFactory::createWriter() const {
    return mocca::make_unique<SimpleStringWriter>();
}

std::unique_ptr<ISerialReader> SimpleStringSerializerFactory::createReader(mocca::ByteArray& data) const {
    data.resetReadPos(); // fixme: not nice
    std::string str = data.read(data.size());
    return mocca::make_unique<SimpleStringReader>(str);
}