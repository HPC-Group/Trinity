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

mocca::ByteArray SimpleStringWriter::write() const {
    std::string str = m_stream.str();
    return mocca::ByteArray::createFromRaw(str.data(), str.size());
}