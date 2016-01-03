#pragma once
#include "ISerializable.h"
#include "Commands.h"

namespace trinity {
namespace common {

class ICommand : public ISerializable {
    
public:
    ICommand(int sid, int rid) : m_sid(sid), m_rid(rid) {}
    ICommand() : m_sid(0), m_rid(0) {}
    virtual ~ICommand(){}
    
    virtual VclType getType() const = 0;
    
    virtual void serialize(std::ostream& stream) = 0;
    virtual void deserialize(std::istream& stream) = 0;
    
protected:
    int m_sid;
    int m_rid;
};
}
}