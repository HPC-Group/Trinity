#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>

#include "ProcessingNode.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkServiceLocator.h"

using namespace trinity;
using namespace mocca::net;

static std::chrono::milliseconds receiveTimeout(50);

// exit handler for interrupts
std::atomic<bool> exitFlag { false };
void exitHandler(int s) {
    LINFO("trinity exit on signal " << std::to_string(s));
    exitFlag = true;
}


ProcessingNode::ProcessingNode(const mocca::net::Endpoint& endpoint) :
m_endpoint(endpoint),
m_aggregator(std::move(NetworkServiceLocator::bind(endpoint)),
             ConnectionAggregator::DisconnectStrategy::RemoveConnection)
{
    signal(SIGINT, exitHandler);
}


void ProcessingNode::listen() {
    LINFO("listening at endpoint \"" << m_endpoint << "\"");
    
    // listening for incoming connections
    while(!exitFlag) {
        auto msgEnvelope = std::move(m_aggregator.receive(receiveTimeout));
        if(!msgEnvelope.isNull()) {
            auto byteArray = msgEnvelope.release().message;
            LINFO("command: " << byteArray.read(byteArray.size()));
        }
    }
}