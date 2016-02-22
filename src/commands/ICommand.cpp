#include "commands/ICommand.h"

using namespace trinity::commands;

ICommand::ICommand(int sid, int rid)
    : m_sid(sid)
    , m_rid(rid) {}

void ICommand::serialize(ISerialObject& serial) const {
    serial.setType(getType());
    serial.append("sid", m_sid);
    serial.append("rid", m_rid);
}

void ICommand::deserialize(ISerialObject& serial) {
    m_sid = serial.getInt("sid");
    m_rid = serial.getInt("rid");
}

std::string ICommand::toString() const {
    std::stringstream stream;
    stream << ISerializable::toString() << "sid: " << m_sid << std::endl << "rid: " << m_rid << std::endl;
    return stream.str();
}

int ICommand::getSid() const {
    return m_sid;
}

int ICommand::getRid() const {
    return m_rid;
}
