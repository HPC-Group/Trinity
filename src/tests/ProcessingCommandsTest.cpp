#include "gtest/gtest.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"

#include "tests/TestUtils.h"

using namespace trinity;


class ProcessingCommandsTest : public ::testing::Test {
protected:
    ProcessingCommandsTest() {}

    virtual ~ProcessingCommandsTest() {}
};


TEST_F(ProcessingCommandsTest, StreamParams) {
    StreamingParams target(2048, 1000);
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}

TEST_F(ProcessingCommandsTest, InitProcessingSessionCmd) {
    {
        StreamingParams streamingParams(2048, 1000);
        InitProcessingSessionCmd::RequestParams target("tcp.prefixed", VclType::DummyRenderer, 0, mocca::net::Endpoint("loopback", "local", "mq"), streamingParams);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        InitProcessingSessionCmd::ReplyParams target("123", "456");
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(ProcessingCommandsTest, SetIsoValueCmd) {
    SetIsoValueCmd::RequestParams target(3.14f);
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}
