#include "commands/ErrorCommands.h"

using namespace trinity::commands;

VclType ErrorCmd::Type = VclType::TrinityError;

ErrorCmd::ReplyParams::ReplyParams(int errorCode)
    : m_errorCode(errorCode) {}

void ErrorCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.append("code", m_errorCode);
}

void ErrorCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_errorCode = reader.getInt("code");
}

std::string ErrorCmd::ReplyParams::printError() const {
    return Vcl::instance().toString(m_errorCode);
}

int ErrorCmd::ReplyParams::getErrorCode() const {
    return m_errorCode;
}

bool ErrorCmd::ReplyParams::equals(const ReplyParams& other) const {
    return m_errorCode == other.m_errorCode;
}

namespace trinity {
namespace commands {
bool operator==(const ErrorCmd::ReplyParams& lhs, const ErrorCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
}
}