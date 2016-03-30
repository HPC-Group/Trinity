#include "VisStreamQueue.h"

#include <chrono>

using namespace trinity;

VisStreamQueue::VisStreamQueue(StreamingParams params)
    : m_streamingParams(params) {}

const StreamingParams& VisStreamQueue::getStreamingParams() const {
    return m_streamingParams;
}

void VisStreamQueue::put(Frame frame) {
    m_queue.enqueue(std::move(frame));
}

mocca::Nullable<Frame> VisStreamQueue::get() {
    return m_queue.dequeue(std::chrono::milliseconds(100));
}