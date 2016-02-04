#include "StringifiedObject.h"
#include <iostream>

using namespace trinity::common;

void StringifiedObject::writeTo(std::ostream& stream) {
    stream << m_stream.str();
}

void StringifiedObject::readFrom(std::istream& stream) {
    std::stringstream& ss = static_cast<std::stringstream&>(stream);
    m_stream << ss.str();
}


void StringifiedObject::append(const std::string& key, float value) {
    m_stream << value << " ";
}

void StringifiedObject::append(const std::string& key, int value) {
    m_stream << value << " ";
}

void StringifiedObject::append(const std::string& key, const std::string& value) {
    m_stream << value << " ";
}

float StringifiedObject::getFloat(const std::string& key) {
    float x;
    m_stream >> x;
    return x;
}

int StringifiedObject::getInt(const std::string& key) {
    int x;
    m_stream >> x;
    return x;
}

std::string StringifiedObject::getString(const std::string& key) {
    std::string x;
    m_stream >> x;
    return x;
}