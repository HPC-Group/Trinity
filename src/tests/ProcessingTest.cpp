#include "gtest/gtest.h"
#include "processing-dummy/ProcessingNode.h"
#include "frontend-dummy/ProcessingPrx.h"

#include "mocca/net/NetworkServiceLocator.h"
#include "mocca/net/LoopbackNetworkService.h"
#include "mocca/net/WSNetworkService.h"
#include "mocca/net/Endpoint.h"


using namespace mocca::net;

class ProcessingTest : public ::testing::Test {
protected:
    
    ProcessingTest() {
        // You can do set-up work for each test here.
        static bool setupDone = false;
        if(!setupDone) {
            std::unique_ptr<IProtocolNetworkService> service(new LoopbackNetworkService());
            NetworkServiceLocator::provideService(std::move(service));
            setupDone = true;
        }
        
        
    }

    virtual ~ProcessingTest() {
        // You can do clean-up work that doesn't throw exceptions here.
    }
};

TEST_F(ProcessingTest, AcceptInitRendererTest) {
    
    Endpoint endpoint (LoopbackNetworkService::protocolStatic(),
             LoopbackNetworkService::transportStatic(), "5678");
    
    trinity::ProcessingNode node(endpoint);
    node.start();
    
    trinity::ProcessingPrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());
    ASSERT_NO_THROW(proxy.spawnRenderer(trinity::vcl::DUMMY_RENDERER));

    node.interrupt();
}


TEST_F(ProcessingTest, SpawnRendererTest) {
    
    Endpoint endpoint (LoopbackNetworkService::protocolStatic(),
                       LoopbackNetworkService::transportStatic(), "5678");
    
    trinity::ProcessingNode node(endpoint);
    node.start();
    trinity::ProcessingPrx proxy(endpoint);

    ASSERT_TRUE(proxy.connect());
    std::unique_ptr<trinity::RendererPrx> renderer;
    ASSERT_NO_THROW(renderer = proxy.spawnRenderer(trinity::vcl::DUMMY_RENDERER));
    ASSERT_TRUE(renderer->connect());
    
    node.interrupt();
}


TEST_F(ProcessingTest, FrameBufferTest) {
    
    Endpoint endpoint (LoopbackNetworkService::protocolStatic(),
                       LoopbackNetworkService::transportStatic(), "5678");
    
    trinity::ProcessingNode node(endpoint);
    node.start();
    trinity::ProcessingPrx proxy(endpoint);
    
    ASSERT_TRUE(proxy.connect());
    std::unique_ptr<trinity::RendererPrx> renderer;
    ASSERT_NO_THROW(renderer = proxy.spawnRenderer(trinity::vcl::DUMMY_RENDERER));
    ASSERT_TRUE(renderer->connect());
    
    ASSERT_NO_THROW(renderer->getFrameBuffer());
    
    node.interrupt();
}
