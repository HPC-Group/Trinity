#include "IONodeProxy.h"

#include "commands/IOCommands.h"
#include "commands/ErrorCommands.h"

using namespace trinity::common;
using namespace trinity::commands;


std::unique_ptr<IOSessionProxy> IONodeProxy::initIO(int fileId) {

    InitIOSessionCmd cmd(0, m_ridGen.nextID(), m_inputChannel.getEndpoint().protocol, fileId);
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
    ReplyInitIOSessionCmd replyCmd(*serialReply);
    // todo: factory, too?
    

    
    // BIG TODO HERE!!!
    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint());
    controlEndpoint.port = std::to_string(replyCmd.getControlPort());

    return std::unique_ptr<IOSessionProxy>(new IOSessionProxy(replyCmd.getSid(), controlEndpoint));
}