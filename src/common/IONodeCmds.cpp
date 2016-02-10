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





InitIOSession::InitIOSession(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

InitIOSession::InitIOSession(int sid, int rid) :
ICommand(sid, rid) {}
InitIOSession::~InitIOSession() {}

VclType InitIOSession::getType() const {
    return VclType::InitIOSession;
}


void InitIOSession::serialize(ISerialObject& serial) const {
    
    ICommand::serialize(serial);
}


void InitIOSession::deserialize(ISerialObject& serial) {
    
    ICommand::deserialize(serial);
}

