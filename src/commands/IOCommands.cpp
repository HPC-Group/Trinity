#include "commands/IOCommands.h"

using namespace trinity::commands;

////////////// ListFilesCmd //////////////

VclType ListFilesCmd::Type = VclType::ListFiles;

bool ListFilesCmd::RequestParams::equals(const ListFilesCmd::RequestParams& other) const {
    return true;
}

std::string ListFilesCmd::RequestParams::toString() const {
    return std::string();
}

void ListFilesCmd::RequestParams::serialize(ISerialWriter& writer) const {}

void ListFilesCmd::RequestParams::deserialize(const ISerialReader& reader) {}


////////////// InitIOSessionCmd //////////////

VclType InitIOSessionCmd::Type = VclType::InitIOSession;

InitIOSessionCmd::RequestParams::RequestParams(const std::string& protocol, int fileId)
    : m_protocol(protocol)
    , m_fileId(fileId) {}

void InitIOSessionCmd::RequestParams::serialize(ISerialWriter& writer) const {
    writer.append("protocol", m_protocol);
    writer.append("fileid", m_fileId);
}

void InitIOSessionCmd::RequestParams::deserialize(const ISerialReader& reader) {
    m_protocol = reader.getString("protocol");
    m_fileId = reader.getInt("fileid");
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

InitIOSessionCmd::ReplyParams::ReplyParams(int controlPort)
    : m_controlPort(controlPort) {}

void InitIOSessionCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.append("controlport", m_controlPort);
}

void InitIOSessionCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_controlPort = reader.getInt("controlport");
}

int InitIOSessionCmd::ReplyParams::getControlPort() const {
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
    writer.append("lodcount", m_lodCount);
}

void GetLODLevelCountCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_lodCount = reader.getInt("lodcount");
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

////////////// IOData //////////////

IOData::IOData(const std::string& name, int fileId, const VclType& dataType)
    : m_name(name)
    , m_fileId(fileId)
    , m_dataType(dataType) {}

void IOData::serialize(ISerialWriter& writer) const {
    writer.append("name", m_name);
    writer.append("fileid", m_fileId);
    writer.append("datatype", Vcl::instance().toString(m_dataType));
}

void IOData::deserialize(const ISerialReader& reader) {
    m_name = reader.getString("name");
    m_fileId = reader.getInt("fileid");
    m_dataType = Vcl::instance().toType(reader.getString("datatype"));
}

std::string IOData::getName() const {
    return m_name;
}

int IOData::getFileId() const {
    return m_fileId;
}

VclType IOData::getDataType() const {
    return m_dataType;
}

bool IOData::equals(const IOData& other) const {
    return m_name == other.m_name && m_fileId == other.m_fileId && m_dataType == other.m_dataType;
}

std::string IOData::toString() const {
    std::stringstream stream;
    stream << "name: " << m_name << "; fileId: " << m_fileId << "; dataType: " << m_dataType;
    return stream.str();
}

namespace trinity {
namespace commands {
bool operator==(const ListFilesCmd::RequestParams& lhs, const ListFilesCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ListFilesCmd::RequestParams& obj) {
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

bool operator==(const IOData& lhs, const IOData& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const IOData& obj) {
    return os << obj.toString();
}
}
}