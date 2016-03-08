#include "gtest/gtest.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"

#include "tests/TestUtils.h"

using namespace trinity::commands;


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
        InitIOSessionCmd::ReplyParams target(4711);
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
        IOData target("name", 4711, VclType::FractalIO);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
}

TEST_F(IOCommandsTest, ListFilesCmd) {
    {
        ListFilesCmd::RequestParams target;
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
    }
    {
        IOData data1("name1", 1, VclType::FractalIO);
        IOData data2("name2", 2, VclType::DummyIO);
        std::vector<IOData> dataVec{ data1, data2 };
        ListFilesCmd::ReplyParams target(dataVec);
        auto result = trinity::testing::writeAndRead(target);
        ASSERT_EQ(target, result);
        std::cout << target;
    }
}