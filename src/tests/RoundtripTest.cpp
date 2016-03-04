#include "gtest/gtest.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "common/IONodeProxy.h"
#include "common/Node.h"

#include "frontend-base/ProcessingNodeProxy.h"
#include "io-base/IOCommandFactory.h"
#include "processing-base/ProcessingCommandFactory.h"

using namespace mocca::net;
using namespace trinity::processing;
using namespace trinity::io;
using namespace trinity::frontend;
using namespace trinity::common;
using namespace trinity::commands;

class NodeTest : public ::testing::Test {
protected:
    NodeTest() { ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~NodeTest() { ConnectionFactorySelector::removeAll(); }
};

std::unique_ptr<Node> createNode(std::unique_ptr<ICommandFactory> factory, const std::string& port) {
    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", port);

    std::vector<std::unique_ptr<IMessageConnectionAcceptor>> acceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(endpoint));

    std::unique_ptr<ConnectionAggregator> aggregator(
        new ConnectionAggregator(std::move(acceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    return std::unique_ptr<Node>(new Node(std::move(aggregator), std::move(factory)));
}

TEST_F(NodeTest, StartProcessingNodeTest) {

    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto node = createNode(std::move(factory), "5678");
    ASSERT_NO_THROW(node->start());
    ASSERT_NO_THROW(node->join());
}

TEST_F(NodeTest, StartIONodeTest) {

    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto node = createNode(std::move(factory), "5678");
    ASSERT_NO_THROW(node->start());
    ASSERT_NO_THROW(node->join());
}

TEST_F(NodeTest, ConnectToProcessingTest) {

    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto processingNode = createNode(std::move(factory), "5678");
    processingNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    ProcessingNodeProxy proxy(endpoint);

    processingNode->join();
}

TEST_F(NodeTest, ConnectToIOTest) {

    std::unique_ptr<trinity::commands::ICommandFactory> ioFactory(new IOCommandFactory);
    auto ioNode = createNode(std::move(ioFactory), "6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    IONodeProxy proxy(endpoint);

    ioNode->join();
}

TEST_F(NodeTest, InitDummyRendererTest) {
    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto processingNode = createNode(std::move(factory), "5678");
    processingNode->start();

    std::unique_ptr<trinity::commands::ICommandFactory> ioFactory(new IOCommandFactory);
    auto ioNode = createNode(std::move(ioFactory), "6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    Endpoint ioEndpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    ProcessingNodeProxy proxy(endpoint);

    StreamingParams params(2048, 1000);
    auto renderer = proxy.initRenderer(VclType::DummyRenderer, 0, ioEndpoint, params);
    processingNode->join();
    ioNode->join();
}

TEST_F(NodeTest, InitWrongRendererTest) {
    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto processingNode = createNode(std::move(factory), "5678");
    processingNode->start();

    std::unique_ptr<trinity::commands::ICommandFactory> ioFactory(new IOCommandFactory);
    auto ioNode = createNode(std::move(ioFactory), "6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    Endpoint ioEndpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    ProcessingNodeProxy proxy(endpoint);

    StreamingParams params(2048, 1000);
    ASSERT_THROW(proxy.initRenderer(VclType::FractalIO, 0, ioEndpoint, params), mocca::Error);

    processingNode->join();
    ioNode->join();
}

TEST_F(NodeTest, SetIsoValueOnGridLeaperTest) {
    
    
    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto processingNode = createNode(std::move(factory), "5678");
    processingNode->start();

    std::unique_ptr<trinity::commands::ICommandFactory> ioFactory(new IOCommandFactory);
    auto ioNode = createNode(std::move(ioFactory), "6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    Endpoint ioEndpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    ProcessingNodeProxy proxy(endpoint);

    StreamingParams params(2048, 1000);
    auto renderer = proxy.initRenderer(VclType::GridLeaper, 0, ioEndpoint, params);
    renderer->setIsoValue(123);

    processingNode->join();
    ioNode->join();
}

TEST_F(NodeTest, CallLodFromDummyRendererTest) {
    std::unique_ptr<trinity::commands::ICommandFactory> factory(new ProcessingCommandFactory);
    auto processingNode = createNode(std::move(factory), "5678");
    processingNode->start();

    std::unique_ptr<trinity::commands::ICommandFactory> ioFactory(new IOCommandFactory);
    auto ioNode = createNode(std::move(ioFactory), "6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    Endpoint ioEndpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    ProcessingNodeProxy proxy(endpoint);

    StreamingParams params(2048, 1000);
    auto renderer = proxy.initRenderer(VclType::DummyRenderer, 24, ioEndpoint, params);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    renderer->setIsoValue(123);

    processingNode->join();
    ioNode->join();
}