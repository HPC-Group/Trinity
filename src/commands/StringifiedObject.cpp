#include "StringifiedObject.h"

#include "ISerializable.h"

#include <iostream>

using namespace trinity::commands;

void StringifiedObject::writeTo(std::ostream& stream) {
    std::string typestring = Vcl::instance().toString(m_type);
    stream << typestring << " ";
    stream << m_stream.str();
}

void StringifiedObject::readFrom(std::istream& stream) {
    std::stringstream& ss = static_cast<std::stringstream&>(stream);
    std::string typestring;
    ss >> typestring;
    m_type = Vcl::instance().toType(typestring);
    m_stream << ss.str();
    m_stream >> typestring; // getting rid of typestring since we dont need it
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

void StringifiedObject::getSerializable(const std::string& key, ISerializable& obj) {
    std::string subclassType = getString(key);
    std::string expectedSubclassType = Vcl::instance().toString(obj.getType());
    if (subclassType != expectedSubclassType) {
        throw mocca::Error("subclass type is incorrect (is " + subclassType + ", should be " +
                               expectedSubclassType + ")",
                           __FILE__, __LINE__);
    }
    obj.deserialize(*this);
}

void StringifiedObject::append(const std::string& key, const ISerializable& obj) {
    append(key, Vcl::instance().toString(obj.getType()));
    obj.serialize(*this);
}
