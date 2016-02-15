#include "gtest/gtest.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/base/ContainerTools.h"
#include "mocca/net/Endpoint.h"


using namespace mocca::net;

class NodeTest : public ::testing::Test {
protected:
    
    NodeTest() {
        ConnectionFactorySelector::addDefaultFactories();
    }

    virtual ~NodeTest() {
        ConnectionFactorySelector::removeAll();
    }
};

/*
TEST_F(NodeTest, StartNodeTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::processing::ProcessingNode node(std::move(aggregator));
    
    ASSERT_NO_THROW(node.start());
    ASSERT_NO_THROW(node.interrupt());
}
*/