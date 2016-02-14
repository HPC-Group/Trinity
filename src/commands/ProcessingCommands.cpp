#include "ProcessingCommands.h"

using namespace trinity::commands;


StreamingParams::StreamingParams(int resX, int resY) : m_resX(resX), m_resY(resY) {}

StreamingParams::StreamingParams() : m_resX(1024), m_resY(768) {}

void StreamingParams::serialize(ISerialObject& serial) const {
    
    serial.append("xres", m_resX);
    serial.append("yres", m_resY);
}


void StreamingParams::deserialize(ISerialObject& serial) {
    
    m_resX = serial.getInt("xres");
    m_resY = serial.getInt("yres");
}

int StreamingParams::getResX() const { return m_resX; }
int StreamingParams::getResY() const { return m_resY; }

VclType StreamingParams::getType() const {
    return VclType::StreamingParams;
}


InitProcessingSessionCmd::InitProcessingSessionCmd(int sid, int rid,
                                const std::string& protocol,
                                const VclType& renderType,
                                int fileId,
                                const std::string& endpoint,
                                const StreamingParams& p) :
ICommand(sid, rid),
m_protocol(protocol),
m_renderType(renderType),
m_fileId(fileId),
m_stringifiedEndpoint(endpoint),
m_streamingParams(p) {}

InitProcessingSessionCmd::InitProcessingSessionCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

InitProcessingSessionCmd::~InitProcessingSessionCmd() {}

VclType InitProcessingSessionCmd::getType() const {
    return VclType::InitRenderer;
}


void InitProcessingSessionCmd::serialize(ISerialObject& serial) const  {

    ICommand::serialize(serial);
    serial.append("protocol", m_protocol);
    serial.append("rendertype", m_vcl.toString(m_renderType));
    serial.append("fileid", m_fileId);
    serial.append("endpoint", m_stringifiedEndpoint);
    //serial.append("streamingparams", m_vcl.toString(m_streamingParams.getType()));
    //serial.append("streamingparams", m_streamingParams);
    //ISerializable* obj = static_cast<ISerializable*>(&m_streamingParams);
    serial.append("streamingparams", &m_streamingParams);
    
    m_streamingParams.serialize(serial);
}


void InitProcessingSessionCmd::deserialize(ISerialObject& serial) {
    
    ICommand::deserialize(serial);
    m_protocol = serial.getString("protocol");
    std::string la = serial.getString("rendertype");
    m_renderType = m_vcl.toType(la);
    m_fileId = serial.getInt("fileid");
    m_stringifiedEndpoint  = serial.getString("endpoint");
    std::string subclassType = serial.getString("streamingparams");
    if(subclassType != m_vcl.toString(m_streamingParams.getType())) {
        throw mocca::Error("subclass type is not streamingparams", __FILE__, __LINE__);
    }
    m_streamingParams.deserialize(serial);
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
ReplyInitProcessingSessionCmd::ReplyInitProcessingSessionCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}
ReplyInitProcessingSessionCmd::ReplyInitProcessingSessionCmd(int sid, int rid) : ICommand(sid, rid){}
ReplyInitProcessingSessionCmd::~ReplyInitProcessingSessionCmd() {}

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


void ReplyInitProcessingSessionCmd::deserialize(ISerialObject& serial) {

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