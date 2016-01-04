#include "gtest/gtest.h"
#include "common/Icommand.h"
#include "common/ISerializable.h"
#include "common/StreamingParams.h"
#include "common/InitRendererCmd.h"

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


TEST_F(SerializationTest, InitRendererCmdTest) {
    StreamingParams params(2048, 1000);
    InitRendererCmd cmd(1, 2, "tcp.Prefixed", VclType::DummyRenderer ,params);
    std::stringstream s;
    cmd.serialize(s);
    InitRendererCmd newCmd;
    std::string cmdName;
    s >> cmdName;
    Vcl vcl;

    ASSERT_EQ(cmdName, vcl.toString(cmd.getType()));
    newCmd.deserialize(s);
    std::stringstream ss;
    newCmd.serialize(ss);
    ASSERT_EQ(newCmd.getType(), VclType::InitRenderer);
    ASSERT_EQ(ss.str(), s.str());
}