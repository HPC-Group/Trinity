#include "IONodeCmds.h"

using namespace trinity::common;

ListFilesCmd::ListFilesCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

ListFilesCmd::ListFilesCmd(int sid, int rid) :
ICommand(sid, rid) {}
ListFilesCmd::~ListFilesCmd() {}

VclType ListFilesCmd::getType() const {
    return VclType::ListFiles;
}


void ListFilesCmd::serialize(ISerialObject& serial) const {

    ICommand::serialize(serial);
}


void ListFilesCmd::deserialize(ISerialObject& serial) {

    ICommand::deserialize(serial);
}



InitIOSessionCmd::InitIOSessionCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

InitIOSessionCmd::InitIOSessionCmd(int sid, int rid, const std::string& protocol, int fileId) :
ICommand(sid, rid), m_protocol(protocol), m_fileId(fileId) {}
InitIOSessionCmd::~InitIOSessionCmd() {}

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


const std::string& InitIOSessionCmd::getProtocol() const {
    return m_protocol;
}

int InitIOSessionCmd::getFileId() const {
    return m_fileId;
}



ReplyInitIOSessionCmd::ReplyInitIOSessionCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

ReplyInitIOSessionCmd::ReplyInitIOSessionCmd(int sid, int rid) :
ICommand(sid, rid) {}
ReplyInitIOSessionCmd::~ReplyInitIOSessionCmd() {}

VclType ReplyInitIOSessionCmd::getType() const {
    return VclType::TrinityReturn;
}


void ReplyInitIOSessionCmd::serialize(ISerialObject& serial) const {
    
    ICommand::serialize(serial);
}


void ReplyInitIOSessionCmd::deserialize(ISerialObject& serial) {
    
    ICommand::deserialize(serial);
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

