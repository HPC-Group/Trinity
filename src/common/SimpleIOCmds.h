#pragma once
#include "ICommand.h"


namespace trinity {
namespace common {

class GetLODLevelCountCmd : public ICommand {
    
public:
    GetLODLevelCountCmd(ISerialObject&);
    GetLODLevelCountCmd(int sid, int rid);
    virtual ~GetLODLevelCountCmd();
    
    virtual VclType getType() const;
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);

};



class ReplyGetLODLevelCountCmd : public ICommand {
    
public:
    ReplyGetLODLevelCountCmd(ISerialObject&);
    ReplyGetLODLevelCountCmd(int sid, int rid);
    virtual ~ReplyGetLODLevelCountCmd();
    
    virtual VclType getType() const;
    int getLODLevelCount() const;
    void setLODLevelCount(int lod);
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
private:
    int m_lodCount;
};
}
}