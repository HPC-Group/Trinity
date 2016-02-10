#include "SimpleRenderingCmds.h"

using namespace trinity::common;

SetIsoValueCmd::SetIsoValueCmd(ISerialObject& obj) : ICommand(0,0) {
    deserialize(obj);
}

SetIsoValueCmd::SetIsoValueCmd(int sid, int rid, float value) :
ICommand(sid, rid), m_isoValue(value) {}
SetIsoValueCmd::~SetIsoValueCmd() {}

VclType SetIsoValueCmd::getType() const {
    return VclType::SetIsoValue;
}


void SetIsoValueCmd::serialize(ISerialObject& serial) const {

    ICommand::serialize(serial);
    serial.append("isovalue", m_isoValue);
    
}


void SetIsoValueCmd::deserialize(ISerialObject& serial) {

    ICommand::deserialize(serial);
    m_isoValue = serial.getFloat("isovalue");
}

float SetIsoValueCmd::getIsoValue() const { return m_isoValue; }
