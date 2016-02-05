#include "ErrorCmd.h"

using namespace trinity::common;

ErrorCmd::ErrorCmd() : ICommand(0, 0){}

ErrorCmd::ErrorCmd(int sid, int rid, int errorCode) :
ICommand(sid, rid), m_errorCode(errorCode) {}
ErrorCmd::~ErrorCmd() {}

VclType ErrorCmd::getType() const {
    return VclType::TrinityError;
}


void ErrorCmd::serialize(ISerialObject& serial) const {

    serial.setType(getType());
    serial.append("sid", m_sid);
    serial.append("rid", m_rid);
    serial.append("code", m_errorCode);
    
}


void ErrorCmd::deserialize(ISerialObject& serial) {

    m_sid = serial.getInt("sid");
    m_rid = serial.getInt("rid");
    m_errorCode = serial.getInt("code");
}


const std::string ErrorCmd::printError() {
    return m_vcl.toString(m_errorCode);
}