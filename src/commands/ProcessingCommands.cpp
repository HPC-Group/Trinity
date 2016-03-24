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
    writer.appendInt("xres", m_resX);
    writer.appendInt("yres", m_resY);
}

void StreamingParams::deserialize(const ISerialReader& reader) {
    m_resX = reader.getInt32("xres");
    m_resY = reader.getInt32("yres");
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

InitProcessingSessionCmd::RequestParams::RequestParams(const std::string& protocol, const VclType& renderType, const std::string& fileId,
                                                       const mocca::net::Endpoint& ioEndpoint, const StreamingParams& p)
    : m_protocol(protocol)
    , m_renderType(renderType)
    , m_fileId(fileId)
    , m_ioEndpoint(ioEndpoint)
    , m_streamingParams(p) {}

void InitProcessingSessionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("protocol", m_protocol);
    writer.appendString("rendertype", Vcl::instance().toString(m_renderType));
    writer.appendString("fileid", m_fileId);
    writer.appendString("ioendpoint", m_ioEndpoint.toString());
    writer.appendObject("streamingparams", m_streamingParams);
}

void InitProcessingSessionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_protocol = reader.getString("protocol");
    m_renderType = Vcl::instance().toType(reader.getString("rendertype"));
    m_fileId = reader.getString("fileid");
    m_ioEndpoint = mocca::net::Endpoint(reader.getString("ioendpoint"));
    m_streamingParams = reader.getSerializable<StreamingParams>("streamingparams");
}

VclType InitProcessingSessionCmd::RequestParams::getRenderType() const {
    return m_renderType;
}

std::string InitProcessingSessionCmd::RequestParams::getProtocol() const {
    return m_protocol;
}

std::string InitProcessingSessionCmd::RequestParams::getFileId() const {
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
    writer.appendString("controlport", m_controlPort);
    writer.appendString("visport", m_visPort);
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
    writer.appendFloat("isovalue", m_isoValue);
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

////////////// InitContextCmd //////////////

VclType InitContextCmd::Type = VclType::InitContext;

InitContextCmd::RequestParams::RequestParams(int32_t value)
    : m_dummy(value) {}

void InitContextCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("dummy", m_dummy);
}

void InitContextCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_dummy = reader.getInt32("dummy");
}

bool InitContextCmd::RequestParams::equals(const RequestParams& other) const {
    return m_dummy == other.m_dummy;
}

std::string InitContextCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "dummy: " << m_dummy;
    return stream.str();
}

#define PYTHON_MAGIC_DEFINITION


VclType ZoomCameraCmd::Type = VclType::ZoomCamera;

float ZoomCameraCmd::RequestParams::getZoom() const {
    return m_zoom;
}

ZoomCameraCmd::RequestParams::RequestParams(float value)
    : m_zoom(value) {}


void ZoomCameraCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("zoom", m_zoom);
}

void ZoomCameraCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_zoom = reader.getFloat("zoom");
}

bool ZoomCameraCmd::RequestParams::equals(const ZoomCameraCmd::RequestParams& other) const {
    return m_zoom == other.m_zoom;
}

std::string ZoomCameraCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

void ZoomCameraCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    // TODO
}

void ZoomCameraCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    // TODO
}

bool ZoomCameraCmd::ReplyParams::equals(const ZoomCameraCmd::ReplyParams& other) const {
    // TODO
    return true;
}

std::string ZoomCameraCmd::ReplyParams::toString() const {
    std::stringstream stream;
    return stream.str();
}


////////////// SetRenderModeCmd //////////////

VclType SetRenderModeCmd::Type = VclType::SetRenderMode;

SetRenderModeCmd::RequestParams::RequestParams(IRenderer::ERenderMode renderMode)
    : m_renderMode(renderMode) {}

void SetRenderModeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("renderMode", IRenderer::renderModeMapper().getByFirst(m_renderMode));
}

void SetRenderModeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_renderMode = IRenderer::renderModeMapper().getBySecond(reader.getString("renderMode"));
}

bool SetRenderModeCmd::RequestParams::equals(const SetRenderModeCmd::RequestParams& other) const {
    return m_renderMode == other.m_renderMode;
}

std::string SetRenderModeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "renderMode: " << IRenderer::renderModeMapper().getByFirst(m_renderMode);
    return stream.str();
}

IRenderer::ERenderMode SetRenderModeCmd::RequestParams::getRenderMode() const {
    return m_renderMode;
}


////////////// SupportsRenderModeCmd //////////////

VclType SupportsRenderModeCmd::Type = VclType::SupportsRenderMode;

SupportsRenderModeCmd::RequestParams::RequestParams(IRenderer::ERenderMode renderMode)
    : m_renderMode(renderMode) {}

void SupportsRenderModeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("renderMode", IRenderer::renderModeMapper().getByFirst(m_renderMode));
}

void SupportsRenderModeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_renderMode = IRenderer::renderModeMapper().getBySecond(reader.getString("renderMode"));
}

bool SupportsRenderModeCmd::RequestParams::equals(const SupportsRenderModeCmd::RequestParams& other) const {
    return m_renderMode == other.m_renderMode;
}

std::string SupportsRenderModeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "renderMode: " << IRenderer::renderModeMapper().getByFirst(m_renderMode);
    return stream.str();
}

IRenderer::ERenderMode SupportsRenderModeCmd::RequestParams::getRenderMode() const {
    return m_renderMode;
}

SupportsRenderModeCmd::ReplyParams::ReplyParams(bool result)
    : m_result(result) {}

void SupportsRenderModeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("result", m_result);
}

void SupportsRenderModeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getBool("result");
}

bool SupportsRenderModeCmd::ReplyParams::equals(const SupportsRenderModeCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

std::string SupportsRenderModeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

bool SupportsRenderModeCmd::ReplyParams::getResult() const {
    return m_result;
}


////////////// SetActiveModalityCmd //////////////

VclType SetActiveModalityCmd::Type = VclType::SetActiveModality;

SetActiveModalityCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void SetActiveModalityCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void SetActiveModalityCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool SetActiveModalityCmd::RequestParams::equals(const SetActiveModalityCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string SetActiveModalityCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t SetActiveModalityCmd::RequestParams::getModality() const {
    return m_modality;
}

////////////// GetActiveModalityCmd //////////////

VclType GetActiveModalityCmd::Type = VclType::SetActiveModality;

void GetActiveModalityCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetActiveModalityCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetActiveModalityCmd::RequestParams::equals(const GetActiveModalityCmd::RequestParams& other) const {
    return true;
}

std::string GetActiveModalityCmd::RequestParams::toString() const {

    std::stringstream stream;
    return stream.str();
}

GetActiveModalityCmd::ReplyParams::ReplyParams(uint64_t modality)
    : m_modality(modality) {}

void GetActiveModalityCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetActiveModalityCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetActiveModalityCmd::ReplyParams::equals(const GetActiveModalityCmd::ReplyParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetActiveModalityCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetActiveModalityCmd::ReplyParams::getModality() const {
    return m_modality;
}

#undef PYTHON_MAGIC_DEFINITION

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

#define PYTHON_MAGIC


bool operator==(const ZoomCameraCmd::RequestParams& lhs, const ZoomCameraCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const ZoomCameraCmd::ReplyParams& lhs, const ZoomCameraCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ZoomCameraCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const ZoomCameraCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetRenderModeCmd::RequestParams& lhs, const SetRenderModeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetRenderModeCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const SupportsRenderModeCmd::RequestParams& lhs, const SupportsRenderModeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const SupportsRenderModeCmd::ReplyParams& lhs, const SupportsRenderModeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SupportsRenderModeCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const SupportsRenderModeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetActiveModalityCmd::RequestParams& lhs, const SetActiveModalityCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetActiveModalityCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetActiveModalityCmd::RequestParams& lhs, const GetActiveModalityCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetActiveModalityCmd::ReplyParams& lhs, const GetActiveModalityCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetActiveModalityCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetActiveModalityCmd::ReplyParams& obj) {
    return os << obj.toString();
}

#undef PYTHON_MAGIC
}
