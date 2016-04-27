#include "IONodeProxy.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity;

IONodeProxy::IONodeProxy(const mocca::net::Endpoint& ep)
    : m_inputChannel(ep, CompressionMode::Uncompressed) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to IO node", __FILE__, __LINE__);
    }
}

std::unique_ptr<IOSessionProxy> IONodeProxy::initIO(const std::string& fileId, bool useLoopback) {
    std::string protocol = useLoopback ? trinity::ioLoopbackEndpoint().protocol : m_inputChannel.getEndpoint().protocol;
    std::string machine = useLoopback ? trinity::ioLoopbackEndpoint().machine : m_inputChannel.getEndpoint().machine;
    auto compressionMode = useLoopback ? CompressionMode::Uncompressed : CompressionMode::Compressed;

    InitIOSessionCmd::RequestParams params(protocol, fileId);
    InitIOSessionRequest request(params, IDGenerator::nextID(), 0);

    auto reply = sendRequestChecked(m_inputChannel, request);

    // BIG TODO HERE!!!
    mocca::net::Endpoint controlEndpoint(protocol, machine, reply->getParams().getControlPort());   

    return mocca::make_unique<IOSessionProxy>(reply->getSid(), controlEndpoint, compressionMode);
}

std::vector<IOData> IONodeProxy::listFiles(const std::string& dirID) const {
    ListFilesCmd::RequestParams params(dirID);
    ListFilesRequest request(params, IDGenerator::nextID(), 0);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getIOData();
}

std::vector<std::string> IONodeProxy::getRoots() const {
    GetRootsCmd::RequestParams params;
    GetRootsRequest request(params, IDGenerator::nextID(), 0);
    auto reply = sendRequestChecked(m_inputChannel, request);
    return reply->getParams().getResult();
}
