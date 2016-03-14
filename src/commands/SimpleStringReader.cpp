#include "commands/SimpleStringReader.h"

#include "commands/ISerializable.h"

using namespace trinity;

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

std::vector<int> SimpleStringReader::getIntVec(const std::string& key) const {
    int32_t size;
    m_stream >> size;
    std::vector<int> result(size);
    for (int32_t i = 0; i < size; ++i) {
        m_stream >> result[i];
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

void SimpleStringReader::push(const std::string& key) {
    std::string dummy;
    m_stream >> dummy;
}

void SimpleStringReader::pop() {
}