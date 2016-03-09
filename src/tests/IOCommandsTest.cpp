#include "gtest/gtest.h"

#include "common/TrinityError.h"
#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOCommandsHandler.h"

#include "tests/TestUtils.h"

using namespace trinity;


class IOCommandsTest : public ::testing::Test {
protected:
    IOCommandsTest() {}

    virtual ~IOCommandsTest() {}
};

TEST_F(IOCommandsTest, InitIOSessionCmd) {
    {
        InitIOSessionCmd::RequestParams target("protocol", 42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        InitIOSessionCmd::ReplyParams target("4711");
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, GetLODLevelCountCmd) {
    {
        GetLODLevelCountCmd::ReplyParams target(4711);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, IOData) {
    {
        IOData target("name", 4711, IOData::DataType::Dataset);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesCmd) {
    {
        ListFilesCmd::RequestParams target(42);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        IOData data1("name1", 1, IOData::DataType::Dataset);
        IOData data2("name2", 2, IOData::DataType::Directory);
        std::vector<IOData> dataVec{ data1, data2 };
        ListFilesCmd::ReplyParams target(dataVec);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesReqRep) {
    {
        ListFilesCmd::RequestParams requestParams(0);
        ListFilesRequest request(requestParams, 1, 2);
        auto reply = trinity::testing::handleRequest<ListFilesHdl>(request);
        ASSERT_EQ(2, reply.getParams().getIOData().size());
    }
    {
        ListFilesCmd::RequestParams requestParams(42); // invalid directory ID
        ListFilesRequest request(requestParams, 1, 2);
        ASSERT_THROW(trinity::testing::handleRequest<ListFilesHdl>(request), TrinityError);
    }
}