#include "commands/SimpleStringWriter.h"

#include "commands/ISerializable.h"

using namespace trinity::commands;

void SimpleStringWriter::append(const std::string& key, float value) {
    m_stream << value << " ";
}

void SimpleStringWriter::append(const std::string& key, int value) {
    m_stream << value << " ";
}

void SimpleStringWriter::append(const std::string& key, const std::string& value) {
    m_stream << value << " ";
}

void SimpleStringWriter::append(const std::string& key, const ISerializable& obj) {
    obj.serialize(*this);
}

void SimpleStringWriter::append(const std::string& key, const std::vector<float>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::append(const std::string& key, const std::vector<int>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::append(const std::string& key, const std::vector<std::string>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::append(const std::string& key, const std::vector<std::unique_ptr<ISerializable>>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i]->serialize(*this);
    }
}

mocca::ByteArray SimpleStringWriter::write() const {
    std::string str = m_stream.str();
    return mocca::ByteArray::createFromRaw(str.data(), str.size());
}