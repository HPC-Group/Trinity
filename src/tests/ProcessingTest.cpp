#include <chrono>
#include <thread>

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
#include "processing-base/VisStreamSender.h"


using namespace mocca::net;
using namespace trinity::processing;
using namespace trinity::io;
using namespace trinity::frontend;
using namespace trinity::common;
using namespace trinity::commands;

class ProcessingTest : public ::testing::Test {
protected:
    ProcessingTest() { ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~ProcessingTest() { ConnectionFactorySelector::removeAll(); }
};


TEST_F(ProcessingTest, VisStreamPutGetTest) {

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");

    trinity::commands::StreamingParams p;
    std::shared_ptr<trinity::common::VisStream> sendstream = std::make_shared<trinity::common::VisStream>(p);

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

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");

    trinity::commands::StreamingParams p;
    std::shared_ptr<trinity::common::VisStream> sendstream = std::make_shared<trinity::common::VisStream>(p);
    std::shared_ptr<trinity::common::VisStream> receivestream = std::make_shared<trinity::common::VisStream>(p);
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