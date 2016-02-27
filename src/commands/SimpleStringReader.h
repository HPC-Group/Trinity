#pragma once

#include "commands/ISerialReader.h"

#include <sstream>

namespace trinity {
namespace commands {

class SimpleStringReader : public ISerialReader {
public:
    SimpleStringReader(const std::string& str);

    float getFloat(const std::string& key) const override;
    int getInt(const std::string& key) const override;
    std::string getString(const std::string& key) const override;

    std::vector<float> getFloatVec(const std::string& key) const override;
    std::vector<int> getIntVec(const std::string& key) const override;
    std::vector<std::string> getStringVec(const std::string& key) const override;

private:
    void getSerializableImpl(const std::string& key, ISerializable& obj) const override;
    std::vector<std::unique_ptr<ISerializable>> getSerializableVecImpl(const std::string& key, const ISerializable& prototype) const override;

private:
    mutable std::stringstream m_stream;
};
}
}