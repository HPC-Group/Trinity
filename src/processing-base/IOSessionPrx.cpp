#include <thread>
#include "IOSessionPrx.h"
#include "common/SimpleIOCmds.h"
#include "common/ErrorCmd.h"
#include "mocca/log/LogManager.h"

using namespace trinity::processing;
using namespace trinity::common;

IOSessionPrx::IOSessionPrx(const int sid, const mocca::net::Endpoint& ioEndpoint) :
m_controlEndpoint(ioEndpoint),
m_sid(sid),
m_inputChannel(m_controlEndpoint)
{
}

IOSessionPrx::~IOSessionPrx() {
    
}
bool IOSessionPrx::connect() {
    LINFO("(p) connecing to remote io at " << m_controlEndpoint);
    return m_inputChannel.connect();
}

int IOSessionPrx::getLODLevelCount() {
    
    GetLODLevelCountCmd cmd(m_sid, m_ridGen.nextID());
    
    // send cmd, receive reply
    m_inputChannel.sendCommand(cmd);
    auto serialReply = m_inputChannel.getReply();
    
    // reply could be return or error
    VclType resultType = serialReply->getType();
    
    if(resultType == VclType::TrinityError) {  // error arrived
        ErrorCmd error(*serialReply);
        throw mocca::Error("init io session: error was returned: " + error.printError(), __FILE__, __LINE__);
    }
    if(resultType != VclType::TrinityReturn)  // sth strange arrived
        throw mocca::Error("init io session: result not of type RET or ERR", __FILE__, __LINE__);
    
    // return was ok
    ReplyGetLODLevelCountCmd replyCmd(*serialReply);
    
    return replyCmd.getLODLevelCount();
}