#include "VisStreamReceiver.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity::frontend;
using namespace trinity::common;

VisStreamReceiver::VisStreamReceiver(const mocca::net::Endpoint endpoint,
                                     std::shared_ptr<VisStream> s) :
m_endpoint(endpoint), m_visStream(s) {
}

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
    
    LINFO("(f) vis receiver listening at \"" + m_endpoint.toString() + "\"");
    
    try {
        
        auto acceptor = mocca::net::ConnectionFactorySelector::bind(m_endpoint);
        while(!m_connection && !isInterrupted()) {
            m_connection = acceptor->accept();
        }
        
        while(!isInterrupted()) {
            m_connection->receive();
        }
        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(f) cannot bind vis receiver: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        auto bytepacket = m_connection->receive();
        if(!bytepacket.isEmpty()) {
            LINFO("(f) vis received");
            while (m_visStream->isSwapping()) {
            }
            m_visStream->insertFrame(bytepacket);
        }
    }
    
    // cleanup
    if(m_connection)
        m_connection.reset();
    
}