#include "common/Node.h"

#include "commands/ICommandFactory.h"
#include "commands/Vcl.h"
#include "common/NetConfig.h"

#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"

#include <iostream>
#include <memory>
#include <string>

using namespace trinity::common;
using namespace trinity::commands;
using namespace mocca::net;


Node::~Node() {
    LINFO("(node) sessions to stop: " << m_sessions.size());
    for (auto& session : m_sessions)
        session->interrupt();
    join();
}

Node::Node(std::unique_ptr<ConnectionAggregator> aggregator, std::unique_ptr<ICommandFactory> factory)
    : m_factory(std::move(factory))
    , m_aggregator(std::move(aggregator)) {}

// dmc: question: does a node (io or processing) ever receive a command other than InitIO or
// InitProcessing? all other commands seem to presuppose a session, so maybe a node that can accept
// all kinds of commands is overgeneralized? or maybe there are two distinct types of commands:
// node-commands and session-commands?
void Node::run() {
    LINFO("(node) listening... ");

    // listening for incoming requests
    while (!isInterrupted()) {

        // receive request
        auto msgEnvelope = m_aggregator->receive(trinity::common::TIMEOUT_REPLY);

        if (!msgEnvelope.isNull()) {          // req arrived
            auto env = msgEnvelope.release(); // release value from nullable
            std::string cmd = env.message.read(env.message.size());
            LINFO("(io) command: " << cmd); // print cmd

            auto request = Request::createFromByteArray(env.message);

            auto handler = m_factory->createHandler(*request);
            auto reply = handler->execute();
            if (reply != nullptr) {
                auto serialReply = Reply::createByteArray(*reply);
                //LINFO("(io) reply: " << replyStream.str()); fixme dmc
                m_aggregator->send(MessageEnvelope(std::move(serialReply), env.connectionID));
            }
        }
    }
}