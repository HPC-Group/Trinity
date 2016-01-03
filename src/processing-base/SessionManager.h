#pragma once
#include <memory>
#include "mocca/base/Singleton.h"
#include "RenderSession.h"

namespace trinity {
namespace processing {
class SessionManager {
    
public:
    void startSession(std::unique_ptr<RenderSession> session):
    void endSession(int sid);
    void endAllSessions();
    
private:
    std::vector<std::unique_ptr<RenderSession> m_sessions;
};
    
typedef Singleton<SessionManager> SessionManagerSingleton;
}
}