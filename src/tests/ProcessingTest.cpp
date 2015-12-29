#include "gtest/gtest.h"
#include "processing-base/ProcessingNode.h"
#include "frontend-base/ProcessingNodePrx.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ContainerTools.h"
#include "common/Commands.h"


using namespace mocca::net;

class ProcessingTest : public ::testing::Test {
protected:
    
    ProcessingTest() {
        ConnectionFactorySelector::addDefaultFactories();
    }

    virtual ~ProcessingTest() {
        ConnectionFactorySelector::removeAll();
    }
};

TEST_F(ProcessingTest, RequestInitRendererTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::processing::ProcessingNode node(std::move(aggregator));
    node.start();
    
    trinity::frontend::ProcessingNodePrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());
    trinity::common::StreamParams params;
    ASSERT_NO_THROW(proxy.initRenderer(trinity::common::VclType::DummyRenderer, params));

    node.interrupt();
}


TEST_F(ProcessingTest, ConnectToRemoteRendererTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::processing::ProcessingNode node(std::move(aggregator));
    node.start();
    trinity::frontend::ProcessingNodePrx proxy(endpoint);

    proxy.connect();
    std::unique_ptr<trinity::frontend::RendererPrx> renderer;

    trinity::common::StreamParams params;
    renderer = proxy.initRenderer(trinity::common::VclType::DummyRenderer, params);
    ASSERT_TRUE(renderer->connect());
    
    node.interrupt();
}


TEST_F(ProcessingTest, FrameBufferTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::processing::ProcessingNode node(std::move(aggregator));
    node.start();
    trinity::frontend::ProcessingNodePrx proxy(endpoint);
    
    proxy.connect();
    std::unique_ptr<trinity::frontend::RendererPrx> renderer;
    
    trinity::common::StreamParams params;
    renderer = proxy.initRenderer(trinity::common::VclType::DummyRenderer, params);
    renderer->connect();
    
    //ASSERT_NO_THROW(renderer->getFrameBuffer());
    
    node.interrupt();
}
