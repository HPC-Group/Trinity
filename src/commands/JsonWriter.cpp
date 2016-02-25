#include "commands/JsonWriter.h"

#include "commands/ISerializable.h"

using namespace trinity::commands;

void JsonWriter::append(const std::string& key, float value) {
    m_root[key] = value;
}

void JsonWriter::append(const std::string& key, int value) {
    m_root[key] = value;
}

void JsonWriter::append(const std::string& key, const std::string& value) {
    m_root[key] = value;
}

void JsonWriter::append(const std::string& key, const ISerializable& obj) {
    JsonWriter subObject;
    obj.serialize(subObject);
    m_root[key] = subObject.m_root;
}

mocca::ByteArray JsonWriter::write() const {
    JsonCpp::FastWriter writer;
    std::string str = writer.write(m_root);
    return mocca::ByteArray::createFromRaw(str.data(), str.size());
}