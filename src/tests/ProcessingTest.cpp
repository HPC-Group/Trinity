#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "processing-base/ProcessingNode.h"
#include "frontend-base/ProcessingNodePrx.h"
#include "common/Commands.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"
#include "mocca/base/ContainerTools.h"



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


TEST_F(ProcessingTest, StreamInitTest1Sec) {
    
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
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    node.interrupt();
}


TEST_F(ProcessingTest, VisStreamTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    trinity::common::StreamParams p;
    std::shared_ptr<trinity::common::VisStream> stream =
    std::make_shared<trinity::common::VisStream>(p);
    trinity::frontend::VisStreamReceiver rec(endpoint, stream);
    trinity::processing::VisStreamSender sender(endpoint, stream);
    rec.startStreaming();
    sender.startStreaming();
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

