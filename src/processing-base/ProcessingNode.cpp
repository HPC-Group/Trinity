#include <iostream>
#include <memory>
#include <string>

#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"
#include "mocca/base/ContainerTools.h"

#include "ProcessingNode.h"

#include "common/Commands.h"
#include "common/NetConfig.h"

using namespace trinity::processing;
using namespace trinity::common;
using namespace mocca::net;


ProcessingNode::~ProcessingNode() {
    LINFO("(p) renderers to stop: " << m_renderSessions.size());
    for (auto& session : m_renderSessions)
        session->interrupt();
    join();
}

ProcessingNode::ProcessingNode(std::unique_ptr<ConnectionAggregator> aggregator) :
m_aggregator(std::move(aggregator)) {}

void ProcessingNode::run() {
    LINFO("(p) listening...");


    
    // listening for incoming requests
    while (!isInterrupted()) {

        // receive request
        auto msgEnvelope = m_aggregator->receive(trinity::common::TIMEOUT_REPLY);

        if (!msgEnvelope.isNull()) {          // req arrived
            auto env = msgEnvelope.release(); // release value from nullable
            std::string cmd = env.message.read(env.message.size());
            LINFO("(p) command: " << cmd); // print cmd
            
            std::stringstream s1, s2;
            s1 << cmd;
            
            auto handler = m_factory.createHandler(s1);
            handler->execute();
            auto reply = handler->getReturnValue();
            if(reply != nullptr) {
                reply->serialize(s2);
                LINFO("(p) reply: " << s2.str());
                m_aggregator->send(MessageEnvelope(std::move(mocca::ByteArray() << s2.str()),
                                                   env.connectionID));
            }
        }
    }
}