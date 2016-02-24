#include "frontend-base/ProcessingNodeProxy.h"

#include "common/ProxyUtils.h"
#include "commands/ProcessingCommands.h"

#include "mocca/log/LogManager.h"
#include "mocca/base/Memory.h"

using namespace trinity::frontend;
using namespace trinity::common;
using namespace trinity::commands;


std::unique_ptr<RendererProxy> ProcessingNodeProxy::initRenderer(const VclType& type, int fileId, const mocca::net::Endpoint& endpoint,
                                                                 const StreamingParams& streamingParams) {
    // creating the cmd that will initialize a remote renderer of the given type
    InitProcessingSessionCmd::RequestParams params(m_inputChannel.getEndpoint().protocol, type, fileId, endpoint.toString(), streamingParams);
    InitProcessingSessionRequest request(params, m_ridGen.nextID(), 0);

    auto reply = sendRequestChecked(m_inputChannel, request);
    auto replyParams = reply->getParams();

    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint().protocol, "localhost", std::to_string(replyParams.getControlPort()));
    mocca::net::Endpoint visEndpoint(m_inputChannel.getEndpoint().protocol, "localhost", std::to_string(replyParams.getVisPort()));

    std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(streamingParams);
    LINFO("(f) creating render proxy for " + controlEndpoint.toString());

    return mocca::make_unique<RendererProxy>(stream, std::move(controlEndpoint), std::move(visEndpoint), reply->getSid());
}