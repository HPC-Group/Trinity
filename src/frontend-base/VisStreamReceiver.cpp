#include "VisStreamReceiver.h"

#include "silverbullet/base/DetectEnv.h"

#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/Thread.h"

#if !defined(DETECTED_IOS_SIMULATOR) && !defined(DETECTED_IOS)
#include "jpeg/JPEGDecoder.h"
#endif

using namespace trinity;

VisStreamReceiver::VisStreamReceiver(const mocca::net::Endpoint endpoint, std::shared_ptr<VisStream> s)
    : m_visStream(s)
    , m_endpoint(endpoint) {}

VisStreamReceiver::~VisStreamReceiver() {
    join();
}

void VisStreamReceiver::startStreaming() {
    start();
}

void VisStreamReceiver::endStreaming() {
    interrupt();
}

void VisStreamReceiver::run() {

    LINFO("(f) vis receiver connecting to \"" + m_endpoint.toString() + "\"");

    try {

        while (!m_connection && !isInterrupted()) {
            m_connection = mocca::net::ConnectionFactorySelector::connect(m_endpoint);
        }

    } catch (const mocca::net::NetworkError& err) {
        LERROR("(f) cannot bind vis receiver: " << err.what());
        return;
    }

    LINFO("(f) vis receiver connected");

#if !defined(DETECTED_IOS_SIMULATOR) && !defined(DETECTED_IOS)
    JPEGDecoder jpeg(JPEGDecoder::Format_RGBA, false);
#endif
    try {
        while (!isInterrupted()) {
            auto bytepacket = m_connection->receive();
            if (!bytepacket.empty()) {
#if !defined(DETECTED_IOS_SIMULATOR) && !defined(DETECTED_IOS)
                auto frame = jpeg.decode(*bytepacket[0]);
                m_visStream->put(std::move(frame));
#else
                m_visStream->put(std::move(*bytepacket[0]));
#endif
            }
        }
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(f) receiving visualization failed: " << err.what());
    }
    catch (const mocca::ThreadInterrupt&) {
        LDEBUG("VisStream thread interrupted");
    }

    // cleanup
    if (m_connection)
        m_connection.reset();
}