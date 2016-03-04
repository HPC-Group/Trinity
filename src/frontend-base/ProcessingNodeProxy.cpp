#include "frontend-base/ProcessingNodeProxy.h"

#include "commands/ProcessingCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity::frontend;
using namespace trinity::common;
using namespace trinity::commands;

ProcessingNodeProxy::ProcessingNodeProxy(const mocca::net::Endpoint& ep)
    : m_inputChannel(ep) {
    LINFO("(f) creating processing node proxy for " + ep.toString());
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to processing node", __FILE__, __LINE__);
    }
}

std::unique_ptr<RendererProxy> ProcessingNodeProxy::initRenderer(const VclType& type, int fileId, const mocca::net::Endpoint& endpoint,
                                                                 const StreamingParams& streamingParams) {
    // creating the cmd that will initialize a remote renderer of the given type
    InitProcessingSessionCmd::RequestParams params(m_inputChannel.getEndpoint().protocol, type, fileId, endpoint.toString(),
                                                   streamingParams);
    InitProcessingSessionRequest request(params, IDGenerator::nextID(), 0);

    auto reply = sendRequestChecked(m_inputChannel, request);
    auto replyParams = reply->getParams();


    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint().protocol, "localhost", std::to_string(replyParams.getControlPort()));
    mocca::net::Endpoint visEndpoint(m_inputChannel.getEndpoint().protocol, "localhost", std::to_string(replyParams.getVisPort()));

    std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(streamingParams);
    LINFO("(f) creating render proxy for " + controlEndpoint.toString());

    return mocca::make_unique<RendererProxy>(stream, std::move(controlEndpoint), std::move(visEndpoint), reply->getSid());
}