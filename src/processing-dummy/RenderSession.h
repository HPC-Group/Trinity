#pragma once
#include <memory>
#include <string>
#include "common/IRenderer.h"



namespace trinity {
    
enum class RenderType { DUMMY, GRIDLEAPER };
    
class RenderSession {
    
public:
    
    RenderSession(const RenderType&);
    ~RenderSession();
    unsigned int getSid() const;
    int getPort() const;
    
    
        
        
private:
    static unsigned int m_nextSid;
    static int m_basePort;
    int m_port;
    unsigned int m_sid;
    std::unique_ptr<IRenderer> m_renderer;
    
    // renderer factory
    static std::unique_ptr<IRenderer> createRenderer(const RenderType&);
        
        
// todo: one day, we might want to release ids
    
    
    
};
}