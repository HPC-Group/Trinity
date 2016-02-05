#include "StreamingParams.h"

using namespace trinity::common;


StreamingParams::StreamingParams(int resX, int resY) : m_resX(resX), m_resY(resY) {}

StreamingParams::StreamingParams() : m_resX(1024), m_resY(768) {}

void StreamingParams::serialize(ISerialObject& serial) const {

    serial.append("xres", m_resX);
    serial.append("yres", m_resY);
}


void StreamingParams::deserialize(ISerialObject& serial) {

    m_resX = serial.getInt("xres");
    m_resY = serial.getInt("yres");
}

int StreamingParams::getResX() const { return m_resX; }
int StreamingParams::getResY() const { return m_resY; }

VclType StreamingParams::getType() const {
    return VclType::StreamingParams;
}