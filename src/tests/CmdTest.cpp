#include "gtest/gtest.h"
#include "commands/Vcl.h"
#include "commands/StringifiedObject.h"
#include "commands/ErrorCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/IOCommands.h"
#include "commands/ISerialObjectFactory.h"

using namespace trinity::commands;


class CmdTest : public ::testing::Test {
protected:
    
    CmdTest() {
    }
    
    virtual ~CmdTest() {
    }
};


TEST_F(CmdTest, StreamParamsTest) {
    StreamingParams params(2048, 1000);
    auto obj1 = ISerialObjectFactory::create();
    params.serialize(*obj1);

    StreamingParams p;
    p.deserialize(*obj1);
    ASSERT_EQ(params.getResX(), p.getResX());
    ASSERT_EQ(params.getResY(), p.getResY());
}

TEST_F(CmdTest, ErrorCmdTest) {
    
    ErrorCmd cmd(1, 2, 3);
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    ErrorCmd parsedCmd(*obj1);
    
    ASSERT_EQ(cmd.getErrorCode(), parsedCmd.getErrorCode());
    ASSERT_EQ(cmd.getSid(), parsedCmd.getSid());
    ASSERT_EQ(cmd.getType(), parsedCmd.getType());
    ASSERT_EQ(cmd.getRid(), parsedCmd.getRid());
}




TEST_F(CmdTest, InitRendererCmdTest) {
    StreamingParams params(2048, 1000);
    InitProcessingSessionCmd cmd(1, 2, "tcp.Prefixed", VclType::DummyRenderer, 0,"loopback",  params);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    InitProcessingSessionCmd newCmd(*obj2);
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    auto obj3 = ISerialObjectFactory::create();
    newCmd.serialize(*obj3);
    obj3->writeTo(ss);

    ASSERT_EQ(ss.str(), s.str());
}

TEST_F(CmdTest, InitIOCmdTest) {

    InitIOSessionCmd cmd(1, 2, "tcp.Prefixed", 20);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    InitIOSessionCmd newCmd(*obj2);
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    auto obj3 = ISerialObjectFactory::create();
    newCmd.serialize(*obj3);
    obj3->writeTo(ss);
    
    ASSERT_EQ(ss.str(), s.str());
    ASSERT_EQ(cmd.getProtocol(), newCmd.getProtocol());
    ASSERT_EQ(cmd.getFileId(), newCmd.getFileId());
}


TEST_F(CmdTest, InitProcessingReplyTest) {
    ReplyInitProcessingSessionCmd cmd(1,2);
    cmd.setControlPort(8080);
    cmd.setVisPort(8090);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    
    
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    ReplyInitProcessingSessionCmd newCmd(*obj2);
    
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    ASSERT_EQ(8080, newCmd.getControlPort());
    ASSERT_EQ(8090, newCmd.getVisPort());
    ASSERT_EQ(cmd.getSid(), newCmd.getSid());
    ASSERT_EQ(cmd.getRid(), newCmd.getRid());
}


TEST_F(CmdTest, InitIOReplyTest) {
    ReplyInitIOSessionCmd cmd(1,2);
    cmd.setControlPort(8080);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    ReplyInitIOSessionCmd newCmd(*obj2);
    
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    ASSERT_EQ(8080, newCmd.getControlPort());
    ASSERT_EQ(cmd.getSid(), newCmd.getSid());
    ASSERT_EQ(cmd.getRid(), newCmd.getRid());
}



TEST_F(CmdTest, GetLodCountTest) {
    GetLODLevelCountCmd cmd(1,2);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    GetLODLevelCountCmd newCmd(*obj2);
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    ASSERT_EQ(cmd.getSid(), newCmd.getSid());
    ASSERT_EQ(cmd.getRid(), newCmd.getRid());
}

TEST_F(CmdTest, GetLodCountReplyTest) {
    ReplyGetLODLevelCountCmd cmd(1,2);
    cmd.setLODLevelCount(43);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    ReplyGetLODLevelCountCmd newCmd(*obj2);
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    ASSERT_EQ(cmd.getSid(), newCmd.getSid());
    ASSERT_EQ(cmd.getRid(), newCmd.getRid());
    ASSERT_EQ(cmd.getLODLevelCount(), newCmd.getLODLevelCount());
}


TEST_F(CmdTest, WrongDeserializeTest) {
    GetLODLevelCountCmd cmd(1,2);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    StreamingParams params(2048, 1000);
    InitProcessingSessionCmd otherCmd(1, 2, "tcp.Prefixed", VclType::DummyRenderer, 0,"loopback",  params);
    ASSERT_THROW(otherCmd.deserialize(*obj2), mocca::Error);
}
