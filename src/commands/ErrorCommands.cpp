#include "ErrorCommands.h"

using namespace trinity::commands;

ErrorCmd::ErrorCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

ErrorCmd::ErrorCmd(int sid, int rid, int errorCode) :
ICommand(sid, rid), m_errorCode(errorCode) {}

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

std::string ErrorCmd::toString() const {
    std::stringstream stream;
    stream << ICommand::toString() << "code: " << m_errorCode << std::endl;
    return stream.str();
}

const std::string ErrorCmd::printError() {
    return Vcl::instance().toString(m_errorCode);
}

int ErrorCmd::getErrorCode() const {
    return m_errorCode;
}