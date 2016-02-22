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
    subObject.append(key, Vcl::instance().toString(obj.getType()));
    obj.serialize(subObject);
    m_root[key] = subObject.m_root;
}

float JsonObject::getFloat(const std::string& key) {
    return m_root[key].asFloat();
}

int JsonObject::getInt(const std::string& key) {
    return m_root[key].asInt();
}

std::string JsonObject::getString(const std::string& key) {
    return m_root[key].asString();
}

void JsonObject::getSerializable(const std::string& key, ISerializable& obj) {
    JsonObject subObject;
    subObject.m_root = m_root[key];
    std::string subclassType = subObject.getString(key);
    std::string expectedSubclassType = Vcl::instance().toString(obj.getType());
    if (subclassType != expectedSubclassType) {
        throw mocca::Error("subclass type is incorrect (is " + subclassType + ", should be " + expectedSubclassType + ")", __FILE__,
                           __LINE__);
    }
    obj.deserialize(subObject);
}

void JsonObject::writeTo(std::ostream& stream) {
    JsonCpp::StyledStreamWriter writer;
    writer.write(stream, m_root);
}

void JsonObject::readFrom(std::istream& stream) {
    JsonCpp::Reader reader;
    reader.parse(stream, m_root);
}