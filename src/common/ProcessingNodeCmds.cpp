#include "common/ProcessingNodeCmds.h"
#include <iostream>

using namespace trinity::common;


InitRendererCmd::InitRendererCmd(int sid, int rid,
                                const std::string& protocol,
                                const VclType& renderType,
                                int ioSid,
                                const StreamingParams& p) :
ICommand(sid, rid),
m_protocol(protocol),
m_renderType(renderType),
m_ioSid(ioSid),
m_streamingParams(p) {}

InitRendererCmd::InitRendererCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

InitRendererCmd::~InitRendererCmd() {}

VclType InitRendererCmd::getType() const {
    return VclType::InitRenderer;
}


void InitRendererCmd::serialize(ISerialObject& serial) const  {

    ICommand::serialize(serial);
    serial.append("protocol", m_protocol);
    serial.append("rendertype", m_vcl.toString(m_renderType));
    serial.append("iosid", m_ioSid);
    serial.append("streamingparams", m_vcl.toString(m_streamingParams.getType()));
    m_streamingParams.serialize(serial);
}


void InitRendererCmd::deserialize(ISerialObject& serial) {
    
    ICommand::deserialize(serial);
    m_protocol = serial.getString("protocol");
    std::string la = serial.getString("rendertype");
    m_renderType = m_vcl.toType(la);
    m_ioSid = serial.getInt("iosid");
    std::string subclassType = serial.getString("streamingparams");
    if(subclassType != m_vcl.toString(m_streamingParams.getType())) {
        throw mocca::Error("subclass type is not streamingparams", __FILE__, __LINE__);
    }
    m_streamingParams.deserialize(serial);
}


const std::string& InitRendererCmd::getProtocol() const {
    return m_protocol;
}

const StreamingParams& InitRendererCmd::getParams() const {
    return m_streamingParams;
}


/////// Reply
ReplyInitRendererCmd::ReplyInitRendererCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}
ReplyInitRendererCmd::ReplyInitRendererCmd(int sid, int rid) : ICommand(sid, rid){}
ReplyInitRendererCmd::~ReplyInitRendererCmd() {}

int ReplyInitRendererCmd::getControlPort() const {
    return m_controlPort;
}

int ReplyInitRendererCmd::getVisPort() const {
    return m_visPort;
}

void ReplyInitRendererCmd::setNewSid(const int sid) {
    m_sid = sid;
}

VclType ReplyInitRendererCmd::getType() const {
    return VclType::TrinityReturn;
}

void ReplyInitRendererCmd::serialize(ISerialObject& serial) const {
    
    ICommand::serialize(serial);
    serial.append("controlport", m_controlPort);
    serial.append("visport", m_visPort);
    
}


void ReplyInitRendererCmd::deserialize(ISerialObject& serial) {

    ICommand::deserialize(serial);
    m_controlPort = serial.getInt("controlport");
    m_visPort = serial.getInt("visport");


}

void ReplyInitRendererCmd::setControlPort(const int port) {
    m_controlPort = port;
}

void ReplyInitRendererCmd::setVisPort(const int port) {
    m_visPort = port;
}