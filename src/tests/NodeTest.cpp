#include "gtest/gtest.h"
#include "processing-base/ProcessingNode.h"
#include "frontend-base/ProcessingNodePrx.h"

#include "mocca/net/ConnectionFactorySelector.h"
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

TEST_F(NodeTest, StartNodeTest) {
    
    Endpoint endpoint (ConnectionFactorySelector::loopback(), "5678");
    
    trinity::processing::ProcessingNode node(endpoint);
    ASSERT_NO_THROW(node.start());
    ASSERT_NO_THROW(node.interrupt());
}