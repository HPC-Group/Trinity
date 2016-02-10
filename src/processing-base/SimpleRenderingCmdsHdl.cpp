#include "SimpleRenderingCmdsHdl.h"
#include "RenderSession.h"
#include "SessionManager.h"

using namespace trinity::processing;
using namespace trinity::common;

SetIsoValueHdl::SetIsoValueHdl(SetIsoValueCmd cmd) :
m_isoValue(cmd.getIsoValue()),
m_sid(cmd.getSid())
{}

SetIsoValueHdl::~SetIsoValueHdl() {}


void SetIsoValueHdl::execute() {

    auto& session = SessionManagerSingleton::instance()->getSession(m_sid);
    auto& renderer = session.getRenderer();
    renderer.setIsoValue(m_isoValue);
    
}


