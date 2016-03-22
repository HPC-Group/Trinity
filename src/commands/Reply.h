#pragma once

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"
#include "commands/ISerializable.h"
#include "commands/Vcl.h"

#include "mocca/base/ByteArray.h"
#include "mocca/base/Error.h"

#include <memory>
#include <sstream>

namespace trinity {

class Reply : public ISerializable {
public:
    Reply()
        : m_rid(0)
        , m_sid(0) {}
    Reply(int rid, int sid, std::shared_ptr<const std::vector<uint8_t>> binary)
        : m_rid(rid)
        , m_sid(sid)
        , m_binary(binary) {}

    virtual VclType getType() const = 0;
    virtual std::string toString() const = 0;

    int getRid() const { return m_rid; }
    int getSid() const { return m_sid; }

    std::shared_ptr<const std::vector<uint8_t>> getBinary() const { return m_binary; }

    static std::unique_ptr<Reply> createFromByteArray(mocca::ByteArray& byteArray);
    static mocca::ByteArray createByteArray(const Reply& request);

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

std::ostream& operator<<(std::ostream& os, const Reply& obj);


template <typename Interface> class ReplyTemplate : public Reply {
    static_assert(std::is_base_of<ISerializable, typename Interface::ReplyParams>::value,
                  "Type Interface::ReplyParams must be ISerializable");

public:
    using ReplyParams = typename Interface::ReplyParams;
    using Ifc = Interface;

    ReplyTemplate() = default;
    ReplyTemplate(const ReplyParams& params, int rid, int sid, std::shared_ptr<const std::vector<uint8_t>> binary = nullptr)
        : Reply(rid, sid, binary)
        , m_params(params) {}

    VclType getType() const override { return Interface::Type; }
    const ReplyParams& getParams() const { return m_params; } // returning reference for performance reasons, be careful!

    std::unique_ptr<ISerializable> clone() const override { return mocca::make_unique<ReplyTemplate<Interface>>(*this); }

    std::string toString() const override {
        std::stringstream stream;
        stream << "type: " << Vcl::instance().toString(getType()) << "; rid: " << getRid() << "; sid: " << getSid() << "; params: { "
               << m_params << " }";
        return stream.str();
    }

private:
    void serializeParams(ISerialWriter& writer) const override { writer.appendObject("params", m_params); }

    void deserializeParams(const ISerialReader& reader) override { m_params = reader.getSerializable<ReplyParams>("params"); }

private:
    ReplyParams m_params;
};
}
