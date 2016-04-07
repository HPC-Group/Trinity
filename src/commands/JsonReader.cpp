#include "commands/JsonReader.h"

#include "commands/ISerializable.h"
#include "common/TrinityError.h"

#include <algorithm>
#include <cstring>

#define TRINITY_CHECK_JSON

using namespace trinity;

JsonReader::JsonReader(const std::string& json) {
    JsonCpp::Reader reader;
    if (!reader.parse(json, m_root)) {
        throw TrinityError("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
}

JsonReader::JsonReader(const mocca::net::Message& message) {
    std::string json(reinterpret_cast<const char*>(message[0]->data()), message[0]->size());
    for (unsigned int i = 1; i < message.size(); ++i) {
        m_binary.push_back(message[i]);
    }
    JsonCpp::Reader reader;
    if (!reader.parse(json, m_root)) {
        throw TrinityError("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
}

JsonReader::JsonReader(const JsonCpp::Value& root, SharedDataVec binary)
    : m_root(root)
    , m_binary(binary) {}

float JsonReader::getFloat(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asFloat();
}

double JsonReader::getDouble(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asDouble();
}

uint8_t JsonReader::getUInt8(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return static_cast<uint8_t>(m_root[key].asUInt()); // uint8 not supported by JSON
}

int32_t JsonReader::getInt32(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asInt();
}

uint32_t JsonReader::getUInt32(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asUInt();
}

int64_t JsonReader::getInt64(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asInt64();
}

uint64_t JsonReader::getUInt64(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asUInt64();
}

bool JsonReader::getBool(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asBool();
}

std::string JsonReader::getString(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    return m_root[key].asString();
}

void JsonReader::getSerializableImpl(const std::string& key, ISerializable& prototype) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    JsonReader subObject(m_root[key], m_binary);
    prototype.deserialize(subObject);
}

std::vector<float> JsonReader::getFloatVec(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<float> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asFloat());
    }
    return result;
}

std::vector<uint8_t> JsonReader::getUInt8Vec(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<uint8_t> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asUInt()); // uint8 not supported by JSON
    }
    return result;
}

std::vector<int32_t> JsonReader::getInt32Vec(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<int32_t> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asInt());
    }
    return result;
}

std::vector<uint64_t> JsonReader::getUInt64Vec(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<uint64_t> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asUInt64());
    }
    return result;
}

std::vector<bool> JsonReader::getBoolVec(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<bool> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asBool());
    }
    return result;
}

std::vector<std::string> JsonReader::getStringVec(const std::string& key) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<std::string> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asString());
    }
    return result;
}

std::vector<std::unique_ptr<ISerializable>> JsonReader::getSerializableVecImpl(const std::string& key,
                                                                               const ISerializable& prototype) const {
#ifdef TRINITY_CHECK_JSON
    if (!m_root.isMember(key))
        throw TrinityError("Invalid key '" + key + "'", __FILE__, __LINE__);
#endif
    std::vector<std::unique_ptr<ISerializable>> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        auto obj = prototype.clone();
        JsonReader subObject(m_root[key][i], m_binary);
        obj->deserialize(subObject);
        result.push_back(std::move(obj));
    }
    return result;
}

JsonReader::SharedDataVec JsonReader::getBinary() const {
    return m_binary;
}