#include "InitRendererCmd.h"

using namespace trinity::common;

InitRendererCmd::InitRendererCmd() : ICommand(0, 0){}

InitRendererCmd::InitRendererCmd(int sid, int rid,
                                const std::string& protocol,
                                const VclType& renderType,
                                const StreamingParams& p) :
ICommand(sid, rid), m_protocol(protocol), m_renderType(renderType), m_streamingParams(p) {}


InitRendererCmd::~InitRendererCmd() {}

VclType InitRendererCmd::getType() const {
    return VclType::InitRenderer;
}


void InitRendererCmd::serialize(std::ostream& stream) {
    stream << m_vcl.toString(getType()) << " " << m_sid << " "
    << m_rid << " " << m_protocol << " " << m_vcl.toString(m_renderType) << " ";
    m_streamingParams.serialize(stream);
}


void InitRendererCmd::deserialize(std::istream& stream) {
    std::string typeString;
    stream >> m_sid >> m_rid >> m_protocol >> typeString;
    m_renderType = m_vcl.toType(typeString);
    m_streamingParams.deserialize(stream);
}


const std::string& InitRendererCmd::getProtocol() const {
    return m_protocol;
}

const StreamingParams& InitRendererCmd::getParams() const {
    return m_streamingParams;
}


/////// Reply

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

void ReplyInitRendererCmd::serialize(std::ostream& stream) {
    stream << m_vcl.toString(getType()) << " " << m_sid << " " << m_rid << " "
    << m_controlPort << " " << m_visPort;
}


void ReplyInitRendererCmd::deserialize(std::istream& stream) {
    stream >> m_sid >> m_rid >> m_controlPort >> m_visPort;
}

void ReplyInitRendererCmd::setControlPort(const int port) {
    m_controlPort = port;
}

void ReplyInitRendererCmd::setVisPort(const int port) {
    m_visPort = port;
}