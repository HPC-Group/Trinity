#include "SessionManager.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"

using namespace trinity::processing;

void SessionManager::startSession(std::unique_ptr<RenderSession> session) {
    session->start();
    m_sessions.push_back(std::move(session));
}

void SessionManager::endSession(int sid) {
    for(auto& session : m_sessions) {
        if(session->getSid() == sid)
            session->interrupt();
    }
    LWARNING("(p) can't end session with id " + std::to_string(sid) + "(does not exist)");
}

void SessionManager::endAllSessions() {
    LINFO("(p) renderers to stop: " << m_sessions.size());
    for (auto& session : m_sessions)
        session->interrupt();
}

RenderSession& SessionManager::getSession(int sid) {
    for(auto& session : m_sessions) {
        if(session->getSid() == sid)
            return *session;
    }
    throw mocca::Error("no session with id " + std::to_string(sid), __FILE__, __LINE__);
    
}