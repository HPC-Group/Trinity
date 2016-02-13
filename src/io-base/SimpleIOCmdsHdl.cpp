#include "SimpleIOCmdsHdl.h"
#include "IOSession.h"
#include "IOSessionManager.h"

using namespace trinity::io;
using namespace trinity::common;

GetLODLevelCountHdl::GetLODLevelCountHdl(GetLODLevelCountCmd cmd) :
m_reply(cmd.getSid(), cmd.getRid()),
m_sid(cmd.getSid())
{}

GetLODLevelCountHdl::~GetLODLevelCountHdl() {}


void GetLODLevelCountHdl::execute() {

    auto& session = SessionManagerSingleton::instance()->getSession(m_sid);
    auto& io = session.getIO();
    m_reply.setLODLevelCount(io.getLODLevelCount());
}


std::unique_ptr<ICommand> GetLODLevelCountHdl::getReturnValue() {
    std::unique_ptr<ReplyGetLODLevelCountCmd> cmd(new ReplyGetLODLevelCountCmd(m_reply));
    return std::move(cmd);
}