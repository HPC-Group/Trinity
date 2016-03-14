#include "commands/JsonReader.h"

#include "common/TrinityError.h"
#include "commands/ISerializable.h"

using namespace trinity;

const JsonCpp::Value& JsonReader::current() const {
    return *m_stack.back();
}

JsonReader::JsonReader(const std::string& str) {
    JsonCpp::Reader reader;
    if (!reader.parse(str, m_root)) {
        throw TrinityError("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
    m_stack.push_back(&m_root);
}

JsonReader::JsonReader(const JsonCpp::Value& root)
    : m_root(root) {
    m_stack.push_back(&m_root);
}

float JsonReader::getFloat(const std::string& key) const {
    return current()[key].asFloat();
}

int JsonReader::getInt(const std::string& key) const {
    return current()[key].asInt();
}

std::string JsonReader::getString(const std::string& key) const {
    return current()[key].asString();
}

void JsonReader::getSerializableImpl(const std::string& key, ISerializable& prototype) const {
    JsonReader subObject(current()[key]);
    prototype.deserialize(subObject);
}

std::vector<float> JsonReader::getFloatVec(const std::string& key) const {
    std::vector<float> result;
    for (uint32_t i = 0; i < current()[key].size(); ++i) {
        result.push_back(current()[key][i].asFloat());
    }
    return result;
}

std::vector<int> JsonReader::getIntVec(const std::string& key) const {
    std::vector<int> result;
    for (uint32_t i = 0; i < current()[key].size(); ++i) {
        result.push_back(current()[key][i].asInt());
    }
    return result;
}

std::vector<std::string> JsonReader::getStringVec(const std::string& key) const {
    std::vector<std::string> result;
    for (uint32_t i = 0; i < current()[key].size(); ++i) {
        result.push_back(current()[key][i].asString());
    }
    return result;
}

std::vector<std::unique_ptr<ISerializable>> JsonReader::getSerializableVecImpl(const std::string& key,
                                                                               const ISerializable& prototype) const {
    std::vector<std::unique_ptr<ISerializable>> result;
    for (uint32_t i = 0; i < current()[key].size(); ++i) {
        auto obj = prototype.clone();
        JsonReader subObject(current()[key][i]);
        obj->deserialize(subObject);
        result.push_back(std::move(obj));
    }
    return result;
}

void JsonReader::push(const std::string& key) {
    m_stack.push_back(&current()[key]);
}

void JsonReader::pop() {
    m_stack.pop_back();
}