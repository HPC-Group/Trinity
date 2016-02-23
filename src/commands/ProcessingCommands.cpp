#include "ProcessingCommands.h"

using namespace trinity::commands;

StreamingParams::StreamingParams(int resX, int resY)
    : m_resX(resX)
    , m_resY(resY) {}

StreamingParams::StreamingParams()
    : m_resX(1024)
    , m_resY(768) {}

void StreamingParams::serialize(ISerialObject& serial) const {
    serial.append("xres", m_resX);
    serial.append("yres", m_resY);
}

void StreamingParams::deserialize(const ISerialObject& serial) {
    m_resX = serial.getInt("xres");
    m_resY = serial.getInt("yres");
}

int StreamingParams::getResX() const {
    return m_resX;
}
int StreamingParams::getResY() const {
    return m_resY;
}

VclType StreamingParams::getType() const {
    return VclType::StreamingParams;
}

InitProcessingSessionCmd::InitProcessingSessionCmd(int sid, int rid, const std::string& protocol, const VclType& renderType, int fileId,
                                                   const std::string& endpoint, const StreamingParams& p)
    : ICommand(sid, rid)
    , m_protocol(protocol)
    , m_renderType(renderType)
    , m_fileId(fileId)
    , m_stringifiedEndpoint(endpoint)
    , m_streamingParams(p) {}

InitProcessingSessionCmd::InitProcessingSessionCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

VclType InitProcessingSessionCmd::getType() const {
    return VclType::InitRenderer;
}

VclType InitProcessingSessionCmd::getRenderType() const {
    return m_renderType;
}

void InitProcessingSessionCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
    serial.append("protocol", m_protocol);
    serial.append("rendertype", Vcl::instance().toString(m_renderType));
    serial.append("fileid", m_fileId);
    serial.append("endpoint", m_stringifiedEndpoint);
    serial.append("streamingparams", m_streamingParams);
}

void InitProcessingSessionCmd::deserialize(const ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_protocol = serial.getString("protocol");
    m_renderType = Vcl::instance().toType(serial.getString("rendertype"));
    m_fileId = serial.getInt("fileid");
    m_stringifiedEndpoint = serial.getString("endpoint");
    serial.getSerializable("streamingparams", m_streamingParams);
}

const std::string& InitProcessingSessionCmd::getProtocol() const {
    return m_protocol;
}

int InitProcessingSessionCmd::getFileId() const {
    return m_fileId;
}

const StreamingParams& InitProcessingSessionCmd::getParams() const {
    return m_streamingParams;
}

const std::string& InitProcessingSessionCmd::getStringifiedEndpoint() const {
    return m_stringifiedEndpoint;
}

/////// Reply
ReplyInitProcessingSessionCmd::ReplyInitProcessingSessionCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}
ReplyInitProcessingSessionCmd::ReplyInitProcessingSessionCmd(int sid, int rid)
    : ICommand(sid, rid) {}

int ReplyInitProcessingSessionCmd::getControlPort() const {
    return m_controlPort;
}

int ReplyInitProcessingSessionCmd::getVisPort() const {
    return m_visPort;
}

void ReplyInitProcessingSessionCmd::setNewSid(int sid) {
    m_sid = sid;
}

VclType ReplyInitProcessingSessionCmd::getType() const {
    return VclType::TrinityReturn;
}

void ReplyInitProcessingSessionCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
    serial.append("controlport", m_controlPort);
    serial.append("visport", m_visPort);
}

void ReplyInitProcessingSessionCmd::deserialize(const ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_controlPort = serial.getInt("controlport");
    m_visPort = serial.getInt("visport");
}

void ReplyInitProcessingSessionCmd::setControlPort(int port) {
    m_controlPort = port;
}

void ReplyInitProcessingSessionCmd::setVisPort(int port) {
    m_visPort = port;
}

SetIsoValueCmd::SetIsoValueCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

SetIsoValueCmd::SetIsoValueCmd(int sid, int rid, float value)
    : ICommand(sid, rid)
    , m_isoValue(value) {}

VclType SetIsoValueCmd::getType() const {
    return VclType::SetIsoValue;
}

void SetIsoValueCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
    serial.append("isovalue", m_isoValue);
}

void SetIsoValueCmd::deserialize(const ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_isoValue = serial.getFloat("isovalue");
}

float SetIsoValueCmd::getIsoValue() const {
    return m_isoValue;
}
