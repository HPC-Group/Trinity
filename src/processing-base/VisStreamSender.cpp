#include "VisStreamSender.h"
#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

VisStreamSender::VisStreamSender(const mocca::net::Endpoint endpoint, std::shared_ptr<VisStream> s)
    : m_visStream(s) {
    m_acceptor = std::move(mocca::net::ConnectionFactorySelector::bind(endpoint));
}

VisStreamSender::~VisStreamSender() {
    join();
}

void VisStreamSender::startStreaming() {
    start();
}

void VisStreamSender::endStreaming() {
    interrupt();
}

std::string VisStreamSender::getPort() const {
    return m_acceptor->localEndpoint()->port;
}

void VisStreamSender::run() {
    LINFO("(p) vis sender binding to  \"" << *m_acceptor->localEndpoint() << "\"");

    try {

        while (!m_connection && !isInterrupted()) {
            m_connection = m_acceptor->accept();
        }

    } catch (const mocca::net::NetworkError& err) {
        LERROR("(p) cannot connect vis receiver: " << err.what());
        return;
    }

    LINFO("(p) vis sender was bound");
    while (!isInterrupted()) {

        auto frameNullable = m_visStream->get();

        /**
        * THIS IS THE POINT TO INSERT JPEG COMPRESSION
            Frame newFrame = convert(f);
            ...
            m_connection->send(std::move(*newFrame));
        */

        if (!frameNullable.isNull()) {
            try {
                m_connection->send(std::move(frameNullable.release()));           
 	} catch (const mocca::net::NetworkError& err) {
                LERROR("(p) cannot send vis: " << err.what());
		interrupt();
            }
        }
    }

    // cleanup
    if (m_connection)
        m_connection.reset();
}
