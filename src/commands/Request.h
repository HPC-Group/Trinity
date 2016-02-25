#pragma once

#include "commands/ISerializable.h"
#include "commands/Vcl.h"
#include "commands/Reply.h"
#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

#include "mocca/base/Error.h"
#include "mocca/base/ByteArray.h"

#include <memory>

namespace trinity {
namespace commands {

class Request : public ISerializable {
public:
    Request() : m_rid(0), m_sid(0) {}
    Request(int rid, int sid)
        : m_rid(rid)
        , m_sid(sid) {}

    virtual VclType getType() const = 0;

    int getRid() const { return m_rid; }
    int getSid() const { return m_sid; }

    static std::unique_ptr<Request> createFromByteArray(mocca::ByteArray& byteArray);
    static mocca::ByteArray createByteArray(const Request& request);

private:
    virtual void serializeParams(ISerialWriter& writer) const = 0;
    void serialize(ISerialWriter& writer) const override {
        writer.append("rid", m_rid);
        writer.append("sid", m_sid);
        serializeParams(writer);
    }

    virtual void deserializeParams(const ISerialReader& reader) = 0;
    void deserialize(const ISerialReader& reader) override {
        m_rid = reader.getInt("rid");
        m_sid = reader.getInt("sid");
        deserializeParams(reader);
    }

private:
    int m_rid;
    int m_sid;
};

template <typename Interface> class RequestTemplate : public Request {
    static_assert(std::is_base_of<ISerializable, typename Interface::RequestParams>::value,
                  "Type Interface::RequestParams must be ISerializable");

public:
    using RequestParams = typename Interface::RequestParams;
    using Ifc = Interface;
    using ReplyType = ReplyTemplate<Interface>;

    RequestTemplate() = default;
    RequestTemplate(const RequestParams& params, int rid, int sid)
        : Request(rid, sid), m_params(params) {}

    VclType getType() const { return Interface::Type; }
    RequestParams getParams() const { return m_params; }

private:
    void serializeParams(ISerialWriter& writer) const override {
        writer.append("params", m_params);
    }

    void deserializeParams(const ISerialReader& reader) override {
        reader.getSerializable("params", m_params);
    }

private:
    RequestParams m_params;
};
}
}
