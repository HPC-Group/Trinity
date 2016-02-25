#include "commands/JsonReader.h"

#include "commands/ISerializable.h"

using namespace trinity::commands;

JsonReader::JsonReader(const std::string& str) {
    JsonCpp::Reader reader;
    if (!reader.parse(str, m_root)) {
        throw mocca::Error("Error parsing JSON: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
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

void JsonReader::getSerializable(const std::string& key, ISerializable& obj) const {
    JsonReader subObject(m_root[key]);
    obj.deserialize(subObject);
}