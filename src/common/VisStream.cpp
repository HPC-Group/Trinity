#include "VisStream.h"

#include <chrono>


using namespace trinity::common;
using namespace trinity::commands;

VisStream::VisStream(StreamingParams params)
    : m_streamingParams(params) {}

const StreamingParams& VisStream::getStreamingParams() const {
    return m_streamingParams;
}


void VisStream::put(Frame frame) {
    m_queue.enqueue(std::move(frame));
}


mocca::Nullable<Frame> VisStream::get() {
    return m_queue.dequeue(std::chrono::milliseconds(100));
}