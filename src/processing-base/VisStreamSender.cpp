#include "VisStreamSender.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity::processing;
using namespace trinity::common;

VisStreamSender::VisStreamSender(const mocca::net::Endpoint endpoint,
                                     std::shared_ptr<VisStream> s) :
m_visStream(s), m_endpoint(endpoint) {
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

void VisStreamSender::run() {
    
    
    LINFO("(p) vis sender binding to  \"" + m_endpoint.toString() + "\"");
    
    try {
        
        auto acceptor = mocca::net::ConnectionFactorySelector::bind(m_endpoint);
        while(!m_connection && !isInterrupted()) {
            m_connection = acceptor->accept();
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(p) cannot connect vis receiver: " << err.what());
        return;
    }
    
    LINFO("(p) vis sender was bound");
    while(!isInterrupted()) {
    
        Frame f = m_visStream->get();
        if(!f.isEmpty()) {
            try {
                m_connection->send(std::move(f));
            } catch (const mocca::net::NetworkError& err) {
                LERROR("(p) cannot send vis: " << err.what());
            }
        }
    }
    
    // cleanup
    if(m_connection)
        m_connection.reset();
    
}