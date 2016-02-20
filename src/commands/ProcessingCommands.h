#pragma once

#include "ICommand.h"

#include "mocca/net/Endpoint.h"

namespace trinity {
namespace commands {

class StreamingParams : public ISerializable {
    
public:
    StreamingParams();
    StreamingParams(int resX, int resY);
    
    virtual VclType getType() const;
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    
    int getResX() const;
    int getResY() const;
    
private:
    int  m_resX, m_resY;
};
    
    
class InitProcessingSessionCmd : public ICommand {
    
public:
    InitProcessingSessionCmd(ISerialObject&);
    InitProcessingSessionCmd(int sid, int rid, const std::string& protocol,
                    const VclType& renderType, int fileId,
                    const std::string& stringifiedIoEndpoint, const StreamingParams& p);
    
    virtual VclType getType() const;
    const std::string& getProtocol() const;
    const std::string& getStringifiedEndpoint() const;
    int getFileId() const;
    VclType getRenderType() const;
    const StreamingParams& getParams() const;
    
    virtual void serialize(ISerialObject& serial) const;
    virtual void deserialize(ISerialObject& serial);
    
private:
    std::string m_protocol;
    VclType m_renderType;
    int m_fileId;
    std::string m_stringifiedEndpoint;
    StreamingParams m_streamingParams;

};


class ReplyInitProcessingSessionCmd : public ICommand {
    
public:
    ReplyInitProcessingSessionCmd(ISerialObject&);
    ReplyInitProcessingSessionCmd(int sid, int rid);
    virtual ~ReplyInitProcessingSessionCmd();
    
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
    float m_isoValue;
};
    
}
}