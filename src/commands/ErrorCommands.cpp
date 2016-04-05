#include "commands/ErrorCommands.h"

using namespace trinity;

VclType ErrorCmd::Type = VclType::TrinityError;

ErrorCmd::ReplyParams::ReplyParams(const std::string& error)
    : m_error(error) {}

void ErrorCmd::ReplyParams::serialize(ISerialWriter& writer) const {
    writer.appendString("error", m_error);
}

void ErrorCmd::ReplyParams::deserialize(const ISerialReader& reader) {
    m_error = reader.getString("error");
}

std::string ErrorCmd::ReplyParams::getError() const {
    return m_error;
}

bool ErrorCmd::ReplyParams::equals(const ReplyParams& other) const {
    return m_error == other.m_error;
}

std::string ErrorCmd::ReplyParams::toString() const {
    std::stringstream stream;
    stream << "error: " << m_error;
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