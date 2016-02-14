#include "StringifiedObject.h"
#include "ISerializable.h"
#include <iostream>

using namespace trinity::common;

void StringifiedObject::writeTo(std::ostream& stream) {
    std::string typestring = m_vcl.toString(m_type);
    stream << typestring << " ";
    stream << m_stream.str();
}

void StringifiedObject::readFrom(std::istream& stream) {
    std::stringstream& ss = static_cast<std::stringstream&>(stream);
    std::string typestring;
    ss >> typestring;
    m_type = m_vcl.toType(typestring);
    m_stream << ss.str();
    m_stream >> typestring;  // getting rid of typestring since we dont need it
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

void StringifiedObject::append(const std::string& key, const ISerializable* obj) {
    append(key, m_vcl.toString(obj->getType()));
}
