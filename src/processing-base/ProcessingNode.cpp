#include "processing-base/ProcessingNode.h"

using namespace trinity;

ProcessingNode::ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
    : AbstractNode(std::move(aggregator)) {}

ProcessingNode::~ProcessingNode() {
    join();
}

std::unique_ptr<ICommandHandler> ProcessingNode::createHandler(const Request& request) const {
    return m_factory.createHandler(request);
}