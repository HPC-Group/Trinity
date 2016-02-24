#include "commands/IOCommands.h"

using namespace trinity::commands;

////////////// ListFilesCmd //////////////

VclType ListFilesCmd::Type = VclType::ListFiles;

void ListFilesCmd::RequestParams::serialize(ISerialObject& serial) const {}

void ListFilesCmd::RequestParams::deserialize(const ISerialObject& serial) {}


////////////// InitIOSessionCmd //////////////

VclType InitIOSessionCmd::Type = VclType::InitIOSession;

InitIOSessionCmd::RequestParams::RequestParams(const std::string& protocol, int fileId)
    : m_protocol(protocol)
    , m_fileId(fileId) {}

void InitIOSessionCmd::RequestParams::serialize(ISerialObject& serial) const {
    serial.append("protocol", m_protocol);
    serial.append("fileid", m_fileId);
}

void InitIOSessionCmd::RequestParams::deserialize(const ISerialObject& serial) {
    m_protocol = serial.getString("protocol");
    m_fileId = serial.getInt("fileid");
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

InitIOSessionCmd::ReplyParams::ReplyParams(int controlPort)
    : m_controlPort(controlPort) {}

void InitIOSessionCmd::ReplyParams::serialize(ISerialObject& serial) const {
    serial.append("controlport", m_controlPort);
}

void InitIOSessionCmd::ReplyParams::deserialize(const ISerialObject& serial) {
    m_controlPort = serial.getInt("controlport");
}

int InitIOSessionCmd::ReplyParams::getControlPort() const {
    return m_controlPort;
}

bool InitIOSessionCmd::ReplyParams::equals(const InitIOSessionCmd::ReplyParams& other) const {
    return m_controlPort == other.m_controlPort;
}


////////////// GetLODLevelCountCmd //////////////

VclType GetLODLevelCountCmd::Type = VclType::GetLODLevelCount;

void GetLODLevelCountCmd::RequestParams::serialize(ISerialObject& serial) const {}

void GetLODLevelCountCmd::RequestParams::deserialize(const ISerialObject& serial) {}

GetLODLevelCountCmd::ReplyParams::ReplyParams(int lodCount)
    : m_lodCount(lodCount) {}

void GetLODLevelCountCmd::ReplyParams::serialize(ISerialObject& serial) const {
    serial.append("lodcount", m_lodCount);
}

void GetLODLevelCountCmd::ReplyParams::deserialize(const ISerialObject& serial) {
    m_lodCount = serial.getInt("lodcount");
}

int GetLODLevelCountCmd::ReplyParams::getLODLevelCount() const {
    return m_lodCount;
}

bool GetLODLevelCountCmd::ReplyParams::equals(const GetLODLevelCountCmd::ReplyParams& other) const {
    return m_lodCount == other.m_lodCount;
}

////////////// IOData //////////////

IOData::IOData(const std::string& name, int fileId, const VclType& dataType)
    : m_name(name)
    , m_fileId(fileId)
    , m_dataType(dataType) {}

void IOData::serialize(ISerialObject& serial) const {
    serial.append("name", m_name);
    serial.append("fileid", m_fileId);
    serial.append("datatype", Vcl::instance().toString(m_dataType));
}

void IOData::deserialize(const ISerialObject& serial) {
    m_name = serial.getString("name");
    m_fileId = serial.getInt("fileid");
    m_dataType = Vcl::instance().toType(serial.getString("datatype"));
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


namespace trinity {
namespace commands {
bool operator==(const InitIOSessionCmd::RequestParams& lhs, const InitIOSessionCmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const InitIOSessionCmd::ReplyParams& lhs, const InitIOSessionCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const GetLODLevelCountCmd::ReplyParams& lhs, const GetLODLevelCountCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const IOData& lhs, const IOData& rhs) {
    return lhs.equals(rhs);
}
}
}