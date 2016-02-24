#pragma once

#include "commands/ISerializable.h"
#include "commands/Vcl.h"

#include "mocca/base/Error.h"

#include <memory>

namespace trinity {
namespace commands {

class Reply : public ISerializable {
public:
    Reply() : m_rid(0), m_sid(0) {}
    Reply(int rid, int sid)
        : m_rid(rid)
        , m_sid(sid) {}

    virtual VclType getType() const = 0;

    int getRid() const { return m_rid; }
    int getSid() const { return m_sid; }

    static std::unique_ptr<Reply> createFromSerialObject(const ISerialObject& serial);
    static std::unique_ptr<ISerialObject> createSerialObject(const Reply& request);

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

template <typename Interface> class ReplyTemplate : public Reply {
    static_assert(std::is_base_of<ISerializable, typename Interface::ReplyParams>::value,
                  "Type Interface::ReplyParams must be ISerializable");

public:
    using ReplyParams = typename Interface::ReplyParams;
    using Ifc = Interface;

    ReplyTemplate() = default;
    ReplyTemplate(const ReplyParams& params, int rid, int sid)
        : Reply(rid, sid)
        , m_params(params) {}

    VclType getType() const override { return Interface::Type; }
    ReplyParams getParams() const { return m_params; }

private:
    void serializeParams(ISerialObject& serial) const override {
        serial.append("params", m_params);
    }

    void deserializeParams(const ISerialObject& serial) override {
        serial.getSerializable("params", m_params);
    }

private:
    ReplyParams m_params;
};
}
}
