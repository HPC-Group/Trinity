#include "VisStream.h"

using namespace trinity::common;

VisStream::VisStream(StreamParams params) : m_streamParams(params),
m_lastFrame(params.frameSize()), m_swapBuffer(params.frameSize()) {
    
}

VisStream::~VisStream() {
    
}

const StreamParams& VisStream::getStreamParams() const {
    return m_streamParams;
}


void VisStream::swapBuffers() {
    m_isSwapping = true;
    swap(m_lastFrame, m_swapBuffer);
    m_isSwapping = false;
}


bool VisStream::isSwapping() const {
    return m_isSwapping;
}

const Frame& VisStream::readLastFrame() {
    return m_lastFrame;
}


void VisStream::insertFrame(Frame& frame) {
    
    swap(m_swapBuffer, frame);  // todo check if it works
}