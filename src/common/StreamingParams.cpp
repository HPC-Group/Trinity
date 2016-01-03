#include "StreamingParams.h"

using namespace trinity::common;


StreamingParams::StreamingParams(int resX, int resY) : m_resX(resX), m_resY(resY) {}

StreamingParams::StreamingParams() : m_resX(1024), m_resY(768) {}

void StreamingParams::serialize(std::ostream& stream) {
    stream << m_resX << " " << m_resY;
}


void StreamingParams::deserialize(std::istream& stream) {
    stream >> m_resX >> m_resY;
}

int StreamingParams::getResX() const { return m_resX; }
int StreamingParams::getResY() const { return m_resY; }