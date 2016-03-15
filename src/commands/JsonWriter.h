#pragma once

#include "commands/ISerialWriter.h"

#include "thirdparty/jsoncpp/json.h"

namespace trinity {

class JsonWriter : public ISerialWriter {
public:
    void appendFloat(const std::string& key, float value) override;
    void appendDouble(const std::string& key, double value) override;
    void appendInt(const std::string& key, int32_t value) override;
    void appendInt(const std::string& key, uint32_t value) override;
    void appendInt(const std::string& key, int64_t value) override;
    void appendInt(const std::string& key, uint64_t value) override;
    void appendBool(const std::string& key, bool value) override;
    void appendString(const std::string& key, const std::string& value) override;
    void appendObject(const std::string& key, const ISerializable& obj) override;

    void appendFloatVec(const std::string& key, const std::vector<float>& vec) override;
    void appendIntVec(const std::string& key, const std::vector<int32_t>& vec) override;
    void appendBoolVec(const std::string& key, const std::vector<bool>& vec) override;
    void appendStringVec(const std::string& key, const std::vector<std::string>& vec) override;
    void appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) override;

    mocca::ByteArray write() const override;

private:
    JsonCpp::Value m_root;
};
}