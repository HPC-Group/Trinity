#include "gtest/gtest.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/Vcl.h"

#include "tests/TestUtils.h"

using namespace trinity;


class ErrorCommandsTest : public ::testing::Test {
protected:
    ErrorCommandsTest() {}

    virtual ~ErrorCommandsTest() {}
};


TEST_F(ErrorCommandsTest, ErrorErrorCommandsTest) {
    ErrorCmd::ReplyParams target("Some Error");
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}