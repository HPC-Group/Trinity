#pragma once

#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"

#include <memory>

namespace trinity {
namespace common {

template <typename SessionType> class SessionManager {
public:
    void addSession(std::unique_ptr<SessionType> session) { m_sessions.push_back(std::move(session)); }

    void endSession(int sid) {
        for (auto& session : m_sessions) {
            if (session->getSid() == sid)
                session->interrupt();
        }
        LWARNING("cannot end session with id " + std::to_string(sid) + "(does not exist)");
    }

    SessionType& getSession(int sid) {
        for (auto& session : m_sessions) {
            if (session->getSid() == sid)
                return *session;
        }
        throw TrinityError("no session with id " + std::to_string(sid), __FILE__, __LINE__);
    }

    void endAllSessions() {
        LINFO("sessions to stop: " << m_sessions.size());
        for (auto& session : m_sessions)
            session->interrupt();
    }

private:
    std::vector<std::unique_ptr<SessionType>> m_sessions;
};
}
}
