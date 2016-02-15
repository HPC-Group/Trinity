#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ContainerTools.h"




using namespace mocca::net;

class IOTest : public ::testing::Test {
protected:
    
    IOTest() {
        ConnectionFactorySelector::addDefaultFactories();
    }

    virtual ~IOTest() {
        ConnectionFactorySelector::removeAll();
    }
};

/*
TEST_F(IOTest, ConnectTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::io::IONode node(std::move(aggregator));
    node.start();
    
    
    trinity::frontend::IONodePrx proxy(endpoint);
    ASSERT_TRUE(proxy.connect());
    node.interrupt();
}


TEST_F(IOTest, RequestInitIOSessionTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::io::IONode node(std::move(aggregator));
    node.start();
    
    trinity::frontend::IONodePrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());

    int sid1, sid2;
    sid1 = proxy.initIO(0);
    sid2 = proxy.initIO(0);
    ASSERT_EQ(1, sid2 - sid1);
    node.interrupt();
}
 */


