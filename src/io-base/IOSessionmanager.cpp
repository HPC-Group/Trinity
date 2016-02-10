#include "IOSessionManager.h"
#include "mocca/log/LogManager.h"
#include "mocca/base/Error.h"

using namespace trinity::io;

void SessionManager::addSession(std::unique_ptr<IOSession> session) {
    m_sessions.push_back(std::move(session));
}

void SessionManager::endSession(int sid) {
    for(auto& session : m_sessions) {
        if(session->getSid() == sid)
            session->interrupt();
    }
    LWARNING("(io) can't end session with id " + std::to_string(sid) + "(does not exist)");
}

void SessionManager::endAllSessions() {
    LINFO("(io) IOers to stop: " << m_sessions.size());
    for (auto& session : m_sessions)
        session->interrupt();
}

IOSession& SessionManager::getSession(int sid) {
    for(auto& session : m_sessions) {
        if(session->getSid() == sid)
            return *session;
    }
    throw mocca::Error("no session with id " + std::to_string(sid), __FILE__, __LINE__);
    
}