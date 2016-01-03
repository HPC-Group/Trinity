#include "gtest/gtest.h"
#include "common/Icommand.h"
#include "common/ICommandHandler.h"
#include "common/CommandFactory.h"
#include "common/InitDummyRendererCmd.h"
#include "common/InitDummyRendererHdl.h"

using namespace trinity::common;

class CmdFactoryTest : public ::testing::Test {
protected:
    
    CmdFactoryTest() {
    }
    
    virtual ~CmdFactoryTest() {
    }
};


TEST_F(CmdFactoryTest, XTest) {
    
    InitDummyRendererCmd cmd(1, 2);
    InitDummyRendererHdl(std::move(cmd));
    
}