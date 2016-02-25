#pragma once

#include "commands/ISerialWriter.h"

#include "thirdparty/jsoncpp/json.h"

namespace trinity {
namespace commands {

class JsonWriter : public ISerialWriter {
public:
    void append(const std::string& key, float value) override;
    void append(const std::string& key, int value) override;
    void append(const std::string& key, const std::string& value) override;
    void append(const std::string& key, const ISerializable& obj) override;

    mocca::ByteArray write() const override;

private:
    JsonCpp::Value m_root;
};
}
}