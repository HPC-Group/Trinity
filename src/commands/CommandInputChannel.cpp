#include "commands/CommandInputChannel.h"

#include "commands/Request.h"
#include "common/NetConfig.h"

#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

using namespace trinity::commands;


CommandInputChannel::CommandInputChannel(const mocca::net::Endpoint& endpoint)
    : m_endpoint(endpoint) {}

bool CommandInputChannel::connect() {
    try {
        m_mainChannel = mocca::net::ConnectionFactorySelector::connect(m_endpoint);

    } catch (const mocca::net::ConnectFailedError&) {
        LWARNING("(chn) no connection to a node  at \"" << m_endpoint << "\": ");
        return false;
    }

    LINFO("(chn) successfully connected to a node");
    return true;
}

void CommandInputChannel::sendRequest(const Request& request) {
    if (!m_mainChannel)
        throw mocca::Error("(chn) cannot send command: channel not connected", __FILE__, __LINE__);

    auto serialRequest = Request::createByteArray(request);

    // fixme dmc: use streaming operators instead
    //std::string str = serialRequest.read(serialRequest.size());
    //LINFO("(chn) sending command: " << str);

    m_mainChannel->send(std::move(serialRequest));
}

mocca::net::Endpoint CommandInputChannel::getEndpoint() const {
    return m_endpoint;
}

std::unique_ptr<Reply> CommandInputChannel::getReply(const std::chrono::milliseconds& ms) {
    auto serialReply = m_mainChannel->receive(ms);

    if (serialReply.isEmpty()) {
        throw mocca::Error("(chn) no reply arrived", __FILE__, __LINE__);
    }

    // fixme dmc: use streaming operators instead
    //std::string str = serialReply.read(serialReply.size());
    //LINFO("(chn) reply: " << str);

    return Reply::createFromByteArray(serialReply);
}