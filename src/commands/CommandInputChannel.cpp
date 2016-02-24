#include "commands/CommandInputChannel.h"

#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"

#include "commands/ISerialObjectFactory.h"
#include "commands/Request.h"
#include "common/NetConfig.h"

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

    auto serialRequest = Request::createSerialObject(request);
    std::stringstream requestStream;
    serialRequest->writeTo(requestStream);

    // command is sent to processing node
    LINFO("(chn) sending command: " << requestStream.str());
    m_mainChannel->send(std::move(mocca::ByteArray() << requestStream.str()));
}

mocca::net::Endpoint CommandInputChannel::getEndpoint() const {
    return m_endpoint;
}

std::unique_ptr<Reply> CommandInputChannel::getReply(const std::chrono::milliseconds& ms) {
    auto byteArray = m_mainChannel->receive(ms);

    if (byteArray.isEmpty()) {
        throw mocca::Error("(chn) no reply arrived", __FILE__, __LINE__);
    }

    // the reply is of type ReplyInitRendererCmd
    std::string rep = byteArray.read(byteArray.size());
    LINFO("(chn) reply: " << rep);

    std::stringstream replyStream;
    replyStream << rep;

    auto serialReply = ISerialObjectFactory::create();
    serialReply->readFrom(replyStream);
    return Reply::createFromSerialObject(*serialReply);
}