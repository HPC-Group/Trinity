#pragma once

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"
#include "commands/ISerializable.h"
#include "commands/Reply.h"
#include "commands/Vcl.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/Error.h"

#include <memory>
#include <sstream>

namespace trinity {

class Request : public ISerializable {
public:
    Request()
        : m_rid(0)
        , m_sid(0) {}
    Request(int rid, int sid, std::shared_ptr<const std::vector<uint8_t>> binary)
        : m_rid(rid)
        , m_sid(sid)
        , m_binary(binary) {}

    virtual VclType getType() const = 0;
    virtual std::string toString() const = 0;

    int getRid() const { return m_rid; }
    int getSid() const { return m_sid; }

    std::shared_ptr<const std::vector<uint8_t>> getBinary() const { return m_binary; }

    static std::unique_ptr<Request> createFromByteArray(mocca::ByteArray& byteArray);
    static mocca::ByteArray createByteArray(const Request& request);

private:
    virtual void serializeParams(ISerialWriter& writer) const = 0;
    void serialize(ISerialWriter& writer) const override {
        writer.appendInt("rid", m_rid);
        writer.appendInt("sid", m_sid);
        serializeParams(writer);
        if (m_binary != nullptr) {
            writer.appendBinary(*m_binary);
        }
    }

    virtual void deserializeParams(const ISerialReader& reader) = 0;
    void deserialize(const ISerialReader& reader) override {
        m_rid = reader.getInt32("rid");
        m_sid = reader.getInt32("sid");
        deserializeParams(reader);
        m_binary = reader.getBinary();
    }

private:
    int m_rid;
    int m_sid;
    std::shared_ptr<const std::vector<uint8_t>> m_binary;
};

std::ostream& operator<<(std::ostream& os, const Request& obj);


template <typename Interface> class RequestTemplate : public Request {
    static_assert(std::is_base_of<ISerializable, typename Interface::RequestParams>::value,
                  "Type Interface::RequestParams must be ISerializable");

public:
    using RequestParams = typename Interface::RequestParams;
    using Ifc = Interface;
    using ReplyType = ReplyTemplate<Interface>;

    RequestTemplate() = default;
    RequestTemplate(const RequestParams& params, int rid, int sid, std::shared_ptr<const std::vector<uint8_t>> binary = nullptr)
        : Request(rid, sid, binary)
        , m_params(params) {}

    VclType getType() const override { return Interface::Type; }
    RequestParams getParams() const { return m_params; }

    std::unique_ptr<ISerializable> clone() const override { return mocca::make_unique<RequestTemplate<Interface>>(*this); }

    std::string toString() const override {
        std::stringstream stream;
        stream << "type: " << Vcl::instance().toString(getType()) << "; rid: " << getRid() << "; sid: " << getSid() << "; params: { "
               << m_params << " }";
        return stream.str();
    }

private:
    void serializeParams(ISerialWriter& writer) const override { writer.appendObject("params", m_params); }

    void deserializeParams(const ISerialReader& reader) override { m_params = reader.getSerializable<RequestParams>("params"); }

private:
    RequestParams m_params;
};
}
