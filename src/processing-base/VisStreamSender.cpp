#include "VisStreamSender.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"
#include "mocca/net/NetworkError.h"
#include "mocca/net/ConnectionFactorySelector.h"

using namespace trinity::processing;
using namespace trinity::common;

VisStreamSender::VisStreamSender(const mocca::net::Endpoint endpoint,
                                     std::shared_ptr<VisStream> s) :
m_visStream(s), m_endpoint(endpoint), m_nextFrame(s->getStreamParams().frameSize()) {
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
    
    LINFO("(p) vis sender connecting to \"" + m_endpoint.toString() + "\"");
    
    try {
        
        
        while(!m_connection && !isInterrupted()) {
            m_connection = mocca::net::ConnectionFactorySelector::connect(m_endpoint);
        }

        
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(p) cannot connect vis receiver: " << err.what());
        return;
    }
    
    while(!isInterrupted()) {
        swap(m_nextFrame, m_visStream->readLastFrame());
        m_connection->send(std::move(m_nextFrame));
        m_visStream->swapBuffers();
    }
    
    // cleanup
    if(m_connection)
        m_connection.reset();
    
}