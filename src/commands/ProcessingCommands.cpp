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

/* AUTOGEN CommandImplOperators */
}
