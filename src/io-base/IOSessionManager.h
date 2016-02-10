#pragma once
#include <memory>
#include "mocca/base/Singleton.h"
#include "IOSession.h"

namespace trinity {
namespace io {
class SessionManager {
    
public:
    void addSession(std::unique_ptr<IOSession> session);
    void endSession(int sid);
    IOSession& getSession(int sid);
    void endAllSessions();
    
private:
    std::vector<std::unique_ptr<IOSession> > m_sessions;
};
    
typedef mocca::Singleton<SessionManager> SessionManagerSingleton;
}
}