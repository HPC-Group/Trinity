#include "commands/ProcessingCommands.h"

using namespace trinity::commands;

////////////// StreamingParams //////////////

StreamingParams::StreamingParams(int resX, int resY)
    : m_resX(resX)
    , m_resY(resY) {}

StreamingParams::StreamingParams()
    : m_resX(1024)
    , m_resY(768) {}

void StreamingParams::serialize(ISerialWriter& writer) const {
    writer.append("xres", m_resX);
    writer.append("yres", m_resY);
}

void StreamingParams::deserialize(const ISerialReader& reader) {
    m_resX = reader.getInt("xres");
    m_resY = reader.getInt("yres");
}

int StreamingParams::getResX() const {
    return m_resX;
}
int StreamingParams::getResY() const {
    return m_resY;
}

bool StreamingParams::equals(const StreamingParams& other) const {
    return m_resX == other.m_resX && m_resY == other.m_resY;
}

////////////// InitProcessingSessionCmd //////////////

VclType InitProcessingSessionCmd::Type = VclType::InitRenderer;

InitProcessingSessionCmd::RequestParams::RequestParams(const std::string& protocol, const VclType& renderType, int fileId,
                                                       const std::string& endpoint, const StreamingParams& p)
    : m_protocol(protocol)
    , m_renderType(renderType)
    , m_fileId(fileId)
    , m_stringifiedEndpoint(endpoint)
    , m_streamingParams(p) {}

void InitProcessingSessionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.append("protocol", m_protocol);
    writer.append("rendertype", Vcl::instance().toString(m_renderType));
    writer.append("fileid", m_fileId);
    writer.append("endpoint", m_stringifiedEndpoint);
    writer.append("streamingparams", m_streamingParams);
}

void InitProcessingSessionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_protocol = reader.getString("protocol");
    m_renderType = Vcl::instance().toType(reader.getString("rendertype"));
    m_fileId = reader.getInt("fileid");
    m_stringifiedEndpoint = reader.getString("endpoint");
    reader.getSerializable("streamingparams", m_streamingParams);
}

VclType InitProcessingSessionCmd::RequestParams::getRenderType() const {
    return m_renderType;
}

std::string InitProcessingSessionCmd::RequestParams::getProtocol() const {
    return m_protocol;
}

int InitProcessingSessionCmd::RequestParams::getFileId() const {
    return m_fileId;
}

StreamingParams InitProcessingSessionCmd::RequestParams::getStreamingParams() const {
    return m_streamingParams;
}

std::string InitProcessingSessionCmd::RequestParams::getStringifiedEndpoint() const {
    return m_stringifiedEndpoint;
}

bool InitProcessingSessionCmd::RequestParams::equals(const RequestParams& other) const {
    return m_protocol == other.m_protocol && m_renderType == other.m_renderType && m_fileId == other.m_fileId &&
           m_stringifiedEndpoint == other.m_stringifiedEndpoint && m_streamingParams == other.m_streamingParams;
}

InitProcessingSessionCmd::ReplyParams::ReplyParams(int controlPort, int visPort)
    : m_controlPort(controlPort)
    , m_visPort(visPort) {}

int InitProcessingSessionCmd::ReplyParams::getControlPort() const {
    return m_controlPort;
}

int InitProcessingSessionCmd::ReplyParams::getVisPort() const {
    return m_visPort;
}

void InitProcessingSessionCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.append("controlport", m_controlPort);
    writer.append("visport", m_visPort);
}

void InitProcessingSessionCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_controlPort = reader.getInt("controlport");
    m_visPort = reader.getInt("visport");
}

bool InitProcessingSessionCmd::ReplyParams::equals(const ReplyParams& other) const {
    return m_controlPort == other.m_controlPort && m_visPort == other.m_visPort;
}

////////////// SetIsoValueCmd //////////////

VclType SetIsoValueCmd::Type = VclType::SetIsoValue;

SetIsoValueCmd::RequestParams::RequestParams(float value)
    : m_isoValue(value) {}

void SetIsoValueCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.append("isovalue", m_isoValue);
}

void SetIsoValueCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_isoValue = reader.getFloat("isovalue");
}

float SetIsoValueCmd::RequestParams::getIsoValue() const {
    return m_isoValue;
}

bool SetIsoValueCmd::RequestParams::equals(const RequestParams& other) const {
    return m_isoValue == other.m_isoValue;
}


namespace trinity {
namespace commands {
bool operator==(const StreamingParams& lhs, const StreamingParams& rhs) {
    return lhs.equals(rhs);
}

bool operator==(const InitProcessingSessionCmd::RequestParams& lhs, const InitProcessingSessionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const InitProcessingSessionCmd::ReplyParams& lhs, const InitProcessingSessionCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}

bool operator==(const SetIsoValueCmd::RequestParams& lhs, const SetIsoValueCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
}
}