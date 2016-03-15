#include "commands/ErrorCommands.h"

using namespace trinity;

VclType ErrorCmd::Type = VclType::TrinityError;

ErrorCmd::ReplyParams::ReplyParams(int errorCode)
    : m_errorCode(errorCode) {}

void ErrorCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendInt("code", m_errorCode);
}

void ErrorCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_errorCode = reader.getInt32("code");
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

std::string ErrorCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "errorCode: " << m_errorCode;
    return stream.str();
}

namespace trinity {
bool operator==(const ErrorCmd::ReplyParams& lhs, const ErrorCmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const ErrorCmd::ReplyParams& obj) {
    return os << obj.toString();
}
}