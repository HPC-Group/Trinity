#include "commands/SimpleStringReader.h"

#include "commands/ISerializable.h"

using namespace trinity::commands;

SimpleStringReader::SimpleStringReader(const std::string& str)
    : m_stream(str) {}

float SimpleStringReader::getFloat(const std::string& key) const {
    float x;
    m_stream >> x;
    return x;
}

int SimpleStringReader::getInt(const std::string& key) const {
    int x;
    m_stream >> x;
    return x;
}

std::string SimpleStringReader::getString(const std::string& key) const {
    std::string x;
    m_stream >> x;
    return x;
}

void SimpleStringReader::getSerializable(const std::string& key, ISerializable& obj) const {
    obj.deserialize(*this);
}