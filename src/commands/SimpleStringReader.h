#pragma once

#include "commands/ISerialReader.h"

#include "mocca/base/ByteArray.h"

#include <sstream>

namespace trinity {

class SimpleStringReader : public ISerialReader {
public:
    SimpleStringReader(const std::string& str);
    SimpleStringReader(mocca::ByteArray& data);

    float getFloat(const std::string& key) const override;
    double getDouble(const std::string& key) const override;
    uint8_t getUInt8(const std::string& key) const override;
    int32_t getInt32(const std::string& key) const override;
    uint32_t getUInt32(const std::string& key) const override;
    int64_t getInt64(const std::string& key) const override;
    uint64_t getUInt64(const std::string& key) const override;
    bool getBool(const std::string& key) const override;
    std::string getString(const std::string& key) const override;

    std::vector<float> getFloatVec(const std::string& key) const override;
    std::vector<int32_t> getInt32Vec(const std::string& key) const override;
    std::vector<uint8_t> getUInt8Vec(const std::string& key) const override;
    std::vector<bool> getBoolVec(const std::string& key) const override;
    std::vector<std::string> getStringVec(const std::string& key) const override;

    const std::vector<uint8_t>& getBinary() const override;

private:
    void getSerializableImpl(const std::string& key, ISerializable& obj) const override;
    std::vector<std::unique_ptr<ISerializable>> getSerializableVecImpl(const std::string& key, const ISerializable& prototype) const override;

private:
    mutable std::stringstream m_stream;
    mutable std::vector<uint8_t> m_binary;
};
}