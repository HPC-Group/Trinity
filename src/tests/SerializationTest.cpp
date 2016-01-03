#include "gtest/gtest.h"
#include "common/Icommand.h"
#include "common/ISerializable.h"
#include "common/StreamingParams.h"
#include "common/InitDummyRendererCmd.h"

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
    std::stringstream s;
    params.serialize(s);
    StreamingParams p;
    p.deserialize(s);
    ASSERT_EQ(p.getResX(), params.getResX());
    ASSERT_EQ(p.getResY(), params.getResY());
}


TEST_F(SerializationTest, InitDummyRendererCmdTest) {
    StreamingParams params(2048, 1000);
    InitDummyRendererCmd cmd(1, 2, "tcp.Prefixed", params);
    std::stringstream s;
    cmd.serialize(s);
    InitDummyRendererCmd newCmd(1,2);
    newCmd.deserialize(s);
    std::stringstream ss;
    newCmd.serialize(ss);
    ASSERT_EQ(newCmd.getType(), VclType::InitDummyRenderer);
    ASSERT_EQ(ss.str(), s.str());  
}