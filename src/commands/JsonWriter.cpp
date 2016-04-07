#include "commands/JsonWriter.h"

#include "commands/ISerializable.h"

using namespace trinity;

JsonWriter::JsonWriter()
    : m_binary(std::make_shared<SharedDataVec>()) {}

JsonWriter::JsonWriter(std::shared_ptr<SharedDataVec> binary)
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
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendIntVec(const std::string& key, const std::vector<uint8_t>& vec) {
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::UInt(vec[i]); // uint8 not supported by JSON
    }
}

void JsonWriter::appendIntVec(const std::string& key, const std::vector<int32_t>& vec) {
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::Int(vec[i]);
    }
}

void JsonWriter::appendIntVec(const std::string& key, const std::vector<uint64_t>& vec) {
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::UInt64(vec[i]);
    }
}

void JsonWriter::appendBoolVec(const std::string& key, const std::vector<bool>& vec) {
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::Value(vec[i]);
    }
}

void JsonWriter::appendStringVec(const std::string& key, const std::vector<std::string>& vec) {
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) {
    m_root[key] = JsonCpp::Value(JsonCpp::arrayValue);
    for (uint32_t i = 0; i < vec.size(); ++i) {
        JsonWriter subObject;
        vec[i]->serialize(subObject);
        m_root[key][i] = subObject.m_root;
    }
}

void JsonWriter::appendBinary(std::shared_ptr<std::vector<uint8_t>> binary) {
    m_binary->push_back(binary);
}

mocca::net::Message JsonWriter::writeMessage() const {
    JsonCpp::FastWriter writer;
    std::string str = writer.write(m_root);
    auto jsonData = std::make_shared<std::vector<uint8_t>>();
    jsonData->resize(str.size());
    // FIXME: this copy might be avoided by using JsonCpp::StreamWriter and streaming the json directly into the vector
    std::copy(begin(str), end(str), begin(*jsonData));
    mocca::net::Message message;
    message.push_back(jsonData);
    for (auto& sharedData : *m_binary) {
        message.push_back(sharedData);
    }
    return message;
}