#pragma once
#include "ICommand.h"


namespace trinity {
namespace commands {

class ListFilesCmd : public ICommand {
    
public:
    ListFilesCmd(ISerialObject&);
    ListFilesCmd(int sid, int rid);
    virtual ~ListFilesCmd();
    virtual VclType getType() const;
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
};
    
class InitIOSessionCmd : public ICommand {
    
public:
    InitIOSessionCmd(ISerialObject&);
    InitIOSessionCmd(int sid, int rid, const std::string& protocol, int fileId);
    virtual ~InitIOSessionCmd();
    virtual VclType getType() const;
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    const std::string& getProtocol() const;
    int getFileId() const;
    
private:
    std::string m_protocol;
    int m_fileId;
};
    
class ReplyInitIOSessionCmd : public ICommand {
    
public:
    ReplyInitIOSessionCmd(ISerialObject&);
    ReplyInitIOSessionCmd(int sid, int rid);
    virtual ~ReplyInitIOSessionCmd();
    virtual VclType getType() const;
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    void setControlPort(const int port);
    int getControlPort() const;
    int getFileId() const;
    void setNewSid(int sid);
    
private:
    int m_controlPort;
};
    
    
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