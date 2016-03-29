#include "VisStreamSender.h"
#include "mocca/base/Error.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "jpeg/JPEGEncoder.h"

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
    JPEGEncoder jpeg(75, JPEGEncoder::Subsample_420);
    
    while (!isInterrupted()) {
        if(!m_connection->isConnected()) {
            interrupt();
            continue;
        }

        auto frameNullable = m_visStream->get();

        if (m_connection->isConnected() && !frameNullable.isNull()) {
            try {
                auto const& params = m_visStream->getStreamingParams();
                frameNullable = jpeg.encode(frameNullable, params.getResX(), params.getResY());
                
				if (!frameNullable.isNull()) {
					m_connection->send(std::move(frameNullable.release()));
					//LINFO("(p) frame out");
				}
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
