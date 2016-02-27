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

    std::vector<float> getFloatVec(const std::string& key) const override;
    std::vector<int> getIntVec(const std::string& key) const override;
    std::vector<std::string> getStringVec(const std::string& key) const override;

private:
    JsonReader(const JsonCpp::Value& root);

    void getSerializableImpl(const std::string& key, ISerializable& prototype) const override;
    std::vector<std::unique_ptr<ISerializable>> getSerializableVecImpl(const std::string& key, const ISerializable& prototype) const override;

private:
    JsonCpp::Value m_root;
};
}
}