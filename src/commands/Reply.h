#pragma once

#include "commands/ISerializable.h"
#include "commands/Vcl.h"
#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

#include "mocca/base/Error.h"
#include "mocca/base/ByteArray.h"

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

    static std::unique_ptr<Reply> createFromByteArray(mocca::ByteArray& byteArray);
    static mocca::ByteArray createByteArray(const Reply& request);

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
    void serializeParams(ISerialWriter& writer) const override {
        writer.append("params", m_params);
    }

    void deserializeParams(const ISerialReader& reader) override {
        reader.getSerializable("params", m_params);
    }

private:
    ReplyParams m_params;
};
}
}
