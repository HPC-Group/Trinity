#pragma once

#include "commands/ISerialWriter.h"

#include "mocca/base/ByteArray.h"

namespace trinity {
namespace commands {

class SimpleStringWriter : public ISerialWriter {
public:
    void append(const std::string& key, float value) override;
    void append(const std::string& key, int value) override;
    void append(const std::string& key, const std::string& value) override;
    void append(const std::string& key, const ISerializable& obj) override;

    mocca::ByteArray write() const override;

private:
    mutable std::stringstream m_stream;
};
}
}