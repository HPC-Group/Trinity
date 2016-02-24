#include "IOSessionProxy.h"
#include <thread>

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "common/ProxyUtils.h"

#include "mocca/log/LogManager.h"


using namespace trinity::common;
using namespace trinity::commands;

IOSessionProxy::IOSessionProxy(const int remoteSid, const mocca::net::Endpoint& ioEndpoint)
    : common::IProxy(ioEndpoint)
    , m_remoteSid(remoteSid) {}

int IOSessionProxy::getLODLevelCount() {
    GetLODLevelCountCmd::RequestParams params;
    GetLODLevelCountRequest request(params, m_ridGen.nextID(), m_remoteSid);

    auto reply = sendRequestChecked(m_inputChannel, request);

    return reply->getParams().getLODLevelCount();
}