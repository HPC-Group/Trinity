#include <thread>
#include "IOSessionProxy.h"

#include "commands/IOCommands.h"
#include "commands/ErrorCommands.h"
#include "mocca/log/LogManager.h"

using namespace trinity::common;
using namespace trinity::commands;

IOSessionProxy::IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint) :
common::IProxy(ioEndpoint),
m_remoteSid(remoteSid)
{
}


int IOSessionProxy::getLODLevelCount() {
    
    GetLODLevelCountCmd cmd(m_remoteSid, m_ridGen.nextID());
    
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