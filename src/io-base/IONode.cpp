#include <iostream>
#include <memory>
#include <string>

#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"
#include "mocca/base/ContainerTools.h"

#include "common/Commands.h"
#include "common/NetConfig.h"
#include "common/ISerialObjectFactory.h"

#include "IONode.h"

using namespace trinity::io;
using namespace trinity::common;
using namespace mocca::net;


IONode::~IONode() {
    LINFO("(io) io sessions to stop: " << m_ioSessions.size());
    for (auto& session : m_ioSessions)
        session->interrupt();
    join();
}

IONode::IONode(std::unique_ptr<ConnectionAggregator> aggregator) :
m_aggregator(std::move(aggregator)) {}

void IONode::run() {
    LINFO("(io) listening...");
    
    // listening for incoming requests
    while (!isInterrupted()) {

        // receive request
        auto msgEnvelope = m_aggregator->receive(trinity::common::TIMEOUT_REPLY);

        if (!msgEnvelope.isNull()) {          // req arrived
            auto env = msgEnvelope.release(); // release value from nullable
            std::string cmd = env.message.read(env.message.size());
            LINFO("(io) command: " << cmd); // print cmd
            
            std::stringstream requestStream, replyStream;
            requestStream << cmd;
            
            auto handler = m_factory.createHandler(requestStream);
            handler->execute();
            auto reply = handler->getReturnValue();
            if(reply != nullptr) {
                auto serialReply = ISerialObjectFactory::create();
                reply->serialize(*serialReply);
                serialReply->writeTo(replyStream);
                LINFO("(io) reply: " << replyStream.str());
                m_aggregator->send(MessageEnvelope(std::move(mocca::ByteArray()
                                                             << replyStream.str()),
                                                                env.connectionID));
            }
        }
    }
}