#include "commands/SimpleStringWriter.h"

#include "commands/ISerializable.h"

using namespace trinity;

void SimpleStringWriter::appendFloat(const std::string& key, float value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendDouble(const std::string& key, double value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendInt(const std::string& key, uint8_t value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendInt(const std::string& key, int32_t value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendInt(const std::string& key, uint32_t value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendInt(const std::string& key, int64_t value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendInt(const std::string& key, uint64_t value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendBool(const std::string& key, bool value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendString(const std::string& key, const std::string& value) {
    m_stream << value << " ";
}

void SimpleStringWriter::appendObject(const std::string& key, const ISerializable& obj) {
    obj.serialize(*this);
}

void SimpleStringWriter::appendFloatVec(const std::string& key, const std::vector<float>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::appendIntVec(const std::string& key, const std::vector<uint8_t>& vec) {
    // fixme
}

void SimpleStringWriter::appendIntVec(const std::string& key, const std::vector<int32_t>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::appendIntVec(const std::string& key, const std::vector<uint64_t>& vec) {
    // fixme
}

void SimpleStringWriter::appendBoolVec(const std::string& key, const std::vector<bool>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::appendStringVec(const std::string& key, const std::vector<std::string>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        m_stream << vec[i] << " ";
    }
}

void SimpleStringWriter::appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) {
    m_stream << vec.size() << " ";
    for (size_t i = 0; i < vec.size(); ++i) {
        vec[i]->serialize(*this);
    }
}

void SimpleStringWriter::appendBinary(std::shared_ptr<const std::vector<uint8_t>> binary) {
    // fixme dmc
}

mocca::ByteArray SimpleStringWriter::write() const {
    std::string str = m_stream.str();
    return mocca::ByteArray::createFromRaw(str.data(), str.size());
}

mocca::net::Message SimpleStringWriter::writeMessage() const {
    // fixme dmc
    return mocca::net::Message();
}