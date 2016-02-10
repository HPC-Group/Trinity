#pragma once
#include "ICommand.h"


namespace trinity {
namespace common {

class ListFilesCmd : public ICommand {
    
public:
    ListFilesCmd(ISerialObject&);
    ListFilesCmd(int sid, int rid);
    virtual ~ListFilesCmd();
    
    virtual VclType getType() const;
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);

};
    
class InitIOSession : public ICommand {
    
public:
    InitIOSession(ISerialObject&);
    InitIOSession(int sid, int rid);
    virtual ~InitIOSession();
    
    virtual VclType getType() const;
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    
};
}
}