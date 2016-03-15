#include "commands/IOCommands.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/base/StringTools.h"

using namespace trinity;


////////////// IOData //////////////

const mocca::BidirectionalMap<IOData::DataType, std::string>& IOData::dataTypeMapper() {
    static mocca::BidirectionalMap<DataType, std::string> mapper;
    if (mapper.empty()) {
        mapper.insert(DataType::Dataset, "dataset");
        mapper.insert(DataType::Directory, "directory");
    }
    return mapper;
}

IOData::IOData(const std::string& name, int fileId, DataType dataType)
    : m_name(name)
    , m_fileId(fileId)
    , m_dataType(dataType) {}

void IOData::serialize(ISerialWriter& writer) const {
    writer.appendString("name", m_name);
    writer.appendInt("fileid", m_fileId);
    writer.appendString("datatype", dataTypeMapper().getByFirst(m_dataType));
}

void IOData::deserialize(const ISerialReader& reader) {
    m_name = reader.getString("name");
    m_fileId = reader.getInt32("fileid");
    m_dataType = dataTypeMapper().getBySecond(reader.getString("datatype"));
}

std::string IOData::getName() const {
    return m_name;
}

int IOData::getFileId() const {
    return m_fileId;
}

IOData::DataType IOData::getDataType() const {
    return m_dataType;
}

bool IOData::equals(const IOData& other) const {
    return m_name == other.m_name && m_fileId == other.m_fileId && m_dataType == other.m_dataType;
}

std::string IOData::toString() const {
    std::stringstream stream;
    stream << "name: " << m_name << "; fileId: " << m_fileId << "; dataType: " << dataTypeMapper().getByFirst(m_dataType);
    return stream.str();
}

////////////// ListFilesCmd //////////////

VclType ListFilesCmd::Type = VclType::ListFiles;

ListFilesCmd::RequestParams::RequestParams(int32_t dirID)
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
    writer.appendInt("dirid", m_dirID);
}

void ListFilesCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_dirID = reader.getInt32("dirid");
}

int32_t ListFilesCmd::RequestParams::getDirID() const {
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

InitIOSessionCmd::RequestParams::RequestParams(const std::string& protocol, int fileId)
    : m_protocol(protocol)
    , m_fileId(fileId) {}

void InitIOSessionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.appendString("protocol", m_protocol);
    writer.appendInt("fileid", m_fileId);
}

void InitIOSessionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_protocol = reader.getString("protocol");
    m_fileId = reader.getInt32("fileid");
}

std::string InitIOSessionCmd::RequestParams::getProtocol() const {
    return m_protocol;
}

int InitIOSessionCmd::RequestParams::getFileId() const {
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

void GetLODLevelCountCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void GetLODLevelCountCmd::RequestParams::deserialize(const ISerialReader& reader) {}

bool GetLODLevelCountCmd::RequestParams::equals(const GetLODLevelCountCmd::RequestParams& other) const {
    return true;
}

std::string GetLODLevelCountCmd::RequestParams::toString() const {
    return std::string();
}

GetLODLevelCountCmd::ReplyParams::ReplyParams(int lodCount)
    : m_lodCount(lodCount) {}

void GetLODLevelCountCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("lodcount", m_lodCount);
}

void GetLODLevelCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_lodCount = reader.getInt32("lodcount");
}

int GetLODLevelCountCmd::ReplyParams::getLODLevelCount() const {
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

#undef PYTHON_MAGIC
}