#pragma once

#include "commands/ISerialWriter.h"

#include "thirdparty/jsoncpp/json.h"

namespace trinity {

class JsonWriter : public ISerialWriter {
public:
    JsonWriter();

    void appendFloat(const std::string& key, float value) override;
    void appendDouble(const std::string& key, double value) override;
    void appendInt(const std::string& key, uint8_t value) override;
    void appendInt(const std::string& key, int32_t value) override;
    void appendInt(const std::string& key, uint32_t value) override;
    void appendInt(const std::string& key, int64_t value) override;
    void appendInt(const std::string& key, uint64_t value) override;
    void appendBool(const std::string& key, bool value) override;
    void appendString(const std::string& key, const std::string& value) override;
    void appendObject(const std::string& key, const ISerializable& obj) override;

    void appendFloatVec(const std::string& key, const std::vector<float>& vec) override;
    void appendIntVec(const std::string& key, const std::vector<uint8_t>& vec) override;
    void appendIntVec(const std::string& key, const std::vector<int32_t>& vec) override;
    void appendIntVec(const std::string& key, const std::vector<uint64_t>& vec) override;
    void appendBoolVec(const std::string& key, const std::vector<bool>& vec) override;
    void appendStringVec(const std::string& key, const std::vector<std::string>& vec) override;
    void appendObjectVec(const std::string& key, const std::vector<ISerializable*>& vec) override;

    void appendBinary(std::shared_ptr<std::vector<uint8_t>> binary) override;

    mocca::net::Message writeMessage() const override;

private:
    class MessageBuf : public std::basic_streambuf<char, std::char_traits<char>> {
    public:
        MessageBuf(std::vector<uint8_t>& vec)
            : m_vec(vec) {
            vec.clear();
            auto casted = reinterpret_cast<char*>(vec.data());
            setp(casted, casted);
        }

        int overflow(int ch) override {
            if (ch != traits_type::eof()) {
                m_vec.push_back(ch);
                pbump(1);
                return ch;
            }
            return traits_type::eof();
        }

    private:
        std::vector<uint8_t>& m_vec;
    };

    using SharedDataVec = std::vector<std::shared_ptr<std::vector<uint8_t>>>;
    JsonWriter(std::shared_ptr<SharedDataVec> binary);

private:
    JsonCpp::Value m_root;
    std::shared_ptr<SharedDataVec> m_binary;
};
}