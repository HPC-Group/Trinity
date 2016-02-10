#pragma once
#include "ICommand.h"
#include "StreamingParams.h"

namespace trinity {
namespace common {

class InitRendererCmd : public ICommand {
    
public:
    InitRendererCmd(ISerialObject&);
    InitRendererCmd(int sid, int rid, const std::string& protocol,
                         const VclType& renderType, int ioSid, const StreamingParams& p);
    
    virtual ~InitRendererCmd();
    
    virtual VclType getType() const;
    const std::string& getProtocol() const;
    const StreamingParams& getParams() const;
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    
private:
    std::string m_protocol;
    VclType m_renderType;
    int m_ioSid;
    StreamingParams m_streamingParams;
};


class ReplyInitRendererCmd : public ICommand {
    
public:
    ReplyInitRendererCmd(ISerialObject&);
    ReplyInitRendererCmd(int sid, int rid);
    virtual ~ReplyInitRendererCmd();
    
    virtual VclType getType() const override;
    int getControlPort() const;
    int getVisPort() const;
    void setControlPort(int port);
    void setVisPort(int port);
    void setNewSid(int sid);
    
    virtual void serialize(ISerialObject& serial) const override;
    virtual void deserialize(ISerialObject& serial) override;
    
private:
    int m_controlPort;
    int m_visPort;
};
}
}