#pragma once
#include "ICommand.h"


namespace trinity {
namespace common {

class SetIsoValueCmd : public ICommand {
    
public:
    SetIsoValueCmd(ISerialObject&);
    SetIsoValueCmd(int sid, int rid, float value);
    virtual ~SetIsoValueCmd();
    
    virtual VclType getType() const;
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    
    float getIsoValue() const;
    
private:
    int m_isoValue;
};
    
    
}
}