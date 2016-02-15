#include <chrono>
#include <thread>

#include "gtest/gtest.h"

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

/*
TEST_F(ProcessingTest, RequestInitRendererTest) {
    
    //////////////// IO NODE ////////////////
    mocca::net::Endpoint ioEndpoint (ConnectionFactorySelector::loopback(), "localhost", "7678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(ioEndpoint));
    
    
    std::unique_ptr<ConnectionAggregator> ioAggregator
    (new ConnectionAggregator(std::move(ioAcceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    trinity::io::IONode ioNode(std::move(ioAggregator));
    ioNode.start();
    //////////////////////////////////////////
    
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> acceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(endpoint));
    
    
    std::unique_ptr<ConnectionAggregator> aggregator
    (new ConnectionAggregator(std::move(acceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    
    trinity::processing::ProcessingNode node(std::move(aggregator));
    node.start();
    
    trinity::frontend::ProcessingNodePrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());
    trinity::common::StreamingParams params;
    ASSERT_NO_THROW(proxy.initRenderer(trinity::common::VclType::DummyRenderer,
                                       0 ,
                                       ioEndpoint.toString(),
                                       params));

    node.interrupt();
}



TEST_F(ProcessingTest, ConnectToRemoteRendererTest) {
    
    //////////////// IO NODE ////////////////
    mocca::net::Endpoint ioEndpoint (ConnectionFactorySelector::loopback(), "localhost", "7678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(ioEndpoint));
    
    
    std::unique_ptr<ConnectionAggregator> ioAggregator
    (new ConnectionAggregator(std::move(ioAcceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    trinity::io::IONode ioNode(std::move(ioAggregator));
    ioNode.start();
    //////////////////////////////////////////
    
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
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

    trinity::common::StreamingParams params;
    renderer = proxy.initRenderer(trinity::common::VclType::DummyRenderer,
                                               0 ,
                                               ioEndpoint.toString(),
                                               params);
    ASSERT_TRUE(renderer->connect());
    node.interrupt();
}


TEST_F(ProcessingTest, SetIsoValueTest) {
    
    //////////////// IO NODE ////////////////
    mocca::net::Endpoint ioEndpoint (ConnectionFactorySelector::loopback(), "localhost", "7678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(ioEndpoint));
    
    
    std::unique_ptr<ConnectionAggregator> ioAggregator
    (new ConnectionAggregator(std::move(ioAcceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    trinity::io::IONode ioNode(std::move(ioAggregator));
    ioNode.start();
    //////////////////////////////////////////
    
    // this is code duplication
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
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
    trinity::common::StreamingParams params;
    renderer = proxy.initRenderer(trinity::common::VclType::DummyRenderer,
                                  0 ,
                                  ioEndpoint.toString(),
                                  params);
    renderer->connect();
    // end of code duplication
    
    renderer->setIsoValue(43.f);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    int remoteSessionId = renderer->getRemoteSessionId();
    auto& session = trinity::processing::SessionManagerSingleton::instance()->getSession(remoteSessionId);
    auto& remoterenderer = static_cast<trinity::processing::DummyRenderer&>(session.getRenderer());
    ASSERT_GE(0.001f, 43.f - remoterenderer.getIsoValue());
}





TEST_F(ProcessingTest, StreamInitTest1Sec) {
    //////////////// IO NODE ////////////////
    mocca::net::Endpoint ioEndpoint (ConnectionFactorySelector::loopback(), "localhost", "7678");
    
    std::vector<std::unique_ptr<mocca::net::IMessageConnectionAcceptor>> ioAcceptors =
    mocca::makeUniquePtrVec<IMessageConnectionAcceptor> (ConnectionFactorySelector::bind(ioEndpoint));
    
    
    std::unique_ptr<ConnectionAggregator> ioAggregator
    (new ConnectionAggregator(std::move(ioAcceptors),
                              ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    trinity::io::IONode ioNode(std::move(ioAggregator));
    ioNode.start();
    //////////////////////////////////////////
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
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
    
    trinity::common::StreamingParams params;
    renderer = proxy.initRenderer(trinity::common::VclType::DummyRenderer,
                                  0 ,
                                  ioEndpoint.toString(),
                                  params);
    renderer->connect();
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    
    node.interrupt();
}

TEST_F(ProcessingTest, VisStreamPutGetTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    trinity::common::StreamingParams p;
    std::shared_ptr<trinity::common::VisStream> sendstream =
    std::make_shared<trinity::common::VisStream>(p);
    
    trinity::common::Frame f1(new mocca::ByteArray());
    *f1 << "123";
    trinity::common::Frame f2(new mocca::ByteArray());
    *f2 << "456";
    ASSERT_TRUE(sendstream->put(std::move(f1)));
    auto ff1 = sendstream->get();
    ASSERT_FALSE(ff1 == nullptr);
    ASSERT_TRUE(sendstream->put(std::move(f2)));
    auto ff2 = sendstream->get();
    ASSERT_FALSE(ff2 == nullptr);
    ASSERT_EQ("123", ff1->read(ff1->size()));
    ASSERT_EQ("456", ff2->read(ff2->size()));

}

TEST_F(ProcessingTest, VisStreamTest) {
    
    ASSERT_LE(3, CAPACITY);
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "localhost", "5678");
    
    trinity::common::StreamingParams p;
    std::shared_ptr<trinity::common::VisStream> sendstream =
    std::make_shared<trinity::common::VisStream>(p);
    std::shared_ptr<trinity::common::VisStream> receivestream =
    std::make_shared<trinity::common::VisStream>(p);
    trinity::frontend::VisStreamReceiver rec(endpoint, receivestream);
    trinity::processing::VisStreamSender sender(endpoint, sendstream);
    rec.startStreaming();
    sender.startStreaming();
    trinity::common::Frame f1(new mocca::ByteArray());
    *f1 << "123";
    trinity::common::Frame f2(new mocca::ByteArray());
    *f2 << "456";
    sendstream->put(std::move(f1));
    sendstream->put(std::move(f2));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto ff1 = receivestream->get();
    auto ff2 = receivestream->get();
    ASSERT_FALSE(ff1 == nullptr);
    ASSERT_FALSE(ff2 == nullptr);
    ASSERT_EQ("123", ff1->read(ff1->size()));
    ASSERT_EQ("456", ff2->read(ff2->size()));
}
*/