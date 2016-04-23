#include "frontend-base/ProcessingNodeProxy.h"

#include "commands/ProcessingCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity;

ProcessingNodeProxy::ProcessingNodeProxy(const mocca::net::Endpoint& ep)
    : m_inputChannel(ep, CompressionMode::Uncompressed) {
    LINFO("(f) creating processing node proxy for " + ep.toString());
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to processing node", __FILE__, __LINE__);
    }
}

std::unique_ptr<RendererProxy> ProcessingNodeProxy::initRenderer(const VclType& type, const std::string& fileId,
                                                                 const mocca::net::Endpoint& ioEndpoint,
                                                                 const StreamingParams& streamingParams) {
    // creating the cmd that will initialize a remote renderer of the given type
    InitProcessingSessionCmd::RequestParams params(m_inputChannel.getEndpoint().protocol, type, fileId, ioEndpoint, streamingParams);
    InitProcessingSessionRequest request(params, IDGenerator::nextID(), 0);

    auto reply = sendRequestChecked(m_inputChannel, request);
    auto replyParams = reply->getParams();

    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint().protocol, m_inputChannel.getEndpoint().machine,
                                         replyParams.getControlPort());
    mocca::net::Endpoint visEndpoint(m_inputChannel.getEndpoint().protocol, m_inputChannel.getEndpoint().machine, replyParams.getVisPort());

    std::shared_ptr<VisStream> stream = std::make_shared<VisStream>(streamingParams);
    LINFO("(f) creating render proxy for " + controlEndpoint.machine);

    return mocca::make_unique<RendererProxy>(stream, std::move(controlEndpoint), std::move(visEndpoint), reply->getSid());
}