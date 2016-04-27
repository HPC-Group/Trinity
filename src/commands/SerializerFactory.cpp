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

std::unique_ptr<ISerialWriter> JsonSerializerFactory::createWriter(CompressionMode compressionMode) const {
    std::unique_ptr<BinaryWriter> binaryWriter;
    if (compressionMode == CompressionMode::Uncompressed) {
        binaryWriter = mocca::make_unique<BinaryNullWriter>();
    }
    else {
        binaryWriter = mocca::make_unique<BinaryCompressWriter>();
    }
    return mocca::make_unique<JsonWriter>(std::move(binaryWriter));
}

std::unique_ptr<ISerialReader> JsonSerializerFactory::createReader(const mocca::net::Message &message, CompressionMode compressionMode) const {
    std::unique_ptr<BinaryReader> binaryReader;
    if (compressionMode == CompressionMode::Uncompressed) {
        binaryReader = mocca::make_unique<BinaryNullReader>();
    }
    else {
        binaryReader = mocca::make_unique<BinaryDecompressReader>();
    }
    return mocca::make_unique<JsonReader>(message, std::move(binaryReader));
}