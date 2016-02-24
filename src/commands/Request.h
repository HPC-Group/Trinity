#pragma once

#include "commands/ISerializable.h"
#include "commands/Vcl.h"

#include "mocca/base/Error.h"

#include "commands/Reply.h"

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

    static std::unique_ptr<Request> createFromSerialObject(const ISerialObject& serial);
    static std::unique_ptr<ISerialObject> createSerialObject(const Request& request);

private:
    virtual void serializeParams(ISerialObject& serial) const = 0;
    void serialize(ISerialObject& serial) const override {
        serial.append("rid", m_rid);
        serial.append("sid", m_sid);
        serializeParams(serial);
    }

    virtual void deserializeParams(const ISerialObject& serial) = 0;
    void deserialize(const ISerialObject& serial) override {
        m_rid = serial.getInt("rid");
        m_sid = serial.getInt("sid");
        deserializeParams(serial);
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
    void serializeParams(ISerialObject& serial) const override {
        serial.append("params", m_params);
    }

    void deserializeParams(const ISerialObject& serial) override {
        serial.getSerializable("params", m_params);
    }

private:
    RequestParams m_params;
};
}
}
