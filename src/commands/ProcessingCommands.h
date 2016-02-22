#pragma once

#include "ICommand.h"

#include "mocca/net/Endpoint.h"

namespace trinity {
namespace commands {

class StreamingParams : public ISerializable {
public:
    StreamingParams();
    StreamingParams(int resX, int resY);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(ISerialObject& serial) override;
    std::string toString() const override;

    int getResX() const;
    int getResY() const;

private:
    int m_resX, m_resY;
};

class InitProcessingSessionCmd : public ICommand {
public:
    InitProcessingSessionCmd(ISerialObject&);
    InitProcessingSessionCmd(int sid, int rid, const std::string& protocol, const VclType& renderType, int fileId,
                             const std::string& stringifiedIoEndpoint, const StreamingParams& p);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(ISerialObject& serial) override;
    std::string toString() const override;

    const std::string& getProtocol() const;
    const std::string& getStringifiedEndpoint() const;
    int getFileId() const;
    VclType getRenderType() const;
    const StreamingParams& getParams() const;

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

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(ISerialObject& serial) override;
    std::string toString() const override;

    int getControlPort() const;
    int getVisPort() const;
    void setControlPort(int port);
    void setVisPort(int port);
    void setNewSid(int sid);

private:
    int m_controlPort;
    int m_visPort;
};

class SetIsoValueCmd : public ICommand {
public:
    SetIsoValueCmd(ISerialObject&);
    SetIsoValueCmd(int sid, int rid, float value);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(ISerialObject& serial) override;
    std::string toString() const override;

    float getIsoValue() const;

private:
    float m_isoValue;
};
}
}