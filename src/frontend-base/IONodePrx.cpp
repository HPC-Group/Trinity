#include <sstream>
#include <iostream>
#include <thread>

#include "mocca/base/Error.h"

#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"

#include "IONodePrx.h"

#include "common/ErrorCmd.h"
#include "common/NetConfig.h"
#include "common/ISerialObjectFactory.h"
#include "common/IONodeCmds.h"

using namespace trinity::frontend;
using namespace trinity::common;

//static std::chrono::milliseconds receiveTimeout(50);

IONodePrx::~IONodePrx() {}

IONodePrx::IONodePrx(const mocca::net::Endpoint& endpoint) :
m_inputChannel(endpoint) {}


bool IONodePrx::connect() { return m_inputChannel.connect(); }


int IONodePrx::initIO(int fileId) {
    InitIOSessionCmd cmd(0, m_ridGen.nextID(), m_inputChannel.getEndpoint().protocol, 0);
    
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
    return replyCmd.getSid();
}