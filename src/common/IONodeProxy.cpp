#include "IONodeProxy.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity::common;
using namespace trinity::commands;

IONodeProxy::IONodeProxy(const mocca::net::Endpoint& ep)
    : m_inputChannel(ep) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to IO node", __FILE__, __LINE__);
    }
}

std::unique_ptr<IOSessionProxy> IONodeProxy::initIO(int fileId) {
    InitIOSessionCmd::RequestParams params(m_inputChannel.getEndpoint().protocol, fileId);
    InitIOSessionRequest request(params, IDGenerator::nextID(), 0);

    auto reply = sendRequestChecked(m_inputChannel, request);

    // BIG TODO HERE!!!
    mocca::net::Endpoint controlEndpoint(m_inputChannel.getEndpoint());
    controlEndpoint.port = std::to_string(reply->getParams().getControlPort());

    return mocca::make_unique<IOSessionProxy>(reply->getSid(), controlEndpoint);
}