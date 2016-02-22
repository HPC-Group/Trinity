#include <iostream>
#include <sstream>
#include <thread>

#include "mocca/log/LogManager.h"

#include "ProcessingNodeProxy.h"

#include "commands/ErrorCommands.h"
#include "commands/ProcessingCommands.h"

using namespace trinity::frontend;
using namespace trinity::common;
using namespace trinity::commands;


std::unique_ptr<RendererProxy> ProcessingNodeProxy::initRenderer(const VclType& type, int fileId, const mocca::net::Endpoint& endpoint,
                                                                 const StreamingParams& params) {

    // creating the cmd that will initialize a remote renderer of the given type
    InitProcessingSessionCmd cmd(0, m_ridGen.nextID(), m_inputChannel.getEndpoint().protocol, type, fileId, endpoint.toString(), params);

    // send cmd, receive reply
    m_inputChannel.sendCommand(cmd);
    auto serialReply = m_inputChannel.getReply();

    // reply could be return or error
    VclType resultType = serialReply->getType();

    if (resultType == VclType::TrinityError) { // error arrived
        ErrorCmd error(*serialReply);
        throw mocca::Error("init renderer: error was returned: " + error.printError(), __FILE__, __LINE__);
    }
    if (resultType != VclType::TrinityReturn) // sth strange arrived
        throw mocca::Error("init renderer: result not of type RET or ERR", __FILE__, __LINE__);

    // return was ok
    ReplyInitProcessingSessionCmd replyCmd(*serialReply);
    return handleInitRendererReply(replyCmd, params);
}

std::unique_ptr<RendererProxy> ProcessingNodeProxy::handleInitRendererReply(const ReplyInitProcessingSessionCmd& replyCmd,
                                                                            const StreamingParams& params) {

    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint().protocol, "localhost", std::to_string(replyCmd.getControlPort()));


    mocca::net::Endpoint visEndpoint(m_inputChannel.getEndpoint().protocol, "localhost", std::to_string(replyCmd.getVisPort()));


    std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(params);
    LINFO("(f) creating render proxy for " + controlEndpoint.toString());

    std::unique_ptr<RendererProxy> ren(new RendererProxy(stream, std::move(controlEndpoint), std::move(visEndpoint), replyCmd.getSid()));
    return std::move(ren);
}