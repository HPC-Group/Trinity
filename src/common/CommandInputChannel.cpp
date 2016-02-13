#include "CommandInputChannel.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/NetworkError.h"
#include "mocca/base/ByteArray.h"
#include "mocca/log/ConsoleLog.h"

#include "common/NetConfig.h"
#include "common/ISerialObjectFactory.h"

using namespace trinity::common;

CommandInputChannel::~CommandInputChannel() {}

CommandInputChannel::CommandInputChannel(const mocca::net::Endpoint& endpoint) :
m_endpoint(endpoint) {}


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


void CommandInputChannel::sendCommand(const ICommand& cmd) {
    
    auto serialRequest = ISerialObjectFactory::create();
    std::stringstream requestStream;
    cmd.serialize(*serialRequest);
    serialRequest->writeTo(requestStream);
    
    // command is sent to processing node
    m_mainChannel->send(std::move(mocca::ByteArray()<< requestStream.str()));
    
}

const mocca::net::Endpoint& CommandInputChannel::getEndpoint() const {
    return m_endpoint;
}

std::unique_ptr<ISerialObject> CommandInputChannel::getReply(const std::chrono::milliseconds& ms) {
    
    auto byteArray = m_mainChannel->receive(ms);
    
    if(byteArray.isEmpty()) {
        throw mocca::Error("no reply arrived", __FILE__, __LINE__);
    }
    
    // the reply is of type ReplyInitRendererCmd
    std::string rep = byteArray.read(byteArray.size());
    LINFO("(chn) reply: " << rep);
    
    std::stringstream replyStream;
    replyStream << rep;
    
    auto serialReply = ISerialObjectFactory::create();
    serialReply->readFrom(replyStream);
    return std::move(serialReply);
}