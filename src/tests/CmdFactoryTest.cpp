#include "gtest/gtest.h"

#include "common/Icommand.h"
#include "common/ICommandHandler.h"
#include "common/InitRendererCmd.h"

#include "processing-base/ProcessingCommandFactory.h"
#include "processing-base/InitRendererHdl.h"
#include "processing-base/SessionManager.h"

using namespace trinity::common;
using namespace trinity::processing;

class CmdFactoryTest : public ::testing::Test {
protected:
    
    CmdFactoryTest() {
    }
    
    virtual ~CmdFactoryTest() {
        SessionManagerSingleton::instance()->endAllSessions();
    }
};


TEST_F(CmdFactoryTest, XTest) {
    
    InitRendererCmd cmd;
    InitRendererHdl(std::move(cmd));
    // todo: execute, get return value
    
}