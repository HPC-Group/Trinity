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

void JsonWriter::append(const std::string& key, const std::vector<float>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::append(const std::string& key, const std::vector<int>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::append(const std::string& key, const std::vector<std::string>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::append(const std::string& key, const std::vector<std::unique_ptr<ISerializable>>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        JsonWriter subObject;
        vec[i]->serialize(subObject);
        m_root[key][i] = subObject.m_root;
    }
}

mocca::ByteArray JsonWriter::write() const {
    JsonCpp::FastWriter writer;
    std::string str = writer.write(m_root);
    return mocca::ByteArray::createFromRaw(str.data(), str.size());
}