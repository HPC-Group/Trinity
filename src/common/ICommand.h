#pragma once
#include "ISerializable.h"
#include "Commands.h"

namespace trinity {
namespace common {

// todo: this loses more and more pure virtual functions, keep an eye on that
class ICommand : public ISerializable {
    
public:
    ICommand(int sid, int rid) : m_sid(sid), m_rid(rid) {}
    virtual ~ICommand(){}
    
    virtual VclType getType() const = 0;
    
    virtual void serialize(ISerialObject& serial) const {
        serial.setType(getType());
        serial.append("sid", m_sid);
        serial.append("rid", m_rid);
    }
    virtual void deserialize(ISerialObject& serial) {
        m_sid = serial.getInt("sid");
        m_rid = serial.getInt("rid");
    }
    
    int getSid() const { return m_sid; }
    int getRid() const { return m_rid; }
    
protected:
    int m_sid;
    int m_rid;
    Vcl m_vcl;
};
}
}