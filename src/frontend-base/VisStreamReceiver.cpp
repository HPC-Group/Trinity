#include "VisStreamReceiver.h"
#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity::frontend;
using namespace trinity::common;

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

    while (!isInterrupted()) {
        auto bytepacket = m_connection->receive();

        if (!bytepacket.isEmpty()) {

            m_visStream->put(std::unique_ptr<mocca::ByteArray>(new mocca::ByteArray(bytepacket.clone())));
        }
    }

    // cleanup
    if (m_connection)
        m_connection.reset();
}