#pragma once

#include "commands/ISerialWriter.h"

#include "mocca/base/ByteArray.h"

namespace trinity {

class SimpleStringWriter : public ISerialWriter {
public:
    void appendFloat(const std::string& key, float value) override;
    void appendInt32(const std::string& key, int32_t value) override;
    void appendBool(const std::string& key, bool value) override;
    void appendString(const std::string& key, const std::string& value) override;
    void appendObject(const std::string& key, const ISerializable& obj) override;

    void appendFloatVec(const std::string& key, const std::vector<float>& vec) override;
    void appendInt32Vec(const std::string& key, const std::vector<int32_t>& vec) override;
    void appendBoolVec(const std::string& key, const std::vector<bool>& vec) override;
    void appendStringVec(const std::string& key, const std::vector<std::string>& vec) override;
    void appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) override;

    mocca::ByteArray write() const override;

private:
    mutable std::stringstream m_stream;
};
}