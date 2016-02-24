#include "commands/JsonObject.h"

#include "commands/ISerializable.h"

using namespace trinity::commands;

void JsonObject::append(const std::string& key, float value) {
    m_root[key] = value;
}

void JsonObject::append(const std::string& key, int value) {
    m_root[key] = value;
}

void JsonObject::append(const std::string& key, const std::string& value) {
    m_root[key] = value;
}

void JsonObject::append(const std::string& key, const ISerializable& obj) {
    JsonObject subObject;
    obj.serialize(subObject);
    m_root[key] = subObject.m_root;
}

float JsonObject::getFloat(const std::string& key) const {
    return m_root[key].asFloat();
}

int JsonObject::getInt(const std::string& key) const {
    return m_root[key].asInt();
}

std::string JsonObject::getString(const std::string& key) const {
    return m_root[key].asString();
}

void JsonObject::getSerializable(const std::string& key, ISerializable& obj) const {
    JsonObject subObject;
    subObject.m_root = m_root[key];
    obj.deserialize(subObject);
}

void JsonObject::writeTo(std::ostream& stream) {
    JsonCpp::StyledStreamWriter writer;
    writer.write(stream, m_root);
}

void JsonObject::readFrom(std::istream& stream) {
    JsonCpp::Reader reader;
    if (!reader.parse(stream, m_root)) {
        throw mocca::Error("Error reading serialized object: " + reader.getFormattedErrorMessages(), __FILE__, __LINE__);
    }
}