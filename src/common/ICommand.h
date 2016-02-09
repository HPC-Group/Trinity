#pragma once
#include "ISerializable.h"
#include "Commands.h"

namespace trinity {
namespace common {

class ICommand : public ISerializable {
    
public:
    ICommand(int sid, int rid) : m_sid(sid), m_rid(rid) {}
    virtual ~ICommand(){}
    
    virtual VclType getType() const = 0;
    
    virtual void serialize(ISerialObject& serial) const = 0;
    virtual void deserialize(ISerialObject& serial) = 0;
    
    int getSid() const { return m_sid; }
    int getRid() const { return m_rid; }
    
protected:
    int m_sid;
    int m_rid;
    Vcl m_vcl;
};
}
}