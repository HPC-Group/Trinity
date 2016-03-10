#include "processing-base/ProcessingNode.h"

using namespace trinity;

ProcessingNode::ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
    : AbstractNode(std::move(aggregator)) {}

ProcessingNode::~ProcessingNode() {
    join();
}

std::unique_ptr<ICommandHandler> ProcessingNode::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}

void ProcessingNode::addSession(std::unique_ptr<RenderSession> session) {
    m_sessions.push_back(std::move(session));
}

std::vector<std::unique_ptr<RenderSession>>& ProcessingNode::getSessions() {
    return m_sessions;
}