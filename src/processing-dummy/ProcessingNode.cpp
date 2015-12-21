#include <iostream>
#include <memory>
#include <string>

#include "ProcessingNode.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/StringTools.h"
#include "mocca/base/ContainerTools.h"
#include "common/Commands.h"

using namespace trinity;
using namespace mocca::net;

static std::chrono::milliseconds receiveTimeout(50);

ProcessingNode::~ProcessingNode() {
    LINFO("(p) renderers to stop: " << m_renderSessions.size());
    for (auto& session : m_renderSessions)
        session->interrupt();
    join();
}

ProcessingNode::ProcessingNode(const mocca::net::Endpoint endpoint)
    : m_endpoint(endpoint)
    , m_aggregator(mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(
                       ConnectionFactorySelector::bind(endpoint)),
                   ConnectionAggregator::DisconnectStrategy::RemoveConnection) {}

void ProcessingNode::run() {
    LINFO("(p) listening at endpoint \"" << m_endpoint << "\"");

    // listening for incoming requests
    while (!isInterrupted()) {

        // receive request
        auto msgEnvelope = m_aggregator.receive(receiveTimeout);

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
                    reply = handleSpawnRendererCmd(args);
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
            m_aggregator.send(
            MessageEnvelope(std::move(mocca::ByteArray() << reply), env.senderID));
        }
    }
}

std::string ProcessingNode::handleSpawnRendererCmd(std::vector<std::string>& args) {

    VclType t;
    
    try {
        t = m_vcl.toType(args[3]);
    } catch (const mocca::Error& err) {
        return m_vcl.assembleError(0, stoi(args[2]), 3);
    }


    std::unique_ptr<RenderSession> session(new RenderSession(t));
    std::string reply = m_vcl.assembleRetSpawnRenderer(0, stoi(args[2]),
                                                       session->getSid(),
                                                       session->getPort());



    // endpoint for the render session
    std::unique_ptr<mocca::net::Endpoint> ep(
        new mocca::net::Endpoint(m_endpoint.protocol(),
                                 std::to_string(session->getPort())));
    
    session->provideOwnEndpoint(std::move(ep));
    session->start();
    m_renderSessions.push_back(std::move(session));

    return reply;
}
