#include "gtest/gtest.h"
#include "common/ICommand.h"
#include "common/ISerializable.h"
#include "common/StreamingParams.h"
#include "common/ProcessingNodeCmds.h"
#include "common/ISerialObjectFactory.h"

using namespace trinity::common;


class SerializationTest : public ::testing::Test {
protected:
    
    SerializationTest() {
    }
    
    virtual ~SerializationTest() {
    }
};


TEST_F(SerializationTest, StreamParamsTest) {
    StreamingParams params(2048, 1000);
    auto obj1 = ISerialObjectFactory::create();
    params.serialize(*obj1);

    StreamingParams p;
    p.deserialize(*obj1);
    ASSERT_EQ(p.getResX(), params.getResX());
    ASSERT_EQ(p.getResY(), params.getResY());
}


TEST_F(SerializationTest, InitRendererCmdTest) {
    StreamingParams params(2048, 1000);
    InitRendererCmd cmd(1, 2, "tcp.Prefixed", VclType::DummyRenderer, 0, params);
    std::stringstream s, ss;
    auto obj1 = ISerialObjectFactory::create();
    cmd.serialize(*obj1);
    obj1->writeTo(s);
    
    
    
    auto obj2 = ISerialObjectFactory::create();
    obj2->readFrom(s);
    InitRendererCmd newCmd(*obj2);
    
    Vcl vcl;
    ASSERT_EQ(newCmd.getType(), cmd.getType());
    
    auto obj3 = ISerialObjectFactory::create();
    newCmd.serialize(*obj3);
    obj3->writeTo(ss);

    ASSERT_EQ(ss.str(), s.str());
}