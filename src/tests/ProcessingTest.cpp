#include "gtest/gtest.h"
#include "processing-dummy/ProcessingNode.h"
#include "frontend-dummy/ProcessingNodePrx.h"

#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"
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
    
    trinity::ProcessingNode node(endpoint);
    node.start();
    
    trinity::ProcessingNodePrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());
    ASSERT_NO_THROW(proxy.initRenderer(trinity::VclType::DummyRenderer));

    node.interrupt();
}


TEST_F(ProcessingTest, ConnectToRemoteRendererTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    trinity::ProcessingNode node(endpoint);
    node.start();
    trinity::ProcessingNodePrx proxy(endpoint);

    ASSERT_TRUE(proxy.connect());
    std::unique_ptr<trinity::RendererPrx> renderer;

    ASSERT_NO_THROW(renderer = proxy.initRenderer(trinity::VclType::DummyRenderer));
    ASSERT_TRUE(renderer->connect());
    
    node.interrupt();
}


TEST_F(ProcessingTest, FrameBufferTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    trinity::ProcessingNode node(endpoint);
    node.start();
    trinity::ProcessingNodePrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());
    std::unique_ptr<trinity::RendererPrx> renderer;
    
    ASSERT_NO_THROW(renderer = proxy.initRenderer(trinity::VclType::DummyRenderer));
    ASSERT_TRUE(renderer->connect());
    
    ASSERT_NO_THROW(renderer->getFrameBuffer());
    
    node.interrupt();
}
