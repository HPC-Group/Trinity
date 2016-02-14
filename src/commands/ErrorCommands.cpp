#include "ErrorCommands.h"

using namespace trinity::commands;

ErrorCmd::ErrorCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

ErrorCmd::ErrorCmd(int sid, int rid, int errorCode) :
ICommand(sid, rid), m_errorCode(errorCode) {}
ErrorCmd::~ErrorCmd() {}

VclType ErrorCmd::getType() const {
    return VclType::TrinityError;
}


void ErrorCmd::serialize(ISerialObject& serial) const {
    ICommand::serialize(serial);
    serial.append("code", m_errorCode);
    
}


void ErrorCmd::deserialize(ISerialObject& serial) {
    ICommand::deserialize(serial);
    m_errorCode = serial.getInt("code");
}


const std::string ErrorCmd::printError() {
    return m_vcl.toString(m_errorCode);
}

int ErrorCmd::getErrorCode() const {
    return m_errorCode;
}