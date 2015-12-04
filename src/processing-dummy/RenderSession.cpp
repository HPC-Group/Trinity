#include "RenderSession.h"
#include "mocca/base/Error.h"
#include "DummyRenderer.h"

using namespace trinity;

int RenderSession::m_basePort = 5990; // config base port for renderer here
unsigned int RenderSession::m_nextSid = 1; // sid 0 is considered invalid / dummy

RenderSession::RenderSession(const RenderType& type) {
    m_sid = m_nextSid++;
    m_port = m_basePort++;
    m_renderer = createRenderer(type);
}

RenderSession::~RenderSession() {
    
}


std::unique_ptr<IRenderer> RenderSession::createRenderer(const RenderType& t) {
    switch (t) {
        case RenderType::DUMMY:
            return std::unique_ptr<IRenderer>(new DummyRenderer());
            break;
            
        case RenderType::GRIDLEAPER:
            throw mocca::Error("grid leaper not supported yet", __FILE__, __LINE__);
            break;
            
        default:
            throw mocca::Error("unable to create renderer: type not found", __FILE__, __LINE__);
            break;
    }
}


unsigned int RenderSession::getSid() const {
    return m_sid;
}

int RenderSession::getPort() const {
    return m_port;
}