#include "commands/SimpleStringReader.h"

#include "commands/ISerializable.h"

using namespace trinity;

SimpleStringReader::SimpleStringReader(const std::string& str)
    : m_stream(str) {}

SimpleStringReader::SimpleStringReader(mocca::ByteArray& data)
    : m_stream(data.read(data.size())) {
    // fixme dmc
}

float SimpleStringReader::getFloat(const std::string& key) const {
    float x;
    m_stream >> x;
    return x;
}

double SimpleStringReader::getDouble(const std::string& key) const {
    double x;
    m_stream >> x;
    return x;
}

uint8_t SimpleStringReader::getUInt8(const std::string& key) const {
    uint8_t x;
    m_stream >> x;
    return x;
}

int32_t SimpleStringReader::getInt32(const std::string& key) const {
    int32_t x;
    m_stream >> x;
    return x;
}

uint32_t SimpleStringReader::getUInt32(const std::string& key) const {
    uint32_t x;
    m_stream >> x;
    return x;
}

int64_t SimpleStringReader::getInt64(const std::string& key) const {
    int64_t x;
    m_stream >> x;
    return x;
}

uint64_t SimpleStringReader::getUInt64(const std::string& key) const {
    uint64_t x;
    m_stream >> x;
    return x;
}

bool SimpleStringReader::getBool(const std::string& key) const {
    bool x;
    m_stream >> x;
    return x;
}

std::string SimpleStringReader::getString(const std::string& key) const {
    std::string x;
    m_stream >> x;
    return x;
}

void SimpleStringReader::getSerializableImpl(const std::string& key, ISerializable& obj) const {
    obj.deserialize(*this);
}

std::vector<float> SimpleStringReader::getFloatVec(const std::string& key) const {
    int32_t size;
    m_stream >> size;
    std::vector<float> result(size);
    for (int32_t i = 0; i < size; ++i) {
        m_stream >> result[i];
    }
    return result;
}

std::vector<int32_t> SimpleStringReader::getInt32Vec(const std::string& key) const {
    int32_t size;
    m_stream >> size;
    std::vector<int32_t> result(size);
    for (int32_t i = 0; i < size; ++i) {
        m_stream >> result[i];
    }
    return result;
}

std::vector<uint8_t> SimpleStringReader::getUInt8Vec(const std::string& key) const {
    int32_t size;
    m_stream >> size;
    std::vector<uint8_t> result(size);
    for (int32_t i = 0; i < size; ++i) {
        m_stream >> result[i];
    }
    return result;
}

std::vector<bool> SimpleStringReader::getBoolVec(const std::string& key) const {
    int32_t size;
    m_stream >> size;
    std::vector<bool> result(size);
    for (int32_t i = 0; i < size; ++i) {
        bool val;
        m_stream >> val;
        result[i] = val;
    }
    return result;
}

std::vector<std::string> SimpleStringReader::getStringVec(const std::string& key) const {
    int32_t size;
    m_stream >> size;
    std::vector<std::string> result(size);
    for (int32_t i = 0; i < size; ++i) {
        m_stream >> result[i];
    }
    return result;
}

std::vector<std::unique_ptr<ISerializable>> SimpleStringReader::getSerializableVecImpl(const std::string& key,
                                                                                       const ISerializable& prototype) const {
    int32_t size;
    m_stream >> size;
    std::vector<std::unique_ptr<ISerializable>> result;
    for (int32_t i = 0; i < size; ++i) {
        auto obj = prototype.clone();
        obj->deserialize(*this);
        result.push_back(std::move(obj));
    }
    return result;
}

const std::vector<uint8_t>& SimpleStringReader::getBinary() const {
    return m_binary;
}