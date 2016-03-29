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

SetIsoValueCmd::RequestParams::RequestParams(uint8_t surfaceIndex, float value)
    : m_surfaceIndex(surfaceIndex)
    , m_isoValue(value) {}

void SetIsoValueCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("surfaceIndex", m_surfaceIndex);
    writer.appendFloat("isovalue", m_isoValue);
}

void SetIsoValueCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_surfaceIndex = reader.getUInt8("surfaceIndex");
    m_isoValue = reader.getFloat("isovalue");
}

uint8_t SetIsoValueCmd::RequestParams::getSurfaceIndex() const {
    return m_surfaceIndex;
}

float SetIsoValueCmd::RequestParams::getIsoValue() const {
    return m_isoValue;
}

bool SetIsoValueCmd::RequestParams::equals(const RequestParams& other) const {
    return m_surfaceIndex == other.m_surfaceIndex && m_isoValue == other.m_isoValue;
}

std::string SetIsoValueCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "surfaceIndex :" << m_surfaceIndex << "; isoValue: " << m_isoValue;
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

VclType GetActiveModalityCmd::Type = VclType::GetActiveModality;

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

////////////// SetActiveTimestepCmd //////////////

VclType SetActiveTimestepCmd::Type = VclType::SetActiveTimestep;

SetActiveTimestepCmd::RequestParams::RequestParams(uint64_t timestep)
    : m_timestep(timestep) {}

void SetActiveTimestepCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("timestep", m_timestep);
}

void SetActiveTimestepCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_timestep = reader.getUInt64("timestep");
}

bool SetActiveTimestepCmd::RequestParams::equals(const SetActiveTimestepCmd::RequestParams& other) const {
    return m_timestep == other.m_timestep;
}

std::string SetActiveTimestepCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "timestep: " << m_timestep;
    return stream.str();
}

uint64_t SetActiveTimestepCmd::RequestParams::getTimestep() const {
    return m_timestep;
}

////////////// GetActiveTimestepCmd //////////////

VclType GetActiveTimestepCmd::Type = VclType::GetActiveTimestep;

void GetActiveTimestepCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetActiveTimestepCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetActiveTimestepCmd::RequestParams::equals(const GetActiveTimestepCmd::RequestParams& other) const {
    return true;
}

std::string GetActiveTimestepCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetActiveTimestepCmd::ReplyParams::ReplyParams(uint64_t timestep)
    : m_timestep(timestep) {}

void GetActiveTimestepCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("timestep", m_timestep);
}

void GetActiveTimestepCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_timestep = reader.getUInt64("timestep");
}

bool GetActiveTimestepCmd::ReplyParams::equals(const GetActiveTimestepCmd::ReplyParams& other) const {
    return m_timestep == other.m_timestep;
}

std::string GetActiveTimestepCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "timestep: " << m_timestep;
    return stream.str();
}

uint64_t GetActiveTimestepCmd::ReplyParams::getTimestep() const {
    return m_timestep;
}

////////////// GetModalityCountProcCmd //////////////

VclType GetModalityCountProcCmd::Type = VclType::GetModalityCountProc;

void GetModalityCountProcCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetModalityCountProcCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetModalityCountProcCmd::RequestParams::equals(const GetModalityCountProcCmd::RequestParams& other) const {
    return true;
}

std::string GetModalityCountProcCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetModalityCountProcCmd::ReplyParams::ReplyParams(uint64_t result)
    : m_result(result) {}

void GetModalityCountProcCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("result", m_result);
}

void GetModalityCountProcCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getUInt64("result");
}

bool GetModalityCountProcCmd::ReplyParams::equals(const GetModalityCountProcCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

uint64_t GetModalityCountProcCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetModalityCountProcCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// GetTimestepCountProcCmd //////////////

VclType GetTimestepCountProcCmd::Type = VclType::GetTimestepCountProc;

void GetTimestepCountProcCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetTimestepCountProcCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetTimestepCountProcCmd::RequestParams::equals(const GetTimestepCountProcCmd::RequestParams& other) const {
    return true;
}

std::string GetTimestepCountProcCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetTimestepCountProcCmd::ReplyParams::ReplyParams(uint64_t result)
    : m_result(result) {}

void GetTimestepCountProcCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("result", m_result);
}

void GetTimestepCountProcCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getUInt64("result");
}

bool GetTimestepCountProcCmd::ReplyParams::equals(const GetTimestepCountProcCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

uint64_t GetTimestepCountProcCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetTimestepCountProcCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// Set1DTransferFunctionCmd //////////////

VclType Set1DTransferFunctionCmd::Type = VclType::Set1DTransferFunction;

Set1DTransferFunctionCmd::RequestParams::RequestParams(const TransferFunction1D& tf)
    : m_tf(tf) {}

void Set1DTransferFunctionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("tf", m_tf);
}

void Set1DTransferFunctionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_tf = reader.getSerializable<TransferFunction1D>("tf");
}

bool Set1DTransferFunctionCmd::RequestParams::equals(const Set1DTransferFunctionCmd::RequestParams& other) const {
    return m_tf == other.m_tf;
}

TransferFunction1D Set1DTransferFunctionCmd::RequestParams::getTf() const {
    return m_tf;
}

std::string Set1DTransferFunctionCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "tf: " << m_tf;
    return stream.str();
}

////////////// GetDefault1DTransferFunctionProcCmd //////////////

VclType GetDefault1DTransferFunctionProcCmd::Type = VclType::GetDefault1DTransferFunctionProc;

GetDefault1DTransferFunctionProcCmd::RequestParams::RequestParams(uint64_t index)
    : m_index(index) {}

void GetDefault1DTransferFunctionProcCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("index", m_index);
}

void GetDefault1DTransferFunctionProcCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_index = reader.getUInt64("index");
}

bool GetDefault1DTransferFunctionProcCmd::RequestParams::equals(const GetDefault1DTransferFunctionProcCmd::RequestParams& other) const {
    return m_index == other.m_index;
}

uint64_t GetDefault1DTransferFunctionProcCmd::RequestParams::getIndex() const {
    return m_index;
}

std::string GetDefault1DTransferFunctionProcCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "index: " << m_index;
    return stream.str();
}

GetDefault1DTransferFunctionProcCmd::ReplyParams::ReplyParams(const TransferFunction1D& result)
    : m_result(result) {}

void GetDefault1DTransferFunctionProcCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetDefault1DTransferFunctionProcCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<TransferFunction1D>("result");
}

bool GetDefault1DTransferFunctionProcCmd::ReplyParams::equals(const GetDefault1DTransferFunctionProcCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

TransferFunction1D GetDefault1DTransferFunctionProcCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetDefault1DTransferFunctionProcCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// GetDefault1DTransferFunctionCountProcCmd //////////////

VclType GetDefault1DTransferFunctionCountProcCmd::Type = VclType::GetDefault1DTransferFunctionCountProc;

void GetDefault1DTransferFunctionCountProcCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetDefault1DTransferFunctionCountProcCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetDefault1DTransferFunctionCountProcCmd::RequestParams::equals(
    const GetDefault1DTransferFunctionCountProcCmd::RequestParams& other) const {
    return true;
}

std::string GetDefault1DTransferFunctionCountProcCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetDefault1DTransferFunctionCountProcCmd::ReplyParams::ReplyParams(uint64_t result)
    : m_result(result) {}

void GetDefault1DTransferFunctionCountProcCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("result", m_result);
}

void GetDefault1DTransferFunctionCountProcCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getUInt64("result");
}

bool GetDefault1DTransferFunctionCountProcCmd::ReplyParams::equals(
    const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

uint64_t GetDefault1DTransferFunctionCountProcCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetDefault1DTransferFunctionCountProcCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// Get1DHistogramProcCmd //////////////

VclType Get1DHistogramProcCmd::Type = VclType::Get1DHistogramProc;

void Get1DHistogramProcCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void Get1DHistogramProcCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool Get1DHistogramProcCmd::RequestParams::equals(const Get1DHistogramProcCmd::RequestParams& other) const {
    return true;
}

std::string Get1DHistogramProcCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

Get1DHistogramProcCmd::ReplyParams::ReplyParams(const std::vector<uint64_t>& result)
    : m_result(result) {}

void Get1DHistogramProcCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendIntVec("result", m_result);
}

void Get1DHistogramProcCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getUInt64Vec("result");
}

bool Get1DHistogramProcCmd::ReplyParams::equals(const Get1DHistogramProcCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

std::vector<uint64_t> Get1DHistogramProcCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string Get1DHistogramProcCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: ";
    ::operator<<(stream, m_result);
    return stream.str();
}

////////////// GetIsoValueCmd //////////////

VclType GetIsoValueCmd::Type = VclType::GetIsoValue;

GetIsoValueCmd::RequestParams::RequestParams(uint8_t surfaceIndex)
    : m_surfaceIndex(surfaceIndex) {}

void GetIsoValueCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("surfaceIndex", m_surfaceIndex);
}

void GetIsoValueCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_surfaceIndex = reader.getUInt8("surfaceIndex");
}

bool GetIsoValueCmd::RequestParams::equals(const GetIsoValueCmd::RequestParams& other) const {
    return m_surfaceIndex == other.m_surfaceIndex;
}

uint8_t GetIsoValueCmd::RequestParams::getSurfaceIndex() const {
    return m_surfaceIndex;
}

std::string GetIsoValueCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "surfaceIndex: " << m_surfaceIndex;
    return stream.str();
}

GetIsoValueCmd::ReplyParams::ReplyParams(float result)
    : m_result(result) {}

void GetIsoValueCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("result", m_result);
}

void GetIsoValueCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getFloat("result");
}

bool GetIsoValueCmd::ReplyParams::equals(const GetIsoValueCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

float GetIsoValueCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetIsoValueCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// SetIsosurfaceColorCmd //////////////

VclType SetIsosurfaceColorCmd::Type = VclType::SetIsosurfaceColor;

SetIsosurfaceColorCmd::RequestParams::RequestParams(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor)
    : m_surfaceIndex(surfaceIndex)
    , m_vColor(vColor) {}

void SetIsosurfaceColorCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("surfaceIndex", m_surfaceIndex);
    writer.appendObject("vColor", m_vColor);
}

void SetIsosurfaceColorCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_surfaceIndex = reader.getUInt8("surfaceIndex");
    m_vColor = reader.getSerializable<Core::Math::Vec3ui8>("vColor");
}

bool SetIsosurfaceColorCmd::RequestParams::equals(const SetIsosurfaceColorCmd::RequestParams& other) const {
    return m_surfaceIndex == other.m_surfaceIndex && m_vColor == other.m_vColor;
}

uint8_t SetIsosurfaceColorCmd::RequestParams::getSurfaceIndex() const {
    return m_surfaceIndex;
}

Core::Math::Vec3ui8 SetIsosurfaceColorCmd::RequestParams::getVColor() const {
    return m_vColor;
}

std::string SetIsosurfaceColorCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "surfaceIndex: " << m_surfaceIndex << "; vColor: " << m_vColor;
    return stream.str();
}

////////////// GetIsosurfaceColorCmd //////////////

VclType GetIsosurfaceColorCmd::Type = VclType::GetIsosurfaceColor;

GetIsosurfaceColorCmd::RequestParams::RequestParams(uint8_t surfaceIndex)
    : m_surfaceIndex(surfaceIndex) {}

void GetIsosurfaceColorCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("surfaceIndex", m_surfaceIndex);
}

void GetIsosurfaceColorCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_surfaceIndex = reader.getUInt8("surfaceIndex");
}

bool GetIsosurfaceColorCmd::RequestParams::equals(const GetIsosurfaceColorCmd::RequestParams& other) const {
    return m_surfaceIndex == other.m_surfaceIndex;
}

uint8_t GetIsosurfaceColorCmd::RequestParams::getSurfaceIndex() const {
    return m_surfaceIndex;
}

std::string GetIsosurfaceColorCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "surfaceIndex: " << m_surfaceIndex;
    return stream.str();
}

GetIsosurfaceColorCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui8& result)
    : m_result(result) {}

void GetIsosurfaceColorCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetIsosurfaceColorCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<Core::Math::Vec3ui8>("result");
}

bool GetIsosurfaceColorCmd::ReplyParams::equals(const GetIsosurfaceColorCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

Core::Math::Vec3ui8 GetIsosurfaceColorCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetIsosurfaceColorCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// GetRangeProcCmd //////////////

VclType GetRangeProcCmd::Type = VclType::GetRangeProc;

GetRangeProcCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetRangeProcCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetRangeProcCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetRangeProcCmd::RequestParams::equals(const GetRangeProcCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

uint64_t GetRangeProcCmd::RequestParams::getModality() const {
    return m_modality;
}

std::string GetRangeProcCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

GetRangeProcCmd::ReplyParams::ReplyParams(const Core::Math::Vec2f& result)
    : m_result(result) {}

void GetRangeProcCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetRangeProcCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<Core::Math::Vec2f>("result");
}

bool GetRangeProcCmd::ReplyParams::equals(const GetRangeProcCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

Core::Math::Vec2f GetRangeProcCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetRangeProcCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// SetClearViewPositionCmd //////////////

VclType SetClearViewPositionCmd::Type = VclType::SetClearViewPosition;

SetClearViewPositionCmd::RequestParams::RequestParams(const Core::Math::Vec2f& vNormalizedWindowPos)
    : m_vNormalizedWindowPos(vNormalizedWindowPos) {}

void SetClearViewPositionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("vNormalizedWindowPos", m_vNormalizedWindowPos);
}

void SetClearViewPositionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_vNormalizedWindowPos = reader.getSerializable<Core::Math::Vec2f>("vNormalizedWindowPos");
}

bool SetClearViewPositionCmd::RequestParams::equals(const SetClearViewPositionCmd::RequestParams& other) const {
    return m_vNormalizedWindowPos == other.m_vNormalizedWindowPos;
}

Core::Math::Vec2f SetClearViewPositionCmd::RequestParams::getVNormalizedWindowPos() const {
    return m_vNormalizedWindowPos;
}

std::string SetClearViewPositionCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "vNormalizedWindowPos: " << m_vNormalizedWindowPos;
    return stream.str();
}

////////////// SetClearViewRadiusCmd //////////////

VclType SetClearViewRadiusCmd::Type = VclType::SetClearViewRadius;

SetClearViewRadiusCmd::RequestParams::RequestParams(float radius)
    : m_radius(radius) {}

void SetClearViewRadiusCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("radius", m_radius);
}

void SetClearViewRadiusCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_radius = reader.getFloat("radius");
}

bool SetClearViewRadiusCmd::RequestParams::equals(const SetClearViewRadiusCmd::RequestParams& other) const {
    return m_radius == other.m_radius;
}

float SetClearViewRadiusCmd::RequestParams::getRadius() const {
    return m_radius;
}

std::string SetClearViewRadiusCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "radius: " << m_radius;
    return stream.str();
}

////////////// SetClearBorderSizeCmd //////////////

VclType SetClearBorderSizeCmd::Type = VclType::SetClearBorderSize;

SetClearBorderSizeCmd::RequestParams::RequestParams(float borderSize)
    : m_borderSize(borderSize) {}

void SetClearBorderSizeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("borderSize", m_borderSize);
}

void SetClearBorderSizeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_borderSize = reader.getFloat("borderSize");
}

bool SetClearBorderSizeCmd::RequestParams::equals(const SetClearBorderSizeCmd::RequestParams& other) const {
    return m_borderSize == other.m_borderSize;
}

float SetClearBorderSizeCmd::RequestParams::getBorderSize() const {
    return m_borderSize;
}

std::string SetClearBorderSizeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "borderSize: " << m_borderSize;
    return stream.str();
}

////////////// EnableLightingCmd //////////////

VclType EnableLightingCmd::Type = VclType::EnableLighting;

EnableLightingCmd::RequestParams::RequestParams(bool enable)
    : m_enable(enable) {}

void EnableLightingCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("enable", m_enable);
}

void EnableLightingCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_enable = reader.getBool("enable");
}

bool EnableLightingCmd::RequestParams::equals(const EnableLightingCmd::RequestParams& other) const {
    return m_enable == other.m_enable;
}

bool EnableLightingCmd::RequestParams::getEnable() const {
    return m_enable;
}

std::string EnableLightingCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "enable: " << m_enable;
    return stream.str();
}

////////////// SetLightingColorsCmd //////////////

VclType SetLightingColorsCmd::Type = VclType::SetLightingColors;

SetLightingColorsCmd::RequestParams::RequestParams(const IRenderer::PhongColorTriple& colors)
    : m_colors(colors) {}

void SetLightingColorsCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("colors", m_colors);
}

void SetLightingColorsCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_colors = reader.getSerializable<IRenderer::PhongColorTriple>("colors");
}

bool SetLightingColorsCmd::RequestParams::equals(const SetLightingColorsCmd::RequestParams& other) const {
    return m_colors == other.m_colors;
}

IRenderer::PhongColorTriple SetLightingColorsCmd::RequestParams::getColors() const {
    return m_colors;
}

std::string SetLightingColorsCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "colors: " << m_colors;
    return stream.str();
}

////////////// GetLightingColorsCmd //////////////

VclType GetLightingColorsCmd::Type = VclType::GetLightingColors;

void GetLightingColorsCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetLightingColorsCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetLightingColorsCmd::RequestParams::equals(const GetLightingColorsCmd::RequestParams& other) const {
    return true;
}

std::string GetLightingColorsCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetLightingColorsCmd::ReplyParams::ReplyParams(const IRenderer::PhongColorTriple& result)
    : m_result(result) {}

void GetLightingColorsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetLightingColorsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<IRenderer::PhongColorTriple>("result");
}

bool GetLightingColorsCmd::ReplyParams::equals(const GetLightingColorsCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

IRenderer::PhongColorTriple GetLightingColorsCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetLightingColorsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// SetLightDirectionCmd //////////////

VclType SetLightDirectionCmd::Type = VclType::SetLightDirection;

SetLightDirectionCmd::RequestParams::RequestParams(const Core::Math::Vec3f& direction)
    : m_direction(direction) {}

void SetLightDirectionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("direction", m_direction);
}

void SetLightDirectionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_direction = reader.getSerializable<Core::Math::Vec3f>("direction");
}

bool SetLightDirectionCmd::RequestParams::equals(const SetLightDirectionCmd::RequestParams& other) const {
    return m_direction == other.m_direction;
}

Core::Math::Vec3f SetLightDirectionCmd::RequestParams::getDirection() const {
    return m_direction;
}

std::string SetLightDirectionCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "direction: " << m_direction;
    return stream.str();
}

////////////// GetLightDirectionCmd //////////////

VclType GetLightDirectionCmd::Type = VclType::GetLightDirection;

void GetLightDirectionCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetLightDirectionCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetLightDirectionCmd::RequestParams::equals(const GetLightDirectionCmd::RequestParams& other) const {
    return true;
}

std::string GetLightDirectionCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetLightDirectionCmd::ReplyParams::ReplyParams(const Core::Math::Vec3f& result)
    : m_result(result) {}

void GetLightDirectionCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetLightDirectionCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<Core::Math::Vec3f>("result");
}

bool GetLightDirectionCmd::ReplyParams::equals(const GetLightDirectionCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

Core::Math::Vec3f GetLightDirectionCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetLightDirectionCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// SetSampleRateModifierCmd //////////////

VclType SetSampleRateModifierCmd::Type = VclType::SetSampleRateModifier;

SetSampleRateModifierCmd::RequestParams::RequestParams(float fSampleRateModifier)
    : m_fSampleRateModifier(fSampleRateModifier) {}

void SetSampleRateModifierCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("fSampleRateModifier", m_fSampleRateModifier);
}

void SetSampleRateModifierCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_fSampleRateModifier = reader.getFloat("fSampleRateModifier");
}

bool SetSampleRateModifierCmd::RequestParams::equals(const SetSampleRateModifierCmd::RequestParams& other) const {
    return m_fSampleRateModifier == other.m_fSampleRateModifier;
}

float SetSampleRateModifierCmd::RequestParams::getFSampleRateModifier() const {
    return m_fSampleRateModifier;
}

std::string SetSampleRateModifierCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "fSampleRateModifier: " << m_fSampleRateModifier;
    return stream.str();
}

////////////// GetSampleRateModifierCmd //////////////

VclType GetSampleRateModifierCmd::Type = VclType::GetSampleRateModifier;

void GetSampleRateModifierCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetSampleRateModifierCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetSampleRateModifierCmd::RequestParams::equals(const GetSampleRateModifierCmd::RequestParams& other) const {
    return true;
}

std::string GetSampleRateModifierCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetSampleRateModifierCmd::ReplyParams::ReplyParams(float result)
    : m_result(result) {}

void GetSampleRateModifierCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("result", m_result);
}

void GetSampleRateModifierCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getFloat("result");
}

bool GetSampleRateModifierCmd::ReplyParams::equals(const GetSampleRateModifierCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

float GetSampleRateModifierCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetSampleRateModifierCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// SetBoundingBoxModeCmd //////////////

VclType SetBoundingBoxModeCmd::Type = VclType::SetBoundingBoxMode;

SetBoundingBoxModeCmd::RequestParams::RequestParams(IRenderer::BBoxMode mode)
    : m_mode(mode) {}

void SetBoundingBoxModeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("mode", IRenderer::bboxModeMapper().getByFirst(m_mode));
}

void SetBoundingBoxModeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_mode = IRenderer::bboxModeMapper().getBySecond(reader.getString("mode"));
}

bool SetBoundingBoxModeCmd::RequestParams::equals(const SetBoundingBoxModeCmd::RequestParams& other) const {
    return m_mode == other.m_mode;
}

IRenderer::BBoxMode SetBoundingBoxModeCmd::RequestParams::getMode() const {
    return m_mode;
}

std::string SetBoundingBoxModeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "mode: " << IRenderer::bboxModeMapper().getByFirst(m_mode);
    return stream.str();
}

////////////// GetBoundingBoxModeCmd //////////////

VclType GetBoundingBoxModeCmd::Type = VclType::GetBoundingBoxMode;

void GetBoundingBoxModeCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetBoundingBoxModeCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetBoundingBoxModeCmd::RequestParams::equals(const GetBoundingBoxModeCmd::RequestParams& other) const {
    return true;
}

std::string GetBoundingBoxModeCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetBoundingBoxModeCmd::ReplyParams::ReplyParams(const IRenderer::BBoxMode& result)
    : m_result(result) {}

void GetBoundingBoxModeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendString("result", IRenderer::bboxModeMapper().getByFirst(m_result));
}

void GetBoundingBoxModeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = IRenderer::bboxModeMapper().getBySecond(reader.getString("result"));
}

bool GetBoundingBoxModeCmd::ReplyParams::equals(const GetBoundingBoxModeCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

IRenderer::BBoxMode GetBoundingBoxModeCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetBoundingBoxModeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << IRenderer::bboxModeMapper().getByFirst(m_result);
    return stream.str();
}

////////////// SetRendererSpecialsCmd //////////////

VclType SetRendererSpecialsCmd::Type = VclType::SetRendererSpecials;

SetRendererSpecialsCmd::RequestParams::RequestParams(const std::vector<uint64_t>& params)
    : m_params(params) {}

void SetRendererSpecialsCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendIntVec("params", m_params);
}

void SetRendererSpecialsCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_params = reader.getUInt64Vec("params");
}

bool SetRendererSpecialsCmd::RequestParams::equals(const SetRendererSpecialsCmd::RequestParams& other) const {
    return m_params == other.m_params;
}

std::vector<uint64_t> SetRendererSpecialsCmd::RequestParams::getParams() const {
    return m_params;
}

std::string SetRendererSpecialsCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "params: ";
    ::operator<<(stream, m_params);
    return stream.str();
}

////////////// GetRendererSpecialsCmd //////////////

VclType GetRendererSpecialsCmd::Type = VclType::GetRendererSpecials;

void GetRendererSpecialsCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetRendererSpecialsCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetRendererSpecialsCmd::RequestParams::equals(const GetRendererSpecialsCmd::RequestParams& other) const {
    return true;
}

std::string GetRendererSpecialsCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetRendererSpecialsCmd::ReplyParams::ReplyParams(const std::vector<uint64_t>& result)
    : m_result(result) {}

void GetRendererSpecialsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendIntVec("result", m_result);
}

void GetRendererSpecialsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getUInt64Vec("result");
}

bool GetRendererSpecialsCmd::ReplyParams::equals(const GetRendererSpecialsCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

std::vector<uint64_t> GetRendererSpecialsCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetRendererSpecialsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: ";
    ::operator<<(stream, m_result);
    return stream.str();
}

////////////// SetBackgroundColorsCmd //////////////

VclType SetBackgroundColorsCmd::Type = VclType::SetBackgroundColors;

SetBackgroundColorsCmd::RequestParams::RequestParams(const IRenderer::BackgroundColors& colors)
    : m_colors(colors) {}

void SetBackgroundColorsCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("colors", m_colors);
}

void SetBackgroundColorsCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_colors = reader.getSerializable<IRenderer::BackgroundColors>("colors");
}

bool SetBackgroundColorsCmd::RequestParams::equals(const SetBackgroundColorsCmd::RequestParams& other) const {
    return m_colors == other.m_colors;
}

IRenderer::BackgroundColors SetBackgroundColorsCmd::RequestParams::getColors() const {
    return m_colors;
}

std::string SetBackgroundColorsCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "colors: " << m_colors;
    return stream.str();
}

////////////// GetBackgroundColorsCmd //////////////

VclType GetBackgroundColorsCmd::Type = VclType::GetBackgroundColors;

void GetBackgroundColorsCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetBackgroundColorsCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetBackgroundColorsCmd::RequestParams::equals(const GetBackgroundColorsCmd::RequestParams& other) const {
    return true;
}

std::string GetBackgroundColorsCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetBackgroundColorsCmd::ReplyParams::ReplyParams(const IRenderer::BackgroundColors& result)
    : m_result(result) {}

void GetBackgroundColorsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetBackgroundColorsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<IRenderer::BackgroundColors>("result");
}

bool GetBackgroundColorsCmd::ReplyParams::equals(const GetBackgroundColorsCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

IRenderer::BackgroundColors GetBackgroundColorsCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetBackgroundColorsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// EnableClippingCmd //////////////

VclType EnableClippingCmd::Type = VclType::EnableClipping;

EnableClippingCmd::RequestParams::RequestParams(bool enable)
    : m_enable(enable) {}

void EnableClippingCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("enable", m_enable);
}

void EnableClippingCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_enable = reader.getBool("enable");
}

bool EnableClippingCmd::RequestParams::equals(const EnableClippingCmd::RequestParams& other) const {
    return m_enable == other.m_enable;
}

bool EnableClippingCmd::RequestParams::getEnable() const {
    return m_enable;
}

std::string EnableClippingCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "enable: " << m_enable;
    return stream.str();
}

////////////// SetClipVolumeCmd //////////////

VclType SetClipVolumeCmd::Type = VclType::SetClipVolume;

SetClipVolumeCmd::RequestParams::RequestParams(const Core::Math::Vec3f& minValues, const Core::Math::Vec3f& maxValues)
    : m_minValues(minValues)
    , m_maxValues(maxValues) {}

void SetClipVolumeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("minValues", m_minValues);
    writer.appendObject("maxValues", m_maxValues);
}

void SetClipVolumeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_minValues = reader.getSerializable<Core::Math::Vec3f>("minValues");
    m_maxValues = reader.getSerializable<Core::Math::Vec3f>("maxValues");
}

bool SetClipVolumeCmd::RequestParams::equals(const SetClipVolumeCmd::RequestParams& other) const {
    return m_minValues == other.m_minValues && m_maxValues == other.m_maxValues;
}

Core::Math::Vec3f SetClipVolumeCmd::RequestParams::getMinValues() const {
    return m_minValues;
}

Core::Math::Vec3f SetClipVolumeCmd::RequestParams::getMaxValues() const {
    return m_maxValues;
}

std::string SetClipVolumeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "minValues: " << m_minValues << "; maxValues: " << m_maxValues;
    return stream.str();
}

////////////// SetViewParametersCmd //////////////

VclType SetViewParametersCmd::Type = VclType::SetViewParameters;

SetViewParametersCmd::RequestParams::RequestParams(float angle, float znear, float zfar)
    : m_angle(angle)
    , m_znear(znear)
    , m_zfar(zfar) {}

void SetViewParametersCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("angle", m_angle);
    writer.appendFloat("znear", m_znear);
    writer.appendFloat("zfar", m_zfar);
}

void SetViewParametersCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_angle = reader.getFloat("angle");
    m_znear = reader.getFloat("znear");
    m_zfar = reader.getFloat("zfar");
}

bool SetViewParametersCmd::RequestParams::equals(const SetViewParametersCmd::RequestParams& other) const {
    return m_angle == other.m_angle && m_znear == other.m_znear && m_zfar == other.m_zfar;
}

float SetViewParametersCmd::RequestParams::getAngle() const {
    return m_angle;
}

float SetViewParametersCmd::RequestParams::getZnear() const {
    return m_znear;
}

float SetViewParametersCmd::RequestParams::getZfar() const {
    return m_zfar;
}

std::string SetViewParametersCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "angle: " << m_angle << "; znear: " << m_znear << "; zfar: " << m_zfar;
    return stream.str();
}

////////////// RotateCameraCmd //////////////

VclType RotateCameraCmd::Type = VclType::RotateCamera;

RotateCameraCmd::RequestParams::RequestParams(const Core::Math::Vec3f& rotation)
    : m_rotation(rotation) {}

void RotateCameraCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("rotation", m_rotation);
}

void RotateCameraCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_rotation = reader.getSerializable<Core::Math::Vec3f>("rotation");
}

bool RotateCameraCmd::RequestParams::equals(const RotateCameraCmd::RequestParams& other) const {
    return m_rotation == other.m_rotation;
}

Core::Math::Vec3f RotateCameraCmd::RequestParams::getRotation() const {
    return m_rotation;
}

std::string RotateCameraCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "rotation: " << m_rotation;
    return stream.str();
}

////////////// MoveCameraCmd //////////////

VclType MoveCameraCmd::Type = VclType::MoveCamera;

MoveCameraCmd::RequestParams::RequestParams(const Core::Math::Vec3f& direction)
    : m_direction(direction) {}

void MoveCameraCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("direction", m_direction);
}

void MoveCameraCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_direction = reader.getSerializable<Core::Math::Vec3f>("direction");
}

bool MoveCameraCmd::RequestParams::equals(const MoveCameraCmd::RequestParams& other) const {
    return m_direction == other.m_direction;
}

Core::Math::Vec3f MoveCameraCmd::RequestParams::getDirection() const {
    return m_direction;
}

std::string MoveCameraCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "direction: " << m_direction;
    return stream.str();
}

////////////// RotateSceneCmd //////////////

VclType RotateSceneCmd::Type = VclType::RotateScene;

RotateSceneCmd::RequestParams::RequestParams(const Core::Math::Vec3f& rotation)
    : m_rotation(rotation) {}

void RotateSceneCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("rotation", m_rotation);
}

void RotateSceneCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_rotation = reader.getSerializable<Core::Math::Vec3f>("rotation");
}

bool RotateSceneCmd::RequestParams::equals(const RotateSceneCmd::RequestParams& other) const {
    return m_rotation == other.m_rotation;
}

Core::Math::Vec3f RotateSceneCmd::RequestParams::getRotation() const {
    return m_rotation;
}

std::string RotateSceneCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "rotation: " << m_rotation;
    return stream.str();
}

////////////// MoveSceneCmd //////////////

VclType MoveSceneCmd::Type = VclType::MoveScene;

MoveSceneCmd::RequestParams::RequestParams(const Core::Math::Vec3f& direction)
    : m_direction(direction) {}

void MoveSceneCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("direction", m_direction);
}

void MoveSceneCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_direction = reader.getSerializable<Core::Math::Vec3f>("direction");
}

bool MoveSceneCmd::RequestParams::equals(const MoveSceneCmd::RequestParams& other) const {
    return m_direction == other.m_direction;
}

Core::Math::Vec3f MoveSceneCmd::RequestParams::getDirection() const {
    return m_direction;
}

std::string MoveSceneCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "direction: " << m_direction;
    return stream.str();
}

////////////// RescaleSceneCmd //////////////

VclType RescaleSceneCmd::Type = VclType::RescaleScene;

RescaleSceneCmd::RequestParams::RequestParams(float scale)
    : m_scale(scale) {}

void RescaleSceneCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendFloat("scale", m_scale);
}

void RescaleSceneCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_scale = reader.getFloat("scale");
}

bool RescaleSceneCmd::RequestParams::equals(const RescaleSceneCmd::RequestParams& other) const {
    return m_scale == other.m_scale;
}

float RescaleSceneCmd::RequestParams::getScale() const {
    return m_scale;
}

std::string RescaleSceneCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "scale: " << m_scale;
    return stream.str();
}

////////////// ResetCameraCmd //////////////

VclType ResetCameraCmd::Type = VclType::ResetCamera;

void ResetCameraCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void ResetCameraCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool ResetCameraCmd::RequestParams::equals(const ResetCameraCmd::RequestParams& other) const {
    return true;
}

std::string ResetCameraCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

////////////// ResetObjectCmd //////////////

VclType ResetObjectCmd::Type = VclType::ResetObject;

void ResetObjectCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void ResetObjectCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool ResetObjectCmd::RequestParams::equals(const ResetObjectCmd::RequestParams& other) const {
    return true;
}

std::string ResetObjectCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

////////////// StartRenderingCmd //////////////

VclType StartRenderingCmd::Type = VclType::StartRendering;

void StartRenderingCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void StartRenderingCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool StartRenderingCmd::RequestParams::equals(const StartRenderingCmd::RequestParams& other) const {
    return true;
}

std::string StartRenderingCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

////////////// StopRenderingCmd //////////////

VclType StopRenderingCmd::Type = VclType::StopRendering;

void StopRenderingCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void StopRenderingCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool StopRenderingCmd::RequestParams::equals(const StopRenderingCmd::RequestParams& other) const {
    return true;
}

std::string StopRenderingCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

////////////// IsIdleCmd //////////////

VclType IsIdleCmd::Type = VclType::IsIdle;

void IsIdleCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void IsIdleCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool IsIdleCmd::RequestParams::equals(const IsIdleCmd::RequestParams& other) const {
    return true;
}

std::string IsIdleCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

IsIdleCmd::ReplyParams::ReplyParams(bool result)
    : m_result(result) {}

void IsIdleCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("result", m_result);
}

void IsIdleCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getBool("result");
}

bool IsIdleCmd::ReplyParams::equals(const IsIdleCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

bool IsIdleCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string IsIdleCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// ProceedRenderingCmd //////////////

VclType ProceedRenderingCmd::Type = VclType::ProceedRendering;

void ProceedRenderingCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void ProceedRenderingCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool ProceedRenderingCmd::RequestParams::equals(const ProceedRenderingCmd::RequestParams& other) const {
    return true;
}

std::string ProceedRenderingCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

ProceedRenderingCmd::ReplyParams::ReplyParams(bool result)
    : m_result(result) {}

void ProceedRenderingCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("result", m_result);
}

void ProceedRenderingCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getBool("result");
}

bool ProceedRenderingCmd::ReplyParams::equals(const ProceedRenderingCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

bool ProceedRenderingCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string ProceedRenderingCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

/* AUTOGEN CommandImpl */

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

bool operator==(const SetActiveTimestepCmd::RequestParams& lhs, const SetActiveTimestepCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetActiveTimestepCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetActiveTimestepCmd::RequestParams& lhs, const GetActiveTimestepCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetActiveTimestepCmd::ReplyParams& lhs, const GetActiveTimestepCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetActiveTimestepCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetActiveTimestepCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetModalityCountProcCmd::RequestParams& lhs, const GetModalityCountProcCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetModalityCountProcCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetModalityCountProcCmd::ReplyParams& lhs, const GetModalityCountProcCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetModalityCountProcCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetTimestepCountProcCmd::RequestParams& lhs, const GetTimestepCountProcCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetTimestepCountProcCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetTimestepCountProcCmd::ReplyParams& lhs, const GetTimestepCountProcCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetTimestepCountProcCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const Set1DTransferFunctionCmd::RequestParams& lhs, const Set1DTransferFunctionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const Set1DTransferFunctionCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDefault1DTransferFunctionProcCmd::RequestParams& lhs,
                const GetDefault1DTransferFunctionProcCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionProcCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetDefault1DTransferFunctionProcCmd::ReplyParams& lhs, const GetDefault1DTransferFunctionProcCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionProcCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDefault1DTransferFunctionCountProcCmd::RequestParams& lhs,
                const GetDefault1DTransferFunctionCountProcCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCountProcCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& lhs,
                const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const Get1DHistogramProcCmd::RequestParams& lhs, const Get1DHistogramProcCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const Get1DHistogramProcCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const Get1DHistogramProcCmd::ReplyParams& lhs, const Get1DHistogramProcCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const Get1DHistogramProcCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetIsoValueCmd::RequestParams& lhs, const GetIsoValueCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetIsoValueCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetIsoValueCmd::ReplyParams& lhs, const GetIsoValueCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetIsoValueCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetIsosurfaceColorCmd::RequestParams& lhs, const SetIsosurfaceColorCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetIsosurfaceColorCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetIsosurfaceColorCmd::RequestParams& lhs, const GetIsosurfaceColorCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetIsosurfaceColorCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetIsosurfaceColorCmd::ReplyParams& lhs, const GetIsosurfaceColorCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetIsosurfaceColorCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetRangeProcCmd::RequestParams& lhs, const GetRangeProcCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRangeProcCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetRangeProcCmd::ReplyParams& lhs, const GetRangeProcCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRangeProcCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetClearViewPositionCmd::RequestParams& lhs, const SetClearViewPositionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetClearViewPositionCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetClearViewRadiusCmd::RequestParams& lhs, const SetClearViewRadiusCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetClearViewRadiusCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetClearBorderSizeCmd::RequestParams& lhs, const SetClearBorderSizeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetClearBorderSizeCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const EnableLightingCmd::RequestParams& lhs, const EnableLightingCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const EnableLightingCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetLightingColorsCmd::RequestParams& lhs, const SetLightingColorsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetLightingColorsCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetLightingColorsCmd::RequestParams& lhs, const GetLightingColorsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetLightingColorsCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetLightingColorsCmd::ReplyParams& lhs, const GetLightingColorsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetLightingColorsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetLightDirectionCmd::RequestParams& lhs, const SetLightDirectionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetLightDirectionCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetLightDirectionCmd::RequestParams& lhs, const GetLightDirectionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetLightDirectionCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetLightDirectionCmd::ReplyParams& lhs, const GetLightDirectionCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetLightDirectionCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetSampleRateModifierCmd::RequestParams& lhs, const SetSampleRateModifierCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetSampleRateModifierCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetSampleRateModifierCmd::RequestParams& lhs, const GetSampleRateModifierCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetSampleRateModifierCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetSampleRateModifierCmd::ReplyParams& lhs, const GetSampleRateModifierCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetSampleRateModifierCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetBoundingBoxModeCmd::RequestParams& lhs, const SetBoundingBoxModeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetBoundingBoxModeCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBoundingBoxModeCmd::RequestParams& lhs, const GetBoundingBoxModeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBoundingBoxModeCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetBoundingBoxModeCmd::ReplyParams& lhs, const GetBoundingBoxModeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBoundingBoxModeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetRendererSpecialsCmd::RequestParams& lhs, const SetRendererSpecialsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetRendererSpecialsCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetRendererSpecialsCmd::RequestParams& lhs, const GetRendererSpecialsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRendererSpecialsCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetRendererSpecialsCmd::ReplyParams& lhs, const GetRendererSpecialsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRendererSpecialsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetBackgroundColorsCmd::RequestParams& lhs, const SetBackgroundColorsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetBackgroundColorsCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBackgroundColorsCmd::RequestParams& lhs, const GetBackgroundColorsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBackgroundColorsCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetBackgroundColorsCmd::ReplyParams& lhs, const GetBackgroundColorsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBackgroundColorsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const EnableClippingCmd::RequestParams& lhs, const EnableClippingCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const EnableClippingCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetClipVolumeCmd::RequestParams& lhs, const SetClipVolumeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetClipVolumeCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const SetViewParametersCmd::RequestParams& lhs, const SetViewParametersCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const SetViewParametersCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const RotateCameraCmd::RequestParams& lhs, const RotateCameraCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const RotateCameraCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const MoveCameraCmd::RequestParams& lhs, const MoveCameraCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const MoveCameraCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const RotateSceneCmd::RequestParams& lhs, const RotateSceneCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const RotateSceneCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const MoveSceneCmd::RequestParams& lhs, const MoveSceneCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const MoveSceneCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const RescaleSceneCmd::RequestParams& lhs, const RescaleSceneCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const RescaleSceneCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const ResetCameraCmd::RequestParams& lhs, const ResetCameraCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ResetCameraCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const ResetObjectCmd::RequestParams& lhs, const ResetObjectCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ResetObjectCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const StartRenderingCmd::RequestParams& lhs, const StartRenderingCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const StartRenderingCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const StopRenderingCmd::RequestParams& lhs, const StopRenderingCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const StopRenderingCmd::RequestParams& obj) {
    return os << obj.toString();
}

bool operator==(const IsIdleCmd::RequestParams& lhs, const IsIdleCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const IsIdleCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const IsIdleCmd::ReplyParams& lhs, const IsIdleCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const IsIdleCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const ProceedRenderingCmd::RequestParams& lhs, const ProceedRenderingCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ProceedRenderingCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const ProceedRenderingCmd::ReplyParams& lhs, const ProceedRenderingCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ProceedRenderingCmd::ReplyParams& obj) {
    return os << obj.toString();
}

/* AUTOGEN CommandImplOperators */
}
