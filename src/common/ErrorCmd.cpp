#include "ErrorCmd.h"

using namespace trinity::common;

ErrorCmd::ErrorCmd() : ICommand(0, 0){}

ErrorCmd::ErrorCmd(int sid, int rid, int errorCode) :
ICommand(sid, rid), m_errorCode(errorCode) {}
ErrorCmd::~ErrorCmd() {}

VclType ErrorCmd::getType() const {
    return VclType::TrinityError;
}


void ErrorCmd::serialize(std::ostream& stream) {
    stream << m_vcl.toString(getType()) << " " << m_sid << " "
    << m_rid << " " << m_errorCode;
}


void ErrorCmd::deserialize(std::istream& stream) {
    std::string typeString;
    stream >> m_sid >> m_rid >> m_errorCode;
}


const std::string& ErrorCmd::printError() {
    return m_vcl.toString(m_errorCode);
}