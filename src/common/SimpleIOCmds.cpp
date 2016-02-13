#include "SimpleIOCmds.h"

using namespace trinity::common;

GetLODLevelCountCmd::GetLODLevelCountCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

GetLODLevelCountCmd::GetLODLevelCountCmd(int sid, int rid) :
ICommand(sid, rid) {}
GetLODLevelCountCmd::~GetLODLevelCountCmd() {}

VclType GetLODLevelCountCmd::getType() const {
    return VclType::GetLODLevelCount;
}


void GetLODLevelCountCmd::serialize(ISerialObject& serial) const {

    ICommand::serialize(serial);
}


void GetLODLevelCountCmd::deserialize(ISerialObject& serial) {

    ICommand::deserialize(serial);
}



ReplyGetLODLevelCountCmd::ReplyGetLODLevelCountCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

ReplyGetLODLevelCountCmd::ReplyGetLODLevelCountCmd(int sid, int rid) :
ICommand(sid, rid) {}
ReplyGetLODLevelCountCmd::~ReplyGetLODLevelCountCmd() {}

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

int ReplyGetLODLevelCountCmd::getLODLevelCount() const { return m_lodCount; }
void ReplyGetLODLevelCountCmd::setLODLevelCount(int lod) { m_lodCount = lod; }