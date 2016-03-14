#include "commands/JsonWriter.h"

#include "commands/ISerializable.h"

using namespace trinity;

void JsonWriter::appendFloat(const std::string& key, float value) {
    m_root[key] = value;
}

void JsonWriter::appendInt(const std::string& key, int value) {
    m_root[key] = value;
}

void JsonWriter::appendBool(const std::string& key, bool value) {
    m_root[key] = value;
}

void JsonWriter::appendString(const std::string& key, const std::string& value) {
    m_root[key] = value;
}

void JsonWriter::appendObject(const std::string& key, const ISerializable& obj) {
    JsonWriter subObject;
    obj.serialize(subObject);
    m_root[key] = subObject.m_root;
}

void JsonWriter::appendFloatVec(const std::string& key, const std::vector<float>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendIntVec(const std::string& key, const std::vector<int>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendBoolVec(const std::string& key, const std::vector<bool>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = JsonCpp::Value(vec[i]);
    }
}

void JsonWriter::appendStringVec(const std::string& key, const std::vector<std::string>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        m_root[key][i] = vec[i];
    }
}

void JsonWriter::appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) {
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