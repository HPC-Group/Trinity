#include "gtest/gtest.h"

#include "commands/ISerialObjectFactory.h"
#include "commands/Request.h"

using namespace trinity::commands;


class RequestTest : public ::testing::Test {
protected:
    RequestTest() {}
    virtual ~RequestTest() {}

    struct MyInterface {
        static VclType getType() { return VclType::InitIOSession; }

        struct RequestParams : public ISerializable {
            RequestParams() = default;
            RequestParams(int i, const std::string s)
                : intParam(i)
                , strParam(s) {}

            VclType getType() const override { return VclType::DummyIO; }
            void serialize(ISerialObject& serial) const {
                serial.append("int", intParam);
                serial.append("str", strParam);
            }
            void deserialize(const ISerialObject& serial) {
                intParam = serial.getInt("int");
                strParam = serial.getString("str");
            }

            int intParam;
            std::string strParam;
        };
    };

    using MyInterfaceRequest = RequestTemplate<MyInterface>;
};

TEST_F(RequestTest, GetType) {
    MyInterface::RequestParams requestParams(42, "Hello");
    MyInterfaceRequest request(requestParams);
    ASSERT_EQ(VclType::InitIOSession, request.getType());
}

TEST_F(RequestTest, Serialization) {
    MyInterface::RequestParams requestParams(42, "Hello");
    MyInterfaceRequest request(requestParams);
    auto serialized = ISerialObjectFactory::create();
    request.serialize(*serialized);

    MyInterfaceRequest result;
    result.deserialize(*serialized);
    ASSERT_EQ(42, result.getParams().intParam);
    ASSERT_EQ("Hello", result.getParams().strParam);
}