#include "IOSessionProxy.h"
#include <thread>

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "common/ProxyUtils.h"
#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"


using namespace trinity::common;
using namespace trinity::commands;

IOSessionProxy::IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint)
    : m_inputChannel(ioEndpoint)
    , m_remoteSid(remoteSid) {
    if (!connectInputChannel(m_inputChannel)) {
        throw TrinityError("Error connecting to IO session", __FILE__, __LINE__);
    }
}

int IOSessionProxy::getLODLevelCount() {
    GetLODLevelCountCmd::RequestParams params;
    GetLODLevelCountRequest request(params, IDGenerator::nextID(), m_remoteSid);

    auto reply = sendRequestChecked(m_inputChannel, request);

    return reply->getParams().getLODLevelCount();
}