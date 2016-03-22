#include "commands/JsonWriter.h"

#include "commands/ISerializable.h"

using namespace trinity;

JsonWriter::JsonWriter()
    : m_binary(std::make_shared<std::vector<uint8_t>>()) {}

JsonWriter::JsonWriter(std::shared_ptr<std::vector<uint8_t>> binary)
    : m_binary(binary) {}

void JsonWriter::appendFloat(const std::string& key, float value) {
    m_root[key] = value;
}

void JsonWriter::appendDouble(const std::string& key, double value) {
    m_root[key] = value;
}

void JsonWriter::appendInt(const std::string& key, uint8_t value) {
    m_root[key] = static_cast<JsonCpp::UInt>(value); // uint8 not supported by JSON
}

void JsonWriter::appendInt(const std::string& key, int32_t value) {
    m_root[key] = static_cast<JsonCpp::Int>(value);
}

void JsonWriter::appendInt(const std::string& key, uint32_t value) {
    m_root[key] = static_cast<JsonCpp::UInt>(value);
}

void JsonWriter::appendInt(const std::string& key, int64_t value) {
    m_root[key] = static_cast<JsonCpp::Int64>(value);
}

void JsonWriter::appendInt(const std::string& key, uint64_t value) {
    m_root[key] = static_cast<JsonCpp::UInt64>(value);
}

void JsonWriter::appendBool(const std::string& key, bool value) {
    m_root[key] = value;
}

void JsonWriter::appendString(const std::string& key, const std::string& value) {
    m_root[key] = value;
}

void JsonWriter::appendObject(const std::string& key, const ISerializable& obj) {
    JsonWriter subObject(m_binary);
    obj.serialize(subObject);
    m_root[key] = subObject.m_root;
}

void JsonWriter::appendFloatVec(const std::string& key, const std::vector<float>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendIntVec(const std::string& key, const std::vector<int32_t>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendIntVec(const std::string& key, const std::vector<uint8_t>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::UInt(vec[i]); // uint8 not supported by JSON
    }
}

void JsonWriter::appendBoolVec(const std::string& key, const std::vector<bool>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::Value(vec[i]);
    }
}

void JsonWriter::appendStringVec(const std::string& key, const std::vector<std::string>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        JsonWriter subObject;
        vec[i]->serialize(subObject);
        m_root[key][i] = subObject.m_root;
    }
}

void JsonWriter::appendBinary(const std::vector<uint8_t>& vec) {
    m_binary->insert(end(*m_binary), begin(vec), end(vec));
}

mocca::ByteArray JsonWriter::write() const {
    JsonCpp::FastWriter writer;
    std::string str = writer.write(m_root);
    mocca::ByteArray result(sizeof(uint32_t) + str.size() + m_binary->size());
    result << static_cast<uint32_t>(str.size());
    result.append(str.data(), str.size());
    result.append(m_binary->data(), m_binary->size());
    return result;
}