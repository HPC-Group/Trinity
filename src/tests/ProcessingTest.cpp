#include <chrono>
#include <thread>

#include "gtest/gtest.h"

#include "mocca/base/ContainerTools.h"
#include "mocca/net/ConnectionAggregator.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "common/IONodeProxy.h"

#include "frontend-base/ProcessingNodeProxy.h"
#include "io-base/IOCommandFactory.h"
#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/VisStreamSender.h"


using namespace mocca::net;
using namespace trinity;

class ProcessingTest : public ::testing::Test {
protected:
    ProcessingTest() { ConnectionFactorySelector::addDefaultFactories(); }

    virtual ~ProcessingTest() { ConnectionFactorySelector::removeAll(); }
};


TEST_F(ProcessingTest, VisStreamPutGetTest) {

    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");

    trinity::StreamingParams p;
    std::shared_ptr<trinity::VisStream> sendstream = std::make_shared<trinity::VisStream>(p);

    trinity::Frame f1;
    f1 << "123";
    sendstream->put(std::move(f1));
    auto ff1Nullable = sendstream->get();
    ASSERT_FALSE(ff1Nullable.isNull());
    auto ff1 = ff1Nullable.release();
    ASSERT_EQ("123", ff1.read(ff1.size()));

    trinity::Frame f2;
    f2 << "456";
    sendstream->put(std::move(f2));
    auto ff2Nullable = sendstream->get();
    ASSERT_FALSE(ff2Nullable.isNull());
    auto ff2 = ff2Nullable.release();
    ASSERT_EQ("456", ff2.read(ff2.size()));
}

TEST_F(ProcessingTest, VisStreamTest) {
    Endpoint endpoint(ConnectionFactorySelector::loopback(), "localhost", "5678");

    trinity::StreamingParams p;
    std::shared_ptr<trinity::VisStream> sendstream = std::make_shared<trinity::VisStream>(p);
    std::shared_ptr<trinity::VisStream> receivestream = std::make_shared<trinity::VisStream>(p);
    trinity::VisStreamReceiver rec(endpoint, receivestream);
    trinity::VisStreamSender sender(endpoint, sendstream);
    rec.startStreaming();
    sender.startStreaming();
    trinity::Frame f1;
    f1 << "123";
    trinity::Frame f2;
    f2 << "456";
    sendstream->put(std::move(f1));
    sendstream->put(std::move(f2));
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto ff1Nullable = receivestream->get();
    auto ff2Nullable = receivestream->get();
    ASSERT_FALSE(ff1Nullable.isNull());
    ASSERT_FALSE(ff2Nullable.isNull());
    auto ff1 = ff1Nullable.release();
    auto ff2 = ff2Nullable.release();
    ASSERT_EQ("123", ff1.read(ff1.size()));
    ASSERT_EQ("456", ff2.read(ff2.size()));
    rec.join();
    sender.join();
}