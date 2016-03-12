#include "commands/ProcessingCommands.h"

using namespace trinity;

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

std::string StreamingParams::toString() const {
    std::stringstream stream;
    stream << "xres: " << m_resX << "; yres: " << m_resY;
    return stream.str();
}

bool StreamingParams::equals(const StreamingParams& other) const {
    return m_resX == other.m_resX && m_resY == other.m_resY;
}

////////////// InitProcessingSessionCmd //////////////

VclType InitProcessingSessionCmd::Type = VclType::InitRenderer;

InitProcessingSessionCmd::RequestParams::RequestParams(const std::string& protocol, const VclType& renderType, int fileId,
                                                       const mocca::net::Endpoint& ioEndpoint, const StreamingParams& p)
    : m_protocol(protocol)
    , m_renderType(renderType)
    , m_fileId(fileId)
    , m_ioEndpoint(ioEndpoint)
    , m_streamingParams(p) {}

void InitProcessingSessionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.append("protocol", m_protocol);
    writer.append("rendertype", Vcl::instance().toString(m_renderType));
    writer.append("fileid", m_fileId);
    writer.append("ioendpoint", m_ioEndpoint.toString());
    writer.append("streamingparams", m_streamingParams);
}

void InitProcessingSessionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_protocol = reader.getString("protocol");
    m_renderType = Vcl::instance().toType(reader.getString("rendertype"));
    m_fileId = reader.getInt("fileid");
    m_ioEndpoint = mocca::net::Endpoint(reader.getString("ioendpoint"));
    m_streamingParams = reader.getSerializable<StreamingParams>("streamingparams");
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

mocca::net::Endpoint InitProcessingSessionCmd::RequestParams::getIoEndpoint() const {
    return m_ioEndpoint;
}

bool InitProcessingSessionCmd::RequestParams::equals(const RequestParams& other) const {
    return m_protocol == other.m_protocol && m_renderType == other.m_renderType && m_fileId == other.m_fileId &&
           m_ioEndpoint == other.m_ioEndpoint && m_streamingParams == other.m_streamingParams;
}

std::string InitProcessingSessionCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "protocol: " << m_protocol << "; rendertype: " << Vcl::instance().toString(m_renderType) << "; fileid: " << m_fileId
           << "; ioendpoint: " << m_ioEndpoint << "; streamingparams: { " << m_streamingParams << " }";
    return stream.str();
}

InitProcessingSessionCmd::ReplyParams::ReplyParams(const std::string& controlPort, const std::string& visPort)
    : m_controlPort(controlPort)
    , m_visPort(visPort) {}

std::string InitProcessingSessionCmd::ReplyParams::getControlPort() const {
    return m_controlPort;
}

std::string InitProcessingSessionCmd::ReplyParams::getVisPort() const {
    return m_visPort;
}

void InitProcessingSessionCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.append("controlport", m_controlPort);
    writer.append("visport", m_visPort);
}

void InitProcessingSessionCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_controlPort = reader.getString("controlport");
    m_visPort = reader.getString("visport");
}

bool InitProcessingSessionCmd::ReplyParams::equals(const ReplyParams& other) const {
    return m_controlPort == other.m_controlPort && m_visPort == other.m_visPort;
}

std::string InitProcessingSessionCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "controlPort: " << m_controlPort << "; visPort: " << m_visPort;
    return stream.str();
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

std::string SetIsoValueCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "isoValue: " << m_isoValue;
    return stream.str();
}

////////////// SetIsoValueCmd //////////////

VclType InitContextCmd::Type = VclType::InitContext;

InitContextCmd::RequestParams::RequestParams(int32_t value)
    : m_dummy(value) {}

void InitContextCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.append("dummy", m_dummy);
}

void InitContextCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_dummy = reader.getInt("dummy");
}

bool InitContextCmd::RequestParams::equals(const RequestParams& other) const {
    return m_dummy == other.m_dummy;
}

std::string InitContextCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "dummy: " << m_dummy;
    return stream.str();
}

namespace trinity {
bool operator==(const StreamingParams& lhs, const StreamingParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const StreamingParams& obj) {
    return os << obj.toString();
}

bool operator==(const InitProcessingSessionCmd::RequestParams& lhs, const InitProcessingSessionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const InitProcessingSessionCmd::ReplyParams& lhs, const InitProcessingSessionCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const InitProcessingSessionCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const InitProcessingSessionCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetIsoValueCmd::RequestParams& lhs, const SetIsoValueCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetIsoValueCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const InitContextCmd::RequestParams& lhs, const InitContextCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const InitContextCmd::RequestParams& obj) {
    return os << obj.toString();
}
}
