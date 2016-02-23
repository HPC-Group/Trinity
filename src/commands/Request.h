#pragma once

#include "commands/ISerializable.h"
#include "commands/Vcl.h"

#include "mocca/base/Error.h"

#include <memory>

namespace trinity {
namespace commands {
template <typename Interface> class Request : public ISerializable {
    static_assert(std::is_base_of<ISerializable, typename Interface::RequestParams>::value, "Type Interface::RequestParams must be ISerializable");
    using RequestParams = typename Interface::RequestParams;

public:
    Request() = default;
    Request(const RequestParams& params)
        : m_params(params)
        , m_rid(0) {}

    int getRid() const { return m_rid; }

    VclType getType() const { return Interface::getType(); }
    RequestParams getParams() const { return m_params; }

    void serialize(ISerialObject& serial) const override {
        serial.append("type", Vcl::instance().toString(getType()));
        serial.append("rid", m_rid);
        serial.append("params", m_params);
    }

    void deserialize(ISerialObject& serial) override {
        auto type = Vcl::instance().toType(serial.getString("type"));
        if (type != getType()) {
            throw mocca::Error("Request type mismatch", __FILE__, __LINE__);
        }
        m_rid = serial.getInt("rid");
        serial.getSerializable("params", m_params);
    }
    
private:
    RequestParams m_params;
    int m_rid;
};
}
}
