#pragma once

#include "common/Enums.h"
#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

#include "mocca/net/Message.h"

#include <memory>

namespace trinity {

// abstract factory pattern
class ISerializerFactory {
public:
    virtual std::unique_ptr<ISerialWriter> createWriter(CompressionMode compressionMode = CompressionMode::Uncompressed) const = 0;
    virtual std::unique_ptr<ISerialReader> createReader(const mocca::net::Message &message, CompressionMode compressionMode = CompressionMode::Uncompressed) const = 0;

    static const ISerializerFactory& defaultFactory(); // edit this method to change the default serializer
};

class JsonSerializerFactory : public ISerializerFactory {
public:
    std::unique_ptr<ISerialWriter> createWriter(CompressionMode compressionMode = CompressionMode::Uncompressed) const override;
    std::unique_ptr<ISerialReader> createReader(const mocca::net::Message &message, CompressionMode compressionMode = CompressionMode::Uncompressed) const override;
};
}
