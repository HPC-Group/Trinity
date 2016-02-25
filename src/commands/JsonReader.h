#pragma once

#include "commands/ISerialReader.h"

#include "thirdparty/jsoncpp/json.h"

namespace trinity {
namespace commands {

class JsonReader : public ISerialReader {
public:
    JsonReader(const std::string& str);

    float getFloat(const std::string& key) const override;
    int getInt(const std::string& key) const override;
    std::string getString(const std::string& key) const override;
    void getSerializable(const std::string& key, ISerializable& obj) const override;

private:
    JsonReader(const JsonCpp::Value& root);

private:
    JsonCpp::Value m_root;
};
}
}