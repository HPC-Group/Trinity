#include <sstream>
#include <iostream>
#include <thread>

#include "mocca/base/Error.h"

#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/base/StringTools.h"

#include "ProcessingNodePrx.h"

#include "common/ErrorCmd.h"
#include "common/NetConfig.h"
#include "common/ISerialObjectFactory.h"

using namespace trinity::frontend;
using namespace trinity::common;

//static std::chrono::milliseconds receiveTimeout(50);

ProcessingNodePrx::~ProcessingNodePrx() {}

ProcessingNodePrx::ProcessingNodePrx(const mocca::net::Endpoint& endpoint) :
m_inputChannel(endpoint) {}


bool ProcessingNodePrx::connect() { return m_inputChannel.connect(); }


std::unique_ptr<RendererPrx> ProcessingNodePrx::initRenderer(const VclType& type,
                                                             const StreamingParams& params) {
    
    // creating the cmd that will initialize a remote renderer of the given type
    InitRendererCmd cmd(0, m_ridGen.nextID(), m_inputChannel.getEndpoint().protocol, type, params);
    
    // send cmd, receive reply
    m_inputChannel.sendCommand(cmd);
    auto serialReply = m_inputChannel.getReply();
    
    // reply could be return or error
    VclType resultType = serialReply->getType();
    
    if(resultType == VclType::TrinityError) {  // error arrived
        ErrorCmd error(*serialReply);
        throw mocca::Error("init renderer: error was returned: " + error.printError(), __FILE__, __LINE__);
    }
    if(resultType != VclType::TrinityReturn)  // sth strange arrived
        throw mocca::Error("init renderer: result not of type RET or ERR", __FILE__, __LINE__);
    
    // return was ok
    ReplyInitRendererCmd replyCmd(*serialReply);
    return handleInitRendererReply(replyCmd, params);

}

std::unique_ptr<RendererPrx>
ProcessingNodePrx::handleInitRendererReply(const ReplyInitRendererCmd& replyCmd,
                                           const StreamingParams& params) {
    
    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint().protocol,
                                         "localhost",
                                         std::to_string(replyCmd.getControlPort()));
    
    
    mocca::net::Endpoint visEndpoint(m_inputChannel.getEndpoint().protocol,
                                     "localhost",
                                     std::to_string(replyCmd.getVisPort()));
    
    
    std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(params);
    LINFO("(f) creating render proxy for " << controlEndpoint);
    
    std::unique_ptr<RendererPrx> ren(new RendererPrx(stream,
                                                     std::move(controlEndpoint),
                                                     std::move(visEndpoint),
                                                     replyCmd.getSid()));
    return std::move(ren);
}