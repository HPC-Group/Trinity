#include "gtest/gtest.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ISerialObjectFactory.h"
#include "commands/ProcessingCommands.h"
#include "commands/StringifiedObject.h"
#include "commands/Vcl.h"

#include "tests/TestUtils.h"

using namespace trinity::commands;


class ErrorCommandsTest : public ::testing::Test {
protected:
    ErrorCommandsTest() {}

    virtual ~ErrorCommandsTest() {}
};


TEST_F(ErrorCommandsTest, ErrorErrorCommandsTest) {
    ErrorCmd::ReplyParams target(4711);
    auto result = trinity::testing::writeAndRead(target);
    ASSERT_EQ(target, result);
}