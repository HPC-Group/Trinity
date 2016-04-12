#include "VisStream.h"

#include <chrono>

using namespace trinity;

VisStream::VisStream(StreamingParams params)
    : m_streamingParams(params) {}

const StreamingParams& VisStream::getStreamingParams() const {
    return m_streamingParams;
}

void VisStream::put(Frame frame) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_frame = std::move(frame);
    m_cv.notify_one();
}

Frame VisStream::get() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (!m_cv.wait_for(lock, std::chrono::milliseconds(10), [&] { return !m_frame.empty(); })) {
        return Frame();
    }
    return std::move(m_frame);
}