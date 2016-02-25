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
    void getSerializable(const std::string& key, ISerializable& obj) const override;

private:
    mutable std::stringstream m_stream;
};
}
}