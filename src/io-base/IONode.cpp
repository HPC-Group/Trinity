#include "io-base/IONode.h"

using namespace trinity;

IONode::IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
    : AbstractNode(std::move(aggregator)) {}

IONode::~IONode() {
    join();
}

std::unique_ptr<ICommandHandler> IONode::createHandler(const Request& request) const {
    return m_factory.createHandler(request);
}