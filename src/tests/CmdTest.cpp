#include <string>

#include "gtest/gtest.h"
#include "common/Commands.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"


using namespace mocca::net;
using namespace trinity::common;

class CmdTest : public ::testing::Test {
protected:
    
    CmdTest() {
        ConnectionFactorySelector::addDefaultFactories();
    }

    virtual ~CmdTest() {
        ConnectionFactorySelector::removeAll();
    }
};

TEST_F(CmdTest, ParamsToStringTest) {
    
    StreamParams params;
    params.m_resX = 1680;
    params.m_resY = 1020;
    std::string stringifiedParams = params.toString();
    ASSERT_EQ("1680:1020", stringifiedParams);
}

TEST_F(CmdTest, StringToParamsTest) {
    
    std::string s = "1680:1020";
    StreamParams params(s);
    ASSERT_EQ(1680, params.m_resX);
    ASSERT_EQ(1020, params.m_resY);
}

TEST_F(CmdTest, AssembleInitRendererTest) {
    
    int rid = 12;
    int sid = 10;
    std::string protocol = "tcp.Prefixed";
    StreamParams params;
    params.m_resX = 1234;
    
    Vcl v;
    std::string ret = v.assembleInitRenderer(sid, rid, protocol,
                                             VclType::DummyRenderer, params);
    
    std::string dummyRenderer = v.toString(VclType::DummyRenderer);
    std::string request = v.toString(VclType::InitRenderer);
    ASSERT_EQ(ret,
              request + "_" + std::to_string(sid) + "_" + std::to_string(rid) + "_"
              + protocol + "_" + dummyRenderer + "_" + params.toString());
}

TEST_F(CmdTest, AssembleIRetnitRendererTest) {
    Vcl v;
    int sid = 12;
    int rid = 10;
    int newSid = 13;
    int ctrlPort = 5050;
    int visPort = 5051;
    std::string retTag = v.toString(VclType::TrinityReturn);
    std::string ret = v.assembleRetInitRenderer(sid, rid, newSid, ctrlPort, visPort);
    ASSERT_EQ(retTag + "_" + std::to_string(sid)
              + "_" + std::to_string(rid) + "_" + std::to_string(newSid) + "_"
              + std::to_string(ctrlPort) + "_" + std::to_string(visPort), ret);
}