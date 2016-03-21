#include "commands/IOCommands.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/base/StringTools.h"

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

#define PYTHON_MAGIC_DEFINITION


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


////////////// MaxMinForKeyCmd //////////////

VclType MaxMinForKeyCmd::Type = VclType::MaxMinForKey;

MaxMinForKeyCmd::RequestParams::RequestParams(const BrickKey& brickKey)
    : m_brickKey(brickKey) {}

void MaxMinForKeyCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("brickKey", m_brickKey);
}

void MaxMinForKeyCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_brickKey = reader.getSerializable<BrickKey>("brickKey");
}

bool MaxMinForKeyCmd::RequestParams::equals(const MaxMinForKeyCmd::RequestParams& other) const {
    return m_brickKey == other.m_brickKey;
}

std::string MaxMinForKeyCmd::RequestParams::toString() const {
    std::stringstream stream;
    stream << "brickKey: " << m_brickKey;
    return stream.str();
}

BrickKey MaxMinForKeyCmd::RequestParams::getBrickKey() const {
    return m_brickKey;
}

MaxMinForKeyCmd::ReplyParams::ReplyParams(const MinMaxBlock& minMaxBlock)
    : m_minMaxBock(minMaxBlock) {}

void MaxMinForKeyCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendObject("minMaxBlock", m_minMaxBock);
}

void MaxMinForKeyCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_minMaxBock = reader.getSerializable<MinMaxBlock>("minMaxBlock");
}

bool MaxMinForKeyCmd::ReplyParams::equals(const MaxMinForKeyCmd::ReplyParams& other) const {
    return m_minMaxBock == other.m_minMaxBock;
}

std::string MaxMinForKeyCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << m_minMaxBock;
    return stream.str();
}

MinMaxBlock MaxMinForKeyCmd::ReplyParams::getMinMaxBlock() const {
    return m_minMaxBock;
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

GetBrickCmd::ReplyParams::ReplyParams(std::vector<uint8_t> brick, bool success)
    : m_success(success),
    m_brick(std::move(brick))
     {}

void GetBrickCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendBool("success", m_success);
    writer.appendIntVec("brick", m_brick);
}

void GetBrickCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_success = reader.getBool("success");
    m_brick = reader.getUInt8Vec("brick");
}

bool GetBrickCmd::ReplyParams::equals(const GetBrickCmd::ReplyParams& other) const {
    return m_success == other.m_success && m_brick == other.m_brick;
}

std::string GetBrickCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "success: " << m_success;
    stream << "; brick: ";
    ::operator<<(stream, m_brick); // ugly, but necessary because of namespaces
    return stream.str();
}

bool GetBrickCmd::ReplyParams::getSuccess() const {
    return m_success;
}

const std::vector<uint8_t>& GetBrickCmd::ReplyParams::getBrick() const {
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

#undef PYTHON_MAGIC_DEFINITION


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

#define PYTHON_MAGIC


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

bool operator==(const MaxMinForKeyCmd::RequestParams& lhs, const MaxMinForKeyCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const MaxMinForKeyCmd::ReplyParams& lhs, const MaxMinForKeyCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const MaxMinForKeyCmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const MaxMinForKeyCmd::ReplyParams& obj) {
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

#undef PYTHON_MAGIC
}