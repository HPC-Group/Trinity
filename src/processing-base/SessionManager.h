#pragma once
#include <memory>
#include "mocca/base/Singleton.h"
#include "RenderSession.h"

namespace trinity {
namespace processing {
class SessionManager {
    
public:
    void addSession(std::unique_ptr<RenderSession> session);
    void endSession(int sid);
    RenderSession& getSession(int sid);
    void endAllSessions();
    
private:
    std::vector<std::unique_ptr<RenderSession> > m_sessions;
};
    
typedef mocca::Singleton<SessionManager> SessionManagerSingleton;
}
}