#include "commands/JsonWriter.h"

#include "commands/ISerializable.h"

using namespace trinity;

JsonWriter::JsonWriter() {
    m_stack.push_back(&m_root);
}

JsonCpp::Value& JsonWriter::current() {
    return *m_stack.back();
}

void JsonWriter::append(const std::string& key, float value) {
    current()[key] = value;
}

void JsonWriter::append(const std::string& key, int value) {
    current()[key] = value;
}

void JsonWriter::append(const std::string& key, const std::string& value) {
    current()[key] = value;
}

void JsonWriter::append(const std::string& key, const ISerializable& obj) {
    JsonWriter subObject;
    obj.serialize(subObject);
    current()[key] = subObject.m_root;
}

void JsonWriter::append(const std::string& key, const std::vector<float>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        current()[key][i] = vec[i];
    }
}

void JsonWriter::append(const std::string& key, const std::vector<int>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        current()[key][i] = vec[i];
    }
}

void JsonWriter::append(const std::string& key, const std::vector<std::string>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        current()[key][i] = vec[i];
    }
}

void JsonWriter::append(const std::string& key, const std::vector<ISerializable*>& vec) {
    for (uint32_t i = 0; i < vec.size(); ++i) {
        JsonWriter subObject;
        vec[i]->serialize(subObject);
        current()[key][i] = subObject.m_root;
    }
}

void JsonWriter::push(const std::string& key) {
    current()[key] = JsonCpp::Value();
    m_stack.push_back(&current()[key]);
}

void JsonWriter::pop() {
    m_stack.pop_back();
}

mocca::ByteArray JsonWriter::write() const {
    JsonCpp::FastWriter writer;
    std::string str = writer.write(m_root);
    return mocca::ByteArray::createFromRaw(str.data(), str.size());
}