#include "commands/IOCommands.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/base/StringTools.h"
#include "mocca/log/LogManager.h"

using namespace trinity;

////////////// ListFilesCmd //////////////

VclType ListFilesCmd::Type = VclType::ListFiles;

ListFilesCmd::RequestParams::RequestParams(const std::string& dirID)
    : m_dirID(dirID) {}

bool ListFilesCmd::RequestParams::equals(const ListFilesCmd::RequestParams& other) const {
    return m_dirID == other.m_dirID;
}

std::string ListFilesCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "dirid: " << m_dirID;
    return stream.str();
}

void ListFilesCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("dirid", m_dirID);
}

void ListFilesCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_dirID = reader.getString("dirid");
}

std::string ListFilesCmd::RequestParams::getDirID() const {
    return m_dirID;
}

ListFilesCmd::ReplyParams::ReplyParams(const std::vector<IOData>& ioData)
    : m_ioData(ioData) {}

bool ListFilesCmd::ReplyParams::equals(const ListFilesCmd::ReplyParams& other) const {
    return m_ioData == other.m_ioData;
}

std::string ListFilesCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "iodata: ";
    ::operator<<(stream, m_ioData); // ugly, but necessary because of namespaces
    return stream.str();
}

void ListFilesCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObjectVec("iodata", mocca::transformToBasePtrVec<ISerializable>(begin(m_ioData), end(m_ioData)));
}

void ListFilesCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_ioData = reader.getSerializableVec<IOData>("iodata");
}

std::vector<IOData> ListFilesCmd::ReplyParams::getIOData() const {
    return m_ioData;
}

////////////// InitIOSessionCmd //////////////

VclType InitIOSessionCmd::Type = VclType::InitIOSession;

InitIOSessionCmd::RequestParams::RequestParams(const std::string& protocol, const std::string& fileId)
    : m_protocol(protocol)
    , m_fileId(fileId) {}

void InitIOSessionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("protocol", m_protocol);
    writer.appendString("fileid", m_fileId);
}

void InitIOSessionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_protocol = reader.getString("protocol");
    m_fileId = reader.getString("fileid");
}

std::string InitIOSessionCmd::RequestParams::getProtocol() const {
    return m_protocol;
}

std::string InitIOSessionCmd::RequestParams::getFileId() const {
    return m_fileId;
}

bool InitIOSessionCmd::RequestParams::equals(const InitIOSessionCmd::RequestParams& other) const {
    return m_protocol == other.m_protocol && m_fileId == other.m_fileId;
}

std::string InitIOSessionCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "protocol: " << m_protocol << "; fileId: " << m_fileId;
    return stream.str();
}

InitIOSessionCmd::ReplyParams::ReplyParams(const std::string& controlPort)
    : m_controlPort(controlPort) {}

void InitIOSessionCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendString("controlport", m_controlPort);
}

void InitIOSessionCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_controlPort = reader.getString("controlport");
}

std::string InitIOSessionCmd::ReplyParams::getControlPort() const {
    return m_controlPort;
}

bool InitIOSessionCmd::ReplyParams::equals(const InitIOSessionCmd::ReplyParams& other) const {
    return m_controlPort == other.m_controlPort;
}

std::string InitIOSessionCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "controlPort: " << m_controlPort;
    return stream.str();
}

////////////// GetLODLevelCountCmd //////////////

VclType GetLODLevelCountCmd::Type = VclType::GetLODLevelCount;

GetLODLevelCountCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetLODLevelCountCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetLODLevelCountCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetLODLevelCountCmd::RequestParams::equals(const GetLODLevelCountCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetLODLevelCountCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetLODLevelCountCmd::RequestParams::getModality() const {
    return m_modality;
}

GetLODLevelCountCmd::ReplyParams::ReplyParams(uint64_t lodCount)
    : m_lodCount(lodCount) {}

void GetLODLevelCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("lodcount", m_lodCount);
}

void GetLODLevelCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_lodCount = reader.getInt32("lodcount");
}

uint64_t GetLODLevelCountCmd::ReplyParams::getLODLevelCount() const {
    return m_lodCount;
}

bool GetLODLevelCountCmd::ReplyParams::equals(const GetLODLevelCountCmd::ReplyParams& other) const {
    return m_lodCount == other.m_lodCount;
}

std::string GetLODLevelCountCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "lodCount: " << m_lodCount;
    return stream.str();
}

////////////// GetMaxBrickSizeCmd //////////////

VclType GetMaxBrickSizeCmd::Type = VclType::GetMaxBrickSize;

void GetMaxBrickSizeCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetMaxBrickSizeCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetMaxBrickSizeCmd::RequestParams::equals(const GetMaxBrickSizeCmd::RequestParams& other) const {
    return true;
}

std::string GetMaxBrickSizeCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetMaxBrickSizeCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui64& maxBrickSize)
    : m_maxBrickSize(maxBrickSize) {}

void GetMaxBrickSizeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("maxBrickSize", m_maxBrickSize);
}

void GetMaxBrickSizeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_maxBrickSize = reader.getSerializable<Core::Math::Vec3ui64>("maxBrickSize");
}

bool GetMaxBrickSizeCmd::ReplyParams::equals(const GetMaxBrickSizeCmd::ReplyParams& other) const {
    return m_maxBrickSize == other.m_maxBrickSize;
}

std::string GetMaxBrickSizeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "maxBrickSize: " << m_maxBrickSize;
    return stream.str();
}

Core::Math::Vec3ui64 GetMaxBrickSizeCmd::ReplyParams::getMaxBrickSize() const {
    return m_maxBrickSize;
}

////////////// GetMaxUsedBrickSizesCmd //////////////

VclType GetMaxUsedBrickSizesCmd::Type = VclType::GetMaxUsedBrickSizes;

void GetMaxUsedBrickSizesCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetMaxUsedBrickSizesCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetMaxUsedBrickSizesCmd::RequestParams::equals(const GetMaxUsedBrickSizesCmd::RequestParams& other) const {
    return true;
}

std::string GetMaxUsedBrickSizesCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetMaxUsedBrickSizesCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui64& maxUsedBrickSizes)
    : m_maxUsedBrickSizes(maxUsedBrickSizes) {}

void GetMaxUsedBrickSizesCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("maxUsedBrickSizes", m_maxUsedBrickSizes);
}

void GetMaxUsedBrickSizesCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_maxUsedBrickSizes = reader.getSerializable<Core::Math::Vec3ui64>("maxUsedBrickSizes");
}

bool GetMaxUsedBrickSizesCmd::ReplyParams::equals(const GetMaxUsedBrickSizesCmd::ReplyParams& other) const {
    return m_maxUsedBrickSizes == other.m_maxUsedBrickSizes;
}

std::string GetMaxUsedBrickSizesCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "maxUsedBrickSizes: " << m_maxUsedBrickSizes;
    return stream.str();
}

Core::Math::Vec3ui64 GetMaxUsedBrickSizesCmd::ReplyParams::getMaxUsedBrickSizes() const {
    return m_maxUsedBrickSizes;
}

////////////// GetNumberOfTimestepsCmd //////////////

VclType GetNumberOfTimestepsCmd::Type = VclType::GetNumberOfTimesteps;

void GetNumberOfTimestepsCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetNumberOfTimestepsCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetNumberOfTimestepsCmd::RequestParams::equals(const GetNumberOfTimestepsCmd::RequestParams& other) const {
    return true;
}

std::string GetNumberOfTimestepsCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetNumberOfTimestepsCmd::ReplyParams::ReplyParams(uint64_t numberOfTimesteps)
    : m_numberOfTimesteps(numberOfTimesteps) {}

void GetNumberOfTimestepsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("numberOfTimesteps", m_numberOfTimesteps);
}

void GetNumberOfTimestepsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_numberOfTimesteps = reader.getUInt64("numberOfTimesteps");
}

bool GetNumberOfTimestepsCmd::ReplyParams::equals(const GetNumberOfTimestepsCmd::ReplyParams& other) const {
    return m_numberOfTimesteps == other.m_numberOfTimesteps;
}

std::string GetNumberOfTimestepsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "numberOfTimesteps: " << m_numberOfTimesteps;
    return stream.str();
}

uint64_t GetNumberOfTimestepsCmd::ReplyParams::getNumberOfTimesteps() const {
    return m_numberOfTimesteps;
}

////////////// GetDomainSizeCmd //////////////

VclType GetDomainSizeCmd::Type = VclType::GetDomainSize;

GetDomainSizeCmd::RequestParams::RequestParams(uint64_t lod, uint64_t modality)
    : m_lod(lod)
    , m_modality(modality) {}

void GetDomainSizeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("lod", m_lod);
    writer.appendInt("modality", m_modality);
}

void GetDomainSizeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_lod = reader.getUInt64("lod");
    m_modality = reader.getUInt64("modality");
}

bool GetDomainSizeCmd::RequestParams::equals(const GetDomainSizeCmd::RequestParams& other) const {
    return m_lod == other.m_lod && m_modality == other.m_modality;
}

std::string GetDomainSizeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "lod: " << m_lod << "; modality: " << m_modality;
    return stream.str();
}

uint64_t GetDomainSizeCmd::RequestParams::getLod() const {
    return m_lod;
}

uint64_t GetDomainSizeCmd::RequestParams::getModality() const {
    return m_modality;
}

GetDomainSizeCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui64& domainSize)
    : m_domainSize(domainSize) {}

void GetDomainSizeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("domainSize", m_domainSize);
}

void GetDomainSizeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_domainSize = reader.getSerializable<Core::Math::Vec3ui64>("domainSize");
}

bool GetDomainSizeCmd::ReplyParams::equals(const GetDomainSizeCmd::ReplyParams& other) const {
    return m_domainSize == other.m_domainSize;
}

std::string GetDomainSizeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "domainSize: " << m_domainSize;
    return stream.str();
}

Core::Math::Vec3ui64 GetDomainSizeCmd::ReplyParams::getDomainSize() const {
    return m_domainSize;
}

////////////// GetTransformationCmd //////////////

VclType GetTransformationCmd::Type = VclType::GetTransformation;

GetTransformationCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetTransformationCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetTransformationCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetTransformationCmd::RequestParams::equals(const GetTransformationCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetTransformationCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetTransformationCmd::RequestParams::getModality() const {
    return m_modality;
}

GetTransformationCmd::ReplyParams::ReplyParams(const Core::Math::Mat4d& transformation)
    : m_transformation(transformation) {}

void GetTransformationCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("transformation", m_transformation);
}

void GetTransformationCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_transformation = reader.getSerializable<Core::Math::Mat4d>("transformation");
}

bool GetTransformationCmd::ReplyParams::equals(const GetTransformationCmd::ReplyParams& other) const {
    return m_transformation == other.m_transformation;
}

std::string GetTransformationCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "transformation" << m_transformation;
    return stream.str();
}

Core::Math::Mat4d GetTransformationCmd::ReplyParams::getTransformation() const {
    return m_transformation;
}

////////////// GetBrickOverlapSizeCmd //////////////

VclType GetBrickOverlapSizeCmd::Type = VclType::GetBrickOverlapSize;

void GetBrickOverlapSizeCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetBrickOverlapSizeCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetBrickOverlapSizeCmd::RequestParams::equals(const GetBrickOverlapSizeCmd::RequestParams& other) const {
    return true;
}

std::string GetBrickOverlapSizeCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetBrickOverlapSizeCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui& overlapSize)
    : m_overlapSize(overlapSize) {}

void GetBrickOverlapSizeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("overlapSize", m_overlapSize);
}

void GetBrickOverlapSizeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_overlapSize = reader.getSerializable<Core::Math::Vec3ui>("overlapSize");
}

bool GetBrickOverlapSizeCmd::ReplyParams::equals(const GetBrickOverlapSizeCmd::ReplyParams& other) const {
    return m_overlapSize == other.m_overlapSize;
}

std::string GetBrickOverlapSizeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "overlapSize: " << m_overlapSize;
    return stream.str();
}

Core::Math::Vec3ui GetBrickOverlapSizeCmd::ReplyParams::getOverlapSize() const {
    return m_overlapSize;
}

////////////// GetBrickOverlapSizeCmd //////////////

VclType GetLargestSingleBrickLODCmd::Type = VclType::GetLargestSingleBrickLOD;

GetLargestSingleBrickLODCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetLargestSingleBrickLODCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetLargestSingleBrickLODCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetLargestSingleBrickLODCmd::RequestParams::equals(const GetLargestSingleBrickLODCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetLargestSingleBrickLODCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetLargestSingleBrickLODCmd::RequestParams::getModality() const {
    return m_modality;
}

GetLargestSingleBrickLODCmd::ReplyParams::ReplyParams(uint64_t largestSingleBrickLOD)
    : m_largestSingleBrickLOD(largestSingleBrickLOD) {}

void GetLargestSingleBrickLODCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("largestSingleBrickLOD", m_largestSingleBrickLOD);
}

void GetLargestSingleBrickLODCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_largestSingleBrickLOD = reader.getUInt64("largestSingleBrickLOD");
}

bool GetLargestSingleBrickLODCmd::ReplyParams::equals(const GetLargestSingleBrickLODCmd::ReplyParams& other) const {
    return m_largestSingleBrickLOD == other.m_largestSingleBrickLOD;
}

std::string GetLargestSingleBrickLODCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "largestSingleBrickLOD: " << m_largestSingleBrickLOD;
    return stream.str();
}

uint64_t GetLargestSingleBrickLODCmd::ReplyParams::getLargestSingleBrickLOD() const {
    return m_largestSingleBrickLOD;
}

////////////// GetBrickVoxelCountsCmd //////////////

VclType GetBrickVoxelCountsCmd::Type = VclType::GetBrickVoxelCounts;

GetBrickVoxelCountsCmd::RequestParams::RequestParams(const BrickKey& brickKey)
    : m_brickKey(brickKey) {}

void GetBrickVoxelCountsCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickKey", m_brickKey);
}

void GetBrickVoxelCountsCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_brickKey = reader.getSerializable<BrickKey>("brickKey");
}

bool GetBrickVoxelCountsCmd::RequestParams::equals(const GetBrickVoxelCountsCmd::RequestParams& other) const {
    return m_brickKey == other.m_brickKey;
}

std::string GetBrickVoxelCountsCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "brickKey: " << m_brickKey;
    return stream.str();
}

BrickKey GetBrickVoxelCountsCmd::RequestParams::getBrickKey() const {
    return m_brickKey;
}

GetBrickVoxelCountsCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui& brickVoxelCounts)
    : m_brickVoxelCounts(brickVoxelCounts) {}

void GetBrickVoxelCountsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickVoxelCounts", m_brickVoxelCounts);
}

void GetBrickVoxelCountsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_brickVoxelCounts = reader.getSerializable<Core::Math::Vec3ui>("brickVoxelCounts");
}

bool GetBrickVoxelCountsCmd::ReplyParams::equals(const GetBrickVoxelCountsCmd::ReplyParams& other) const {
    return m_brickVoxelCounts == other.m_brickVoxelCounts;
}

std::string GetBrickVoxelCountsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "brickVoxelCounts: " << m_brickVoxelCounts;
    return stream.str();
}

Core::Math::Vec3ui GetBrickVoxelCountsCmd::ReplyParams::getBrickVoxelCounts() const {
    return m_brickVoxelCounts;
}

////////////// GetBrickExtentsCmd //////////////

VclType GetBrickExtentsCmd::Type = VclType::GetBrickExtents;

GetBrickExtentsCmd::RequestParams::RequestParams(const BrickKey& brickKey)
    : m_brickKey(brickKey) {}

void GetBrickExtentsCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickKey", m_brickKey);
}

void GetBrickExtentsCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_brickKey = reader.getSerializable<BrickKey>("brickKey");
}

bool GetBrickExtentsCmd::RequestParams::equals(const GetBrickExtentsCmd::RequestParams& other) const {
    return m_brickKey == other.m_brickKey;
}

std::string GetBrickExtentsCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "brickKey: " << m_brickKey;
    return stream.str();
}

BrickKey GetBrickExtentsCmd::RequestParams::getBrickKey() const {
    return m_brickKey;
}

GetBrickExtentsCmd::ReplyParams::ReplyParams(const Core::Math::Vec3f& brickExtents)
    : m_brickExtents(brickExtents) {}

void GetBrickExtentsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickExtents", m_brickExtents);
}

void GetBrickExtentsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_brickExtents = reader.getSerializable<Core::Math::Vec3f>("brickExtents");
}

bool GetBrickExtentsCmd::ReplyParams::equals(const GetBrickExtentsCmd::ReplyParams& other) const {
    return m_brickExtents == other.m_brickExtents;
}

std::string GetBrickExtentsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "brickExtents: " << m_brickExtents;
    return stream.str();
}

Core::Math::Vec3f GetBrickExtentsCmd::ReplyParams::getBrickExtents() const {
    return m_brickExtents;
}

////////////// GetBrickOverlapSizeCmd //////////////

VclType GetBrickLayoutCmd::Type = VclType::GetBrickLayout;

GetBrickLayoutCmd::RequestParams::RequestParams(uint64_t lod, uint64_t modality)
    : m_lod(lod)
    , m_modality(modality) {}

void GetBrickLayoutCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("lod", m_lod);
    writer.appendInt("modality", m_modality);
}

void GetBrickLayoutCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_lod = reader.getUInt64("lod");
    m_modality = reader.getUInt64("modality");
}

bool GetBrickLayoutCmd::RequestParams::equals(const GetBrickLayoutCmd::RequestParams& other) const {
    return m_lod == other.m_lod && m_modality == other.m_modality;
}

std::string GetBrickLayoutCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "lod: " << m_lod << "; modality: " << m_modality;
    return stream.str();
}

uint64_t GetBrickLayoutCmd::RequestParams::getLod() const {
    return m_lod;
}

uint64_t GetBrickLayoutCmd::RequestParams::getModality() const {
    return m_modality;
}

GetBrickLayoutCmd::ReplyParams::ReplyParams(const Core::Math::Vec3ui& brickLayout)
    : m_brickLayout(brickLayout) {}

void GetBrickLayoutCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickLayout", m_brickLayout);
}

void GetBrickLayoutCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_brickLayout = reader.getSerializable<Core::Math::Vec3ui>("brickLayout");
}

bool GetBrickLayoutCmd::ReplyParams::equals(const GetBrickLayoutCmd::ReplyParams& other) const {
    return m_brickLayout == other.m_brickLayout;
}

std::string GetBrickLayoutCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "brickLayout: " << m_brickLayout;
    return stream.str();
}

Core::Math::Vec3ui GetBrickLayoutCmd::ReplyParams::getBrickLayout() const {
    return m_brickLayout;
}

////////////// GetBrickOverlapSizeCmd //////////////

VclType GetModalityCountCmd::Type = VclType::GetModalityCount;

void GetModalityCountCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetModalityCountCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetModalityCountCmd::RequestParams::equals(const GetModalityCountCmd::RequestParams& other) const {
    return true;
}

std::string GetModalityCountCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetModalityCountCmd::ReplyParams::ReplyParams(uint64_t modalityCount)
    : m_modalityCount(modalityCount) {}

void GetModalityCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modalityCount", m_modalityCount);
}

void GetModalityCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_modalityCount = reader.getUInt64("modalityCount");
}

bool GetModalityCountCmd::ReplyParams::equals(const GetModalityCountCmd::ReplyParams& other) const {
    return m_modalityCount == other.m_modalityCount;
}

std::string GetModalityCountCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "modalityCount: " << m_modalityCount;
    return stream.str();
}

uint64_t GetModalityCountCmd::ReplyParams::getModalityCount() const {
    return m_modalityCount;
}

////////////// GetComponentCountCmd //////////////

VclType GetComponentCountCmd::Type = VclType::GetComponentCount;

GetComponentCountCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetComponentCountCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetComponentCountCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetComponentCountCmd::RequestParams::equals(const GetComponentCountCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetComponentCountCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetComponentCountCmd::RequestParams::getModality() const {
    return m_modality;
}

GetComponentCountCmd::ReplyParams::ReplyParams(uint64_t componentCount)
    : m_componentCount(componentCount) {}

void GetComponentCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("componentCount", m_componentCount);
}

void GetComponentCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_componentCount = reader.getUInt64("componentCount");
}

bool GetComponentCountCmd::ReplyParams::equals(const GetComponentCountCmd::ReplyParams& other) const {
    return m_componentCount == other.m_componentCount;
}

std::string GetComponentCountCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "componentCount: " << m_componentCount;
    return stream.str();
}

uint64_t GetComponentCountCmd::ReplyParams::getComponentCount() const {
    return m_componentCount;
}

////////////// GetRangeCmd //////////////

VclType GetRangeCmd::Type = VclType::GetRange;

GetRangeCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetRangeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetRangeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetRangeCmd::RequestParams::equals(const GetRangeCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetRangeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetRangeCmd::RequestParams::getModality() const {
    return m_modality;
}

GetRangeCmd::ReplyParams::ReplyParams(const Core::Math::Vec2f& range)
    : m_range(range) {}

void GetRangeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("range", m_range);
}

void GetRangeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_range = reader.getSerializable<Core::Math::Vec2f>("range");
}

bool GetRangeCmd::ReplyParams::equals(const GetRangeCmd::ReplyParams& other) const {
    return m_range == other.m_range;
}

std::string GetRangeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "range: " << m_range;
    return stream.str();
}

Core::Math::Vec2f GetRangeCmd::ReplyParams::getRange() const {
    return m_range;
}

////////////// GetTotalBrickCountCmd //////////////

VclType GetTotalBrickCountCmd::Type = VclType::GetTotalBrickCount;

GetTotalBrickCountCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetTotalBrickCountCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetTotalBrickCountCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetTotalBrickCountCmd::RequestParams::equals(const GetTotalBrickCountCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetTotalBrickCountCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetTotalBrickCountCmd::RequestParams::getModality() const {
    return m_modality;
}

GetTotalBrickCountCmd::ReplyParams::ReplyParams(uint64_t totalBrickCount)
    : m_totalBrickCount(totalBrickCount) {}

void GetTotalBrickCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("totalBrickCount", m_totalBrickCount);
}

void GetTotalBrickCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_totalBrickCount = reader.getUInt64("totalBrickCount");
}

bool GetTotalBrickCountCmd::ReplyParams::equals(const GetTotalBrickCountCmd::ReplyParams& other) const {
    return m_totalBrickCount == other.m_totalBrickCount;
}

std::string GetTotalBrickCountCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "totalBrickCount: " << m_totalBrickCount;
    return stream.str();
}

uint64_t GetTotalBrickCountCmd::ReplyParams::getTotalBrickCount() const {
    return m_totalBrickCount;
}

////////////// GetBrickCmd //////////////

VclType GetBrickCmd::Type = VclType::GetBrick;

GetBrickCmd::RequestParams::RequestParams(const BrickKey& brickKey)
    : m_brickKey(brickKey) {}

void GetBrickCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickKey", m_brickKey);
}

void GetBrickCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_brickKey = reader.getSerializable<BrickKey>("brickKey");
}

bool GetBrickCmd::RequestParams::equals(const GetBrickCmd::RequestParams& other) const {
    return m_brickKey == other.m_brickKey;
}

std::string GetBrickCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "brickKey: " << m_brickKey;
    return stream.str();
}

BrickKey GetBrickCmd::RequestParams::getBrickKey() const {
    return m_brickKey;
}

GetBrickCmd::ReplyParams::ReplyParams(std::shared_ptr<std::vector<uint8_t>> brick, bool success)
    : m_success(success)
    , m_brick(brick) {}

void GetBrickCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("success", m_success);
    writer.appendBinary(m_brick);
}

void GetBrickCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_success = reader.getBool("success");
    m_brick = reader.getBinary()[0];
}

bool GetBrickCmd::ReplyParams::equals(const GetBrickCmd::ReplyParams& other) const {
    return m_success == other.m_success && *m_brick == *other.m_brick;
}

std::string GetBrickCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "success: " << m_success;
    stream << "; brick: ";
    ::operator<<(stream, *m_brick); // ugly, but necessary because of namespaces
    return stream.str();
}

bool GetBrickCmd::ReplyParams::getSuccess() const {
    return m_success;
}

std::shared_ptr<std::vector<uint8_t>> GetBrickCmd::ReplyParams::getBrick() const {
    return m_brick;
}

////////////// GetTypeCmd //////////////

VclType GetTypeCmd::Type = VclType::GetType;

GetTypeCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetTypeCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetTypeCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetTypeCmd::RequestParams::equals(const GetTypeCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetTypeCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetTypeCmd::RequestParams::getModality() const {
    return m_modality;
}

GetTypeCmd::ReplyParams::ReplyParams(IIO::ValueType valueType)
    : m_valueType(valueType) {}

void GetTypeCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendString("valueType", IIO::valueTypeMapper().getByFirst(m_valueType));
}

void GetTypeCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_valueType = IIO::valueTypeMapper().getBySecond(reader.getString("valueType"));
}

bool GetTypeCmd::ReplyParams::equals(const GetTypeCmd::ReplyParams& other) const {
    return m_valueType == other.m_valueType;
}

std::string GetTypeCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "valueType: " << IIO::valueTypeMapper().getByFirst(m_valueType);
    return stream.str();
}

IIO::ValueType trinity::GetTypeCmd::ReplyParams::getValueType() const {
    return m_valueType;
}

////////////// GetSemanticCmd //////////////

VclType GetSemanticCmd::Type = VclType::GetSemantic;

GetSemanticCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetSemanticCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetSemanticCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetSemanticCmd::RequestParams::equals(const GetSemanticCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetSemanticCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetSemanticCmd::RequestParams::getModality() const {
    return m_modality;
}

GetSemanticCmd::ReplyParams::ReplyParams(IIO::Semantic semantic)
    : m_semantic(semantic) {}

void GetSemanticCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendString("semantic", IIO::semanticMapper().getByFirst(m_semantic));
}

void GetSemanticCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_semantic = IIO::semanticMapper().getBySecond(reader.getString("semantic"));
}

bool GetSemanticCmd::ReplyParams::equals(const GetSemanticCmd::ReplyParams& other) const {
    return m_semantic == other.m_semantic;
}

std::string GetSemanticCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "semantic: " << IIO::semanticMapper().getByFirst(m_semantic);
    return stream.str();
}

IIO::Semantic GetSemanticCmd::ReplyParams::getSemantic() const {
    return m_semantic;
}

////////////// GetDefault1DTransferFunctionCountCmd //////////////

VclType GetDefault1DTransferFunctionCountCmd::Type = VclType::GetDefault1DTransferFunctionCount;

void GetDefault1DTransferFunctionCountCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetDefault1DTransferFunctionCountCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetDefault1DTransferFunctionCountCmd::RequestParams::equals(const GetDefault1DTransferFunctionCountCmd::RequestParams& other) const {
    return true;
}

std::string GetDefault1DTransferFunctionCountCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetDefault1DTransferFunctionCountCmd::ReplyParams::ReplyParams(uint64_t count)
    : m_count(count) {}

void GetDefault1DTransferFunctionCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("count", m_count);
}

void GetDefault1DTransferFunctionCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_count = reader.getUInt64("count");
}

bool GetDefault1DTransferFunctionCountCmd::ReplyParams::equals(const GetDefault1DTransferFunctionCountCmd::ReplyParams& other) const {
    return m_count == other.m_count;
}

std::string GetDefault1DTransferFunctionCountCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "count: " << m_count;
    return stream.str();
}

uint64_t GetDefault1DTransferFunctionCountCmd::ReplyParams::getCount() const {
    return m_count;
}

////////////// GetDefault2DTransferFunctionCountCmd //////////////

VclType GetDefault2DTransferFunctionCountCmd::Type = VclType::GetDefault2DTransferFunctionCount;

void GetDefault2DTransferFunctionCountCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetDefault2DTransferFunctionCountCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetDefault2DTransferFunctionCountCmd::RequestParams::equals(const GetDefault2DTransferFunctionCountCmd::RequestParams& other) const {
    return true;
}

std::string GetDefault2DTransferFunctionCountCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetDefault2DTransferFunctionCountCmd::ReplyParams::ReplyParams(uint64_t count)
    : m_count(count) {}

void GetDefault2DTransferFunctionCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("count", m_count);
}

void GetDefault2DTransferFunctionCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_count = reader.getUInt64("count");
}

bool GetDefault2DTransferFunctionCountCmd::ReplyParams::equals(const GetDefault2DTransferFunctionCountCmd::ReplyParams& other) const {
    return m_count == other.m_count;
}

std::string GetDefault2DTransferFunctionCountCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "count: " << m_count;
    return stream.str();
}

uint64_t GetDefault2DTransferFunctionCountCmd::ReplyParams::getCount() const {
    return m_count;
}

////////////// Get1DHistogramCmd //////////////

VclType Get1DHistogramCmd::Type = VclType::Get1DHistogram;

void Get1DHistogramCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void Get1DHistogramCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool Get1DHistogramCmd::RequestParams::equals(const Get1DHistogramCmd::RequestParams& other) const {
    return true;
}

std::string Get1DHistogramCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

Get1DHistogramCmd::ReplyParams::ReplyParams(const std::vector<uint64_t>& histogram)
    : m_histogram(histogram) {}

void Get1DHistogramCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendIntVec("histogram", m_histogram);
}

void Get1DHistogramCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_histogram = reader.getUInt64Vec("histogram");
}

bool Get1DHistogramCmd::ReplyParams::equals(const Get1DHistogramCmd::ReplyParams& other) const {
    return m_histogram == other.m_histogram;
}

std::string Get1DHistogramCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "histogram: ";
    ::operator<<(stream, m_histogram); // ugly, but necessary because of namespaces
    return stream.str();
}

std::vector<uint64_t> Get1DHistogramCmd::ReplyParams::getHistogram() const {
    return m_histogram;
}

////////////// Get2DHistogramCmd //////////////

VclType Get2DHistogramCmd::Type = VclType::Get1DHistogram;

void Get2DHistogramCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void Get2DHistogramCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool Get2DHistogramCmd::RequestParams::equals(const Get2DHistogramCmd::RequestParams& other) const {
    return true;
}

std::string Get2DHistogramCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

Get2DHistogramCmd::ReplyParams::ReplyParams(const std::vector<uint64_t>& histogram)
    : m_histogram(histogram) {}

void Get2DHistogramCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendIntVec("histogram", m_histogram);
}

void Get2DHistogramCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_histogram = reader.getUInt64Vec("histogram");
}

bool Get2DHistogramCmd::ReplyParams::equals(const Get2DHistogramCmd::ReplyParams& other) const {
    return m_histogram == other.m_histogram;
}

std::string Get2DHistogramCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "histogram: ";
    ::operator<<(stream, m_histogram); // ugly, but necessary because of namespaces
    return stream.str();
}

std::vector<uint64_t> Get2DHistogramCmd::ReplyParams::getHistogram() const {
    return m_histogram;
}

////////////// GetUserDefinedSemanticCmd //////////////

VclType GetUserDefinedSemanticCmd::Type = VclType::GetUserDefinedSemantic;

GetUserDefinedSemanticCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetUserDefinedSemanticCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetUserDefinedSemanticCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetUserDefinedSemanticCmd::RequestParams::equals(const GetUserDefinedSemanticCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

std::string GetUserDefinedSemanticCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

uint64_t GetUserDefinedSemanticCmd::RequestParams::getModality() const {
    return m_modality;
}

GetUserDefinedSemanticCmd::ReplyParams::ReplyParams(const std::string& semantic)
    : m_semantic(semantic) {}

void GetUserDefinedSemanticCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendString("semantic", m_semantic);
}

void GetUserDefinedSemanticCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_semantic = reader.getString("semantic");
}

bool GetUserDefinedSemanticCmd::ReplyParams::equals(const GetUserDefinedSemanticCmd::ReplyParams& other) const {
    return m_semantic == other.m_semantic;
}

std::string GetUserDefinedSemanticCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "semantic: " << m_semantic;
    return stream.str();
}

std::string GetUserDefinedSemanticCmd::ReplyParams::getSemantic() const {
    return m_semantic;
}

////////////// GetDefault1DTransferFunctionCmd //////////////

VclType GetDefault1DTransferFunctionCmd::Type = VclType::GetDefault1DTransferFunction;

GetDefault1DTransferFunctionCmd::RequestParams::RequestParams(uint64_t index)
    : m_index(index) {}

void GetDefault1DTransferFunctionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("index", m_index);
}

void GetDefault1DTransferFunctionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_index = reader.getUInt64("index");
}

bool GetDefault1DTransferFunctionCmd::RequestParams::equals(const GetDefault1DTransferFunctionCmd::RequestParams& other) const {
    return m_index == other.m_index;
}

std::string GetDefault1DTransferFunctionCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "index: " << m_index;
    return stream.str();
}

uint64_t GetDefault1DTransferFunctionCmd::RequestParams::getIndex() const {
    return m_index;
}

GetDefault1DTransferFunctionCmd::ReplyParams::ReplyParams(const TransferFunction1D& function)
    : m_function(function) {}

void GetDefault1DTransferFunctionCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("function", m_function);
}

void GetDefault1DTransferFunctionCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_function = reader.getSerializable<TransferFunction1D>("function");
}

bool GetDefault1DTransferFunctionCmd::ReplyParams::equals(const GetDefault1DTransferFunctionCmd::ReplyParams& other) const {
    return m_function == other.m_function;
}

std::string GetDefault1DTransferFunctionCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "function: " << m_function;
    return stream.str();
}

TransferFunction1D GetDefault1DTransferFunctionCmd::ReplyParams::getFunction() const {
    return m_function;
}

////////////// GetDomainScaleCmd //////////////

VclType GetDomainScaleCmd::Type = VclType::GetDomainScale;

GetDomainScaleCmd::RequestParams::RequestParams(uint64_t modality)
    : m_modality(modality) {}

void GetDomainScaleCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
}

void GetDomainScaleCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
}

bool GetDomainScaleCmd::RequestParams::equals(const GetDomainScaleCmd::RequestParams& other) const {
    return m_modality == other.m_modality;
}

uint64_t GetDomainScaleCmd::RequestParams::getModality() const {
    return m_modality;
}

std::string GetDomainScaleCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality;
    return stream.str();
}

GetDomainScaleCmd::ReplyParams::ReplyParams(const Core::Math::Vec3f& result)
    : m_result(result) {}

void GetDomainScaleCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetDomainScaleCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<Core::Math::Vec3f>("result");
}

bool GetDomainScaleCmd::ReplyParams::equals(const GetDomainScaleCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

Core::Math::Vec3f GetDomainScaleCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetDomainScaleCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// GetFloatBrickLayoutCmd //////////////

VclType GetFloatBrickLayoutCmd::Type = VclType::GetFloatBrickLayout;

GetFloatBrickLayoutCmd::RequestParams::RequestParams(uint64_t lod, uint64_t modality)
    : m_lod(lod)
    , m_modality(modality) {}

void GetFloatBrickLayoutCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("lod", m_lod);
    writer.appendInt("modality", m_modality);
}

void GetFloatBrickLayoutCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_lod = reader.getUInt64("lod");
    m_modality = reader.getUInt64("modality");
}

bool GetFloatBrickLayoutCmd::RequestParams::equals(const GetFloatBrickLayoutCmd::RequestParams& other) const {
    return m_lod == other.m_lod && m_modality == other.m_modality;
}

uint64_t GetFloatBrickLayoutCmd::RequestParams::getLod() const {
    return m_lod;
}

uint64_t GetFloatBrickLayoutCmd::RequestParams::getModality() const {
    return m_modality;
}

std::string GetFloatBrickLayoutCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "lod: " << m_lod << "; modality: " << m_modality;
    return stream.str();
}

GetFloatBrickLayoutCmd::ReplyParams::ReplyParams(const Core::Math::Vec3f& result)
    : m_result(result) {}

void GetFloatBrickLayoutCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("result", m_result);
}

void GetFloatBrickLayoutCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getSerializable<Core::Math::Vec3f>("result");
}

bool GetFloatBrickLayoutCmd::ReplyParams::equals(const GetFloatBrickLayoutCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

Core::Math::Vec3f GetFloatBrickLayoutCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetFloatBrickLayoutCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: " << m_result;
    return stream.str();
}

////////////// GetRootsCmd //////////////

VclType GetRootsCmd::Type = VclType::GetRoots;

void GetRootsCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetRootsCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetRootsCmd::RequestParams::equals(const GetRootsCmd::RequestParams& other) const {
    return true;
}

std::string GetRootsCmd::RequestParams::toString() const {
    std::stringstream stream;
    return stream.str();
}

GetRootsCmd::ReplyParams::ReplyParams(const std::vector<std::string>& result)
    : m_result(result) {}

void GetRootsCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendStringVec("result", m_result);
}

void GetRootsCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_result = reader.getStringVec("result");
}

bool GetRootsCmd::ReplyParams::equals(const GetRootsCmd::ReplyParams& other) const {
    return m_result == other.m_result;
}

std::vector<std::string> GetRootsCmd::ReplyParams::getResult() const {
    return m_result;
}

std::string GetRootsCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: ";
    ::operator<<(stream, m_result);
    return stream.str();
}

////////////// GetBrickMetaDataCmd //////////////

VclType GetBrickMetaDataCmd::Type = VclType::GetBrickMetaData;

GetBrickMetaDataCmd::RequestParams::RequestParams(uint64_t modality, uint64_t timestep)
    : m_modality(modality)
    , m_timestep(timestep) {}

void GetBrickMetaDataCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("modality", m_modality);
    writer.appendInt("timestep", m_timestep);
}

void GetBrickMetaDataCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_modality = reader.getUInt64("modality");
    m_timestep = reader.getUInt64("timestep");
}

bool GetBrickMetaDataCmd::RequestParams::equals(const GetBrickMetaDataCmd::RequestParams& other) const {
    return m_modality == other.m_modality && m_timestep == other.m_timestep;
}

uint64_t GetBrickMetaDataCmd::RequestParams::getModality() const {
    return m_modality;
}

uint64_t GetBrickMetaDataCmd::RequestParams::getTimestep() const {
    return m_timestep;
}

std::string GetBrickMetaDataCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "modality: " << m_modality << "; timestep: " << m_timestep;
    return stream.str();
}

GetBrickMetaDataCmd::ReplyParams::ReplyParams(std::vector<BrickMetaData> result)
    : m_result(std::move(result)) {}

void GetBrickMetaDataCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    auto binary = BrickMetaData::createBinary(m_result);
    for (const auto& b : binary) {
        writer.appendBinary(b);
    }
}

void GetBrickMetaDataCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    auto binary = reader.getBinary();
    m_result = BrickMetaData::createFromBinary(binary);
}

std::vector<BrickMetaData> GetBrickMetaDataCmd::ReplyParams::releaseResult() {
    return std::move(m_result);
}

std::string GetBrickMetaDataCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "result: (binary data)";
    return stream.str();
}

/* AUTOGEN CommandImpl */

namespace trinity {
bool operator==(const IOData& lhs, const IOData& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const IOData& obj) {
    return os << obj.toString();
}

bool operator==(const ListFilesCmd::RequestParams& lhs, const ListFilesCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ListFilesCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const ListFilesCmd::ReplyParams& lhs, const ListFilesCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ListFilesCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const InitIOSessionCmd::RequestParams& lhs, const InitIOSessionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const InitIOSessionCmd::ReplyParams& lhs, const InitIOSessionCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const InitIOSessionCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const InitIOSessionCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetLODLevelCountCmd::RequestParams& lhs, const GetLODLevelCountCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetLODLevelCountCmd::ReplyParams& lhs, const GetLODLevelCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetLODLevelCountCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetLODLevelCountCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetMaxBrickSizeCmd::RequestParams& lhs, const GetMaxBrickSizeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetMaxBrickSizeCmd::ReplyParams& lhs, const GetMaxBrickSizeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetMaxBrickSizeCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetMaxBrickSizeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetMaxUsedBrickSizesCmd::RequestParams& lhs, const GetMaxUsedBrickSizesCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetMaxUsedBrickSizesCmd::ReplyParams& lhs, const GetMaxUsedBrickSizesCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetMaxUsedBrickSizesCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetMaxUsedBrickSizesCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetNumberOfTimestepsCmd::RequestParams& lhs, const GetNumberOfTimestepsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetNumberOfTimestepsCmd::ReplyParams& lhs, const GetNumberOfTimestepsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetNumberOfTimestepsCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetNumberOfTimestepsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDomainSizeCmd::RequestParams& lhs, const GetDomainSizeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetDomainSizeCmd::ReplyParams& lhs, const GetDomainSizeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDomainSizeCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetDomainSizeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetTransformationCmd::RequestParams& lhs, const GetTransformationCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetTransformationCmd::ReplyParams& lhs, const GetTransformationCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetTransformationCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetTransformationCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBrickOverlapSizeCmd::RequestParams& lhs, const GetBrickOverlapSizeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetBrickOverlapSizeCmd::ReplyParams& lhs, const GetBrickOverlapSizeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBrickOverlapSizeCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetBrickOverlapSizeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetLargestSingleBrickLODCmd::RequestParams& lhs, const GetLargestSingleBrickLODCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetLargestSingleBrickLODCmd::ReplyParams& lhs, const GetLargestSingleBrickLODCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetLargestSingleBrickLODCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetLargestSingleBrickLODCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBrickVoxelCountsCmd::RequestParams& lhs, const GetBrickVoxelCountsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetBrickVoxelCountsCmd::ReplyParams& lhs, const GetBrickVoxelCountsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBrickVoxelCountsCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetBrickVoxelCountsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBrickExtentsCmd::RequestParams& lhs, const GetBrickExtentsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetBrickExtentsCmd::ReplyParams& lhs, const GetBrickExtentsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBrickExtentsCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetBrickExtentsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBrickLayoutCmd::RequestParams& lhs, const GetBrickLayoutCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetBrickLayoutCmd::ReplyParams& lhs, const GetBrickLayoutCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBrickLayoutCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetBrickLayoutCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetModalityCountCmd::RequestParams& lhs, const GetModalityCountCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetModalityCountCmd::ReplyParams& lhs, const GetModalityCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetModalityCountCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetModalityCountCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetComponentCountCmd::RequestParams& lhs, const GetComponentCountCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetComponentCountCmd::ReplyParams& lhs, const GetComponentCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetComponentCountCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetComponentCountCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetRangeCmd::RequestParams& lhs, const GetRangeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetRangeCmd::ReplyParams& lhs, const GetRangeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRangeCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetRangeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetTotalBrickCountCmd::RequestParams& lhs, const GetTotalBrickCountCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetTotalBrickCountCmd::ReplyParams& lhs, const GetTotalBrickCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetTotalBrickCountCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetTotalBrickCountCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBrickCmd::RequestParams& lhs, const GetBrickCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetBrickCmd::ReplyParams& lhs, const GetBrickCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBrickCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetBrickCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetTypeCmd::RequestParams& lhs, const GetTypeCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetTypeCmd::ReplyParams& lhs, const GetTypeCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetTypeCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetTypeCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetSemanticCmd::RequestParams& lhs, const GetSemanticCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetSemanticCmd::ReplyParams& lhs, const GetSemanticCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetSemanticCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetSemanticCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDefault1DTransferFunctionCountCmd::RequestParams& lhs,
                const GetDefault1DTransferFunctionCountCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetDefault1DTransferFunctionCountCmd::ReplyParams& lhs,
                const GetDefault1DTransferFunctionCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCountCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCountCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDefault2DTransferFunctionCountCmd::RequestParams& lhs,
                const GetDefault2DTransferFunctionCountCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetDefault2DTransferFunctionCountCmd::ReplyParams& lhs,
                const GetDefault2DTransferFunctionCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDefault2DTransferFunctionCountCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetDefault2DTransferFunctionCountCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const Get1DHistogramCmd::RequestParams& lhs, const Get1DHistogramCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const Get1DHistogramCmd::ReplyParams& lhs, const Get1DHistogramCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const Get1DHistogramCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const Get1DHistogramCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const Get2DHistogramCmd::RequestParams& lhs, const Get2DHistogramCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const Get2DHistogramCmd::ReplyParams& lhs, const Get2DHistogramCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const Get2DHistogramCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const Get2DHistogramCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetUserDefinedSemanticCmd::RequestParams& lhs, const GetUserDefinedSemanticCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetUserDefinedSemanticCmd::ReplyParams& lhs, const GetUserDefinedSemanticCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetUserDefinedSemanticCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetUserDefinedSemanticCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDefault1DTransferFunctionCmd::RequestParams& lhs, const GetDefault1DTransferFunctionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetDefault1DTransferFunctionCmd::ReplyParams& lhs, const GetDefault1DTransferFunctionCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}

std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetDomainScaleCmd::RequestParams& lhs, const GetDomainScaleCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDomainScaleCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetDomainScaleCmd::ReplyParams& lhs, const GetDomainScaleCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetDomainScaleCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetFloatBrickLayoutCmd::RequestParams& lhs, const GetFloatBrickLayoutCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetFloatBrickLayoutCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetFloatBrickLayoutCmd::ReplyParams& lhs, const GetFloatBrickLayoutCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetFloatBrickLayoutCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetRootsCmd::RequestParams& lhs, const GetRootsCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRootsCmd::RequestParams& obj) {
    return os << obj.toString();
}
bool operator==(const GetRootsCmd::ReplyParams& lhs, const GetRootsCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetRootsCmd::ReplyParams& obj) {
    return os << obj.toString();
}

bool operator==(const GetBrickMetaDataCmd::RequestParams& lhs, const GetBrickMetaDataCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const GetBrickMetaDataCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const GetBrickMetaDataCmd::ReplyParams& obj) {
    return os << obj.toString();
}

/* AUTOGEN CommandImplOperators */
}