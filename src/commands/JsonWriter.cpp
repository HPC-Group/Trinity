#include "commands/JsonWriter.h"

#include "common/MemBlockPool.h"
#include "commands/ISerializable.h"

#include "blosc/blosc/blosc.h"

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
    auto jsonData = std::make_shared<std::vector<uint8_t>>();
    MessageBuf buf(*jsonData);
    std::ostream os(&buf);
    JsonCpp::StyledStreamWriter streamWriter;
    streamWriter.write(os, m_root);
    mocca::net::Message message;
    message.push_back(jsonData);
    for (auto& sharedData : *m_binary) {
        auto compressed = MemBlockPool::instance().get(sharedData->size() + BLOSC_MAX_OVERHEAD);
        compressed->resize(sharedData->size() + BLOSC_MAX_OVERHEAD);
        auto resSize = blosc_compress_ctx(4, 1, 16, sharedData->size(), sharedData->data(), compressed->data(), compressed->size(), "blosclz", 0, 6);
        message.push_back(compressed);
    }
    return message;
}