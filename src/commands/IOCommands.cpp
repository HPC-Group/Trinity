#include "IOCommands.h"

using namespace trinity::commands;

ListFilesCmd::ListFilesCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

ListFilesCmd::ListFilesCmd(int sid, int rid)
    : ICommand(sid, rid) {}

VclType ListFilesCmd::getType() const {
    return VclType::ListFiles;
}

void ListFilesCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
}

void ListFilesCmd::deserialize(ISerialObject& serial) {
    ICommand::deserialize(serial);
}

std::string ListFilesCmd::toString() const {
    std::stringstream stream;
    stream << ICommand::toString();
    return stream.str();
}

InitIOSessionCmd::InitIOSessionCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

InitIOSessionCmd::InitIOSessionCmd(int sid, int rid, const std::string& protocol, int fileId)
    : ICommand(sid, rid)
    , m_protocol(protocol)
    , m_fileId(fileId) {}

VclType InitIOSessionCmd::getType() const {
    return VclType::InitIOSession;
}

void InitIOSessionCmd::serialize(ISerialObject& serial) const {

    ICommand::serialize(serial);
    serial.append("protocol", m_protocol);
    serial.append("fileid", m_fileId);
}

void InitIOSessionCmd::deserialize(ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_protocol = serial.getString("protocol");
    m_fileId = serial.getInt("fileid");
}

std::string InitIOSessionCmd::toString() const {
    std::stringstream stream;
    stream << ICommand::toString() << "protocol: " << m_protocol << std::endl
           << "fileid: " << m_fileId << std::endl;
    return stream.str();
}

const std::string& InitIOSessionCmd::getProtocol() const {
    return m_protocol;
}

int InitIOSessionCmd::getFileId() const {
    return m_fileId;
}

ReplyInitIOSessionCmd::ReplyInitIOSessionCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

ReplyInitIOSessionCmd::ReplyInitIOSessionCmd(int sid, int rid)
    : ICommand(sid, rid) {}

VclType ReplyInitIOSessionCmd::getType() const {
    return VclType::TrinityReturn;
}

void ReplyInitIOSessionCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
    serial.append("controlport", m_controlPort);
}

void ReplyInitIOSessionCmd::deserialize(ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_controlPort = serial.getInt("controlport");
}

std::string ReplyInitIOSessionCmd::toString() const {
    std::stringstream stream;
    stream << ICommand::toString() << "controlport: " << m_controlPort << std::endl;
    return stream.str();
}

void ReplyInitIOSessionCmd::setNewSid(int sid) {
    m_sid = sid;
}

void ReplyInitIOSessionCmd::setControlPort(const int port) {
    m_controlPort = port;
}

int ReplyInitIOSessionCmd::getControlPort() const {
    return m_controlPort;
}

GetLODLevelCountCmd::GetLODLevelCountCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

GetLODLevelCountCmd::GetLODLevelCountCmd(int sid, int rid)
    : ICommand(sid, rid) {}

VclType GetLODLevelCountCmd::getType() const {
    return VclType::GetLODLevelCount;
}

void GetLODLevelCountCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
}

void GetLODLevelCountCmd::deserialize(ISerialObject& serial) {
    ICommand::deserialize(serial);
}

std::string GetLODLevelCountCmd::toString() const {
    std::stringstream stream;
    stream << ICommand::toString();
    return stream.str();
}

ReplyGetLODLevelCountCmd::ReplyGetLODLevelCountCmd(ISerialObject& obj)
    : ICommand(0, 0) {
    deserialize(obj);
}

ReplyGetLODLevelCountCmd::ReplyGetLODLevelCountCmd(int sid, int rid)
    : ICommand(sid, rid) {}

VclType ReplyGetLODLevelCountCmd::getType() const {
    return VclType::TrinityReturn;
}

void ReplyGetLODLevelCountCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
    serial.append("lodcount", m_lodCount);
}

void ReplyGetLODLevelCountCmd::deserialize(ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_lodCount = serial.getInt("lodcount");
}

std::string ReplyGetLODLevelCountCmd::toString() const {
    std::stringstream stream;
    stream << ICommand::toString() << "lodcount: " << m_lodCount << std::endl;
    return stream.str();
}

int ReplyGetLODLevelCountCmd::getLODLevelCount() const {
    return m_lodCount;
}
void ReplyGetLODLevelCountCmd::setLODLevelCount(int lod) {
    m_lodCount = lod;
}

IOData::IOData(const std::string& name, int fileId, const VclType& dataType)
    : m_name(name)
    , m_fileId(fileId)
    , m_dataType(dataType) {}

VclType IOData::getType() const {
    return VclType::IOData;
}

void IOData::serialize(ISerialObject& serial) const {
    serial.append("name", m_name);
    serial.append("fileid", m_fileId);
    serial.append("datatype", Vcl::instance().toString(m_dataType));
}

void IOData::deserialize(ISerialObject& serial) {
    serial.append("name", m_name); // ???
    m_name = serial.getString("name");
    serial.append("fileid", m_fileId); // ???
    m_fileId = serial.getInt("fileid");
    m_dataType = Vcl::instance().toType(serial.getString("datatype"));
}

std::string trinity::commands::IOData::toString() const {
    std::stringstream stream;
    stream << ISerializable::toString() << "name: " << m_name << std::endl
           << "fileid: " << m_fileId << std::endl
           << "datatype: " << m_dataType << std::endl;
    return stream.str();
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
