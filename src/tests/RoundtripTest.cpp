#include "gtest/gtest.h"

#include "common/IONodeProxy.h"
#include "common/TrinityError.h"
#include "frontend-base/ProcessingNodeProxy.h"
#include "io-base/IONode.h"
#include "io-base/IOSession.h"
#include "processing-base/ProcessingNode.h"
#include "processing-base/RenderSession.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

using namespace mocca::net;
using namespace trinity;

class NodeTest : public ::testing::Test {
protected:
    NodeTest() { ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~NodeTest() {
        ConnectionFactorySelector::removeAll();
        // BIG fixme dmc: global variables cause interdependencies between tests; this will lead to terrible problems if it isn't fixed
        RenderSessionManager::instance()->endAllSessions();
        IOSessionManager::instance()->endAllSessions();
    }
};

std::unique_ptr<ProcessingNode> createProcessingNode(const std::string& port) {
    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", port);

    std::vector<std::unique_ptr<IMessageConnectionAcceptor>> acceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(endpoint));

    std::unique_ptr<ConnectionAggregator> aggregator(
        new ConnectionAggregator(std::move(acceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    return mocca::make_unique<ProcessingNode>(std::move(aggregator));
}

std::unique_ptr<IONode> createIONode(const std::string& port) {
    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", port);

    std::vector<std::unique_ptr<IMessageConnectionAcceptor>> acceptors =
        mocca::makeUniquePtrVec<IMessageConnectionAcceptor>(ConnectionFactorySelector::bind(endpoint));

    std::unique_ptr<ConnectionAggregator> aggregator(
        new ConnectionAggregator(std::move(acceptors), ConnectionAggregator::DisconnectStrategy::RemoveConnection));
    return mocca::make_unique<IONode>(std::move(aggregator));
}

TEST_F(NodeTest, StartProcessingNodeTest) {
    auto node = createProcessingNode("5678");
    ASSERT_NO_THROW(node->start());
    ASSERT_NO_THROW(node->join());
}

TEST_F(NodeTest, StartIONodeTest) {
    auto node = createProcessingNode("5678");
    ASSERT_NO_THROW(node->start());
    ASSERT_NO_THROW(node->join());
}

TEST_F(NodeTest, ConnectToProcessingTest) {
    auto processingNode = createProcessingNode("5678");
    processingNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    ProcessingNodeProxy proxy(endpoint);

    processingNode->join();
}

TEST_F(NodeTest, ConnectToIOTest) {
    auto ioNode = createIONode("6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    IONodeProxy proxy(endpoint);

    ioNode->join();
}

TEST_F(NodeTest, InitDummyRendererTest) {
    auto processingNode = createProcessingNode("5678");
    processingNode->start();

    auto ioNode = createIONode("6678");
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
    auto processingNode = createProcessingNode("5678");
    processingNode->start();

    auto ioNode = createIONode("6678");
    ioNode->start();

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");
    Endpoint ioEndpoint(ConnectionFactorySelector::loopback(), "localhost", "6678");
    ProcessingNodeProxy proxy(endpoint);

    StreamingParams params(2048, 1000);
    ASSERT_THROW(proxy.initRenderer(VclType::FractalIO, 0, ioEndpoint, params), TrinityError);

    processingNode->join();
    ioNode->join();
}

TEST_F(NodeTest, SetIsoValueOnGridLeaperTest) {
    auto processingNode = createProcessingNode("5678");
    processingNode->start();

    auto ioNode = createIONode("6678");
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
    auto processingNode = createProcessingNode("5678");
    processingNode->start();

    auto ioNode = createIONode("6678");
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