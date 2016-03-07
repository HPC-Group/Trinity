#include "commands/JsonReader.h"

#include "common/TrinityError.h"
#include "commands/ISerializable.h"

using namespace trinity::commands;
using namespace trinity::common;


JsonReader::JsonReader(const std::string& str) {
    JsonCpp::Reader reader;
    if (!reader.parse(str, m_root)) {
        throw TrinityError("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
}

JsonReader::JsonReader(const JsonCpp::Value& root)
    : m_root(root) {}


float JsonReader::getFloat(const std::string& key) const {
    return m_root[key].asFloat();
}

int JsonReader::getInt(const std::string& key) const {
    return m_root[key].asInt();
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

std::vector<int> JsonReader::getIntVec(const std::string& key) const {
    std::vector<int> result;
    for (uint32_t i = 0; i < m_root[key].size(); ++i) {
        result.push_back(m_root[key][i].asInt());
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