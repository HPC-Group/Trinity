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
            std::vector<std::string> args = mocca::splitString<std::string>(cmd, '_');

            std::string reply;
            
            
            if (!m_vcl.isSoundCommand(args)) {
                reply = m_vcl.assembleError(0, 0, 1);
            }
            
            VclType t = m_vcl.toType(args[0]);
            
            switch(t) {
                case VclType::InitRenderer: {
                    reply = handleInitRendererCmd(args);
                    break;
                }
                    
                case VclType::InitConnection: {
                    LINFO("(p) client connected");
                    continue;
                }
                    
                default:
                    reply = m_vcl.assembleError(0, 0, 2);
            }

            
            
            LINFO("(p) reply: " << reply);
            m_aggregator->send(
            MessageEnvelope(std::move(mocca::ByteArray() << reply), env.senderID));
        }
    }
}

std::string ProcessingNode::handleInitRendererCmd(std::vector<std::string>& args) {

    VclType t;
    
    try {
        t = m_vcl.toType(args[4]);
    } catch (const mocca::Error& err) {
        return m_vcl.assembleError(0, stoi(args[4]), 3);
    }

    StreamParams p(args[5]);
    std::unique_ptr<RenderSession> session(new RenderSession(t, std::move(p), args[3]));
    
    
    std::string reply = m_vcl.assembleRetInitRenderer(0, stoi(args[2]),
                                                       session->getSid(),
                                                       session->getControlPort(),
                                                       session->getVisPort());

    session->start();
    m_renderSessions.push_back(std::move(session));

    return reply;
}
