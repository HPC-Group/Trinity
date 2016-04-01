#include "commands/CommandInputChannel.h"

#include "commands/Request.h"
#include "common/NetConfig.h"
#include "common/TrinityError.h"

#include "mocca/base/ByteArray.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

CommandInputChannel::CommandInputChannel(const mocca::net::Endpoint& endpoint)
    : m_endpoint(endpoint) {}

bool CommandInputChannel::connect() const {
    try {
        m_mainChannel = mocca::net::ConnectionFactorySelector::connect(m_endpoint);
    } catch (const mocca::net::ConnectFailedError&) {
        LWARNING("(chn) no connection to a node  at \"" << m_endpoint << "\": ");
        return false;
    }

    LINFO("(chn) successfully connected to a node");
    return true;
}

void CommandInputChannel::sendRequest(const Request& request) const {
    if (!m_mainChannel)
        throw TrinityError("(chn) cannot send command: channel not connected", __FILE__, __LINE__);

    static int count = 0;
    LINFO("Request " << count++ << " (" << Vcl::instance().toString(request.getType()) << ")");

    auto serialRequest = Request::createByteArray(request);
    
    try {
        m_mainChannel->send(std::move(serialRequest));
    } catch (const mocca::net::NetworkError& err) {
        LERROR("(chn) cannot send request: " << err.what());
    }
}

mocca::net::Endpoint CommandInputChannel::getEndpoint() const {
    return m_endpoint;
}

std::unique_ptr<Reply> CommandInputChannel::getReply(const std::chrono::milliseconds& ms) const {
    auto serialReply = m_mainChannel->receive(ms);

    if (serialReply.isEmpty()) {
        throw TrinityError("(chn) no reply arrived", __FILE__, __LINE__);
    }

    auto reply = Reply::createFromByteArray(serialReply);
    return reply;
}