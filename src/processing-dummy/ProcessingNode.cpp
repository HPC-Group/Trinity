#include <iostream>
#include <memory>

#include "ProcessingNode.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/base/StringTools.h"
#include "common/Commands.h"


using namespace trinity;
using namespace mocca::net;


static std::chrono::milliseconds receiveTimeout(50);


ProcessingNode::~ProcessingNode() {
    LINFO("renderers to stop: " << m_renderSessions.size());
    for(auto& session : m_renderSessions)
        session->interrupt();
    join();
}


ProcessingNode::ProcessingNode(const mocca::net::Endpoint endpoint) :
m_endpoint(endpoint),
m_aggregator(std::move(NetworkServiceLocator::bind(endpoint)),
             ConnectionAggregator::DisconnectStrategy::RemoveConnection)
{
}


void ProcessingNode::run() {
    LINFO("listening at endpoint \"" << m_endpoint << "\"");
    
    // listening for incoming requests
    while(!isInterrupted()) {
        
        // receive request
        auto msgEnvelope = m_aggregator.receive(receiveTimeout);
  
        if(!msgEnvelope.isNull()) {  // req arrived
            auto env = msgEnvelope.release();  // release value from nullable
            std::string cmd = env.message.read(env.message.size());
            LINFO("command: " << cmd);  // print cmd
            std::vector<std::string> args = mocca::splitString<std::string>(cmd, '_');
            
            std::stringstream reply;
            
            if (args.size() < vcl::MIN_CMD_LENGTH) {  // cmd not well-formatted
                reply << vcl::TRI_ERROR << "_0_" << "_0_" << "_1";
            } else if (args[0] == vcl::INIT_RENDERER) {
                reply << handleSpawnRendererCmd(args);
            } else if (args[0] == vcl::INIT_CONNECTION) {
                LINFO("someone connected");  // do nothing
                continue;
            } else {
                reply << vcl::TRI_ERROR << "_0_" << args[2] << "_2";  // unknown cmd
            }
            LINFO("reply: " << reply.str());  // print reply
            m_aggregator.send(MessageEnvelope(std::move(mocca::ByteArray()<<reply.str()),
                                              env.senderID));
        }
    }
}

std::string ProcessingNode::handleSpawnRendererCmd(std::vector<std::string>& args) {
    
    RenderType t;
    if(args[3] == vcl::DUMMY_RENDERER) {
        t = RenderType::DUMMY;
    } else {
        return vcl::TRI_ERROR + "_0_" + args[2] + "_3";
    }
    
    std::unique_ptr<RenderSession> session(new RenderSession(t));
    std::string ret = vcl::TRI_RET + "_0_" + args[2] + "_" +
    std::to_string(session->getSid()) + "_" + std::to_string(session->getPort());
    
    // endpoint for the render session
    std::unique_ptr<mocca::net::Endpoint> ep(
                            new mocca::net::Endpoint(m_endpoint.protocol(),
                            m_endpoint.transport(),
                            std::to_string(session->getPort())));
    session->provideOwnEndpoint(std::move(ep));
    session->start();
    m_renderSessions.push_back(std::move(session));
    
    return ret;
}


