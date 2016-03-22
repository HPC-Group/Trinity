#include "commands/JsonReader.h"

#include "commands/ISerializable.h"
#include "common/TrinityError.h"

#include <iterator>

using namespace trinity;

JsonReader::JsonReader(const std::string& json) {
    JsonCpp::Reader reader;
    if (!reader.parse(json, m_root)) {
        throw TrinityError("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
}

JsonReader::JsonReader(mocca::ByteArray& data) {
    auto jsonSize = data.read<uint32_t>();
    auto json = data.read(jsonSize);
    JsonCpp::Reader reader;
    if (!reader.parse(json, m_root)) {
        throw TrinityError("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
    uint32_t binarySize = data.size() - jsonSize - sizeof(uint32_t);
    m_binary.reserve(binarySize);
    std::copy(data.data() + jsonSize + sizeof(uint32_t), data.data() + data.size(), std::back_inserter(m_binary));
}

JsonReader::JsonReader(const JsonCpp::Value& root)
    : m_root(root) {}


float JsonReader::getFloat(const std::string& key) const {
    return m_root[key].asFloat();
}

double JsonReader::getDouble(const std::string& key) const {
    return m_root[key].asDouble();
}

uint8_t JsonReader::getUInt8(const std::string& key) const {
    return static_cast<uint8_t>(m_root[key].asUInt()); // uint8 not supported by JSON
}

int32_t JsonReader::getInt32(const std::string& key) const {
    return m_root[key].asInt();
}

uint32_t JsonReader::getUInt32(const std::string& key) const {
    return m_root[key].asUInt();
}

int64_t JsonReader::getInt64(const std::string& key) const {
    return m_root[key].asInt64();
}

uint64_t JsonReader::getUInt64(const std::string& key) const {
    return m_root[key].asUInt64();
}

bool JsonReader::getBool(const std::string& key) const {
    return m_root[key].asBool();
}

std::string JsonReader::getString(const std::string& key) const {
    return m_root[key].asString();
}

void JsonReader::getSerializableImpl(const std::string& key, ISerializable& prototype) const {
    JsonReader subObject(m_root[key]);
    prototype.deserialize(subObject);
}

std::vector<float> JsonReader::getFloatVec(const std::string& key) const {
    std::vector<float> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asFloat());
    }
    return result;
}

std::vector<int32_t> JsonReader::getInt32Vec(const std::string& key) const {
    std::vector<int32_t> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asInt()); 
    }
    return result;
}

std::vector<uint8_t> JsonReader::getUInt8Vec(const std::string& key) const {
    std::vector<uint8_t> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asUInt()); // uint8 not supported by JSON
    }
    return result;
}

std::vector<bool> JsonReader::getBoolVec(const std::string& key) const {
    std::vector<bool> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asBool());
    }
    return result;
}

std::vector<std::string> JsonReader::getStringVec(const std::string& key) const {
    std::vector<std::string> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asString());
    }
    return result;
}

std::vector<std::unique_ptr<ISerializable>> JsonReader::getSerializableVecImpl(const std::string& key,
                                                                               const ISerializable& prototype) const {
    std::vector<std::unique_ptr<ISerializable>> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        auto obj = prototype.clone();
        JsonReader subObject(m_root[key][i]);
        obj->deserialize(subObject);
        result.push_back(std::move(obj));
    }
    return result;
}

const std::vector<uint8_t>& JsonReader::getBinary() const {
    return m_binary;
}