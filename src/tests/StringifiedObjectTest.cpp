#include "gtest/gtest.h"

#include "commands/ISerializable.h"
#include "commands/StringifiedObject.h"

using namespace trinity::commands;

class StringifiedObjectTest : public ::testing::Test {
protected:
    StringifiedObjectTest() {}

    virtual ~StringifiedObjectTest() {}
};

TEST_F(StringifiedObjectTest, BasicTypes) {
    StringifiedObject target;
    target.append("float", 3.14f);
    target.append("int", 42);
    target.append("string", "Hello");

    ASSERT_EQ(3.14f, target.getFloat("float"));
    ASSERT_EQ(42, target.getInt("int"));
    ASSERT_EQ("Hello", target.getString("string"));
}

TEST_F(StringifiedObjectTest, SubObjects) {
    struct MySerializable : public ISerializable {
        MySerializable()
            : myFloat(.0f)
            , myString() {}
        MySerializable(float f, const std::string s)
            : myFloat(f)
            , myString(s) {}

        VclType getType() const override { return VclType::DummyIO; }
        void serialize(ISerialObject& serial) const override {
            serial.append("subFloat", myFloat);
            serial.append("subString", myString);
        }
        void deserialize(ISerialObject& serial) override {
            myFloat = serial.getFloat("subFloat");
            myString = serial.getString("subString");
        }

        float myFloat;
        std::string myString;
    };

    StringifiedObject target;
    MySerializable subObject{2.718f, "World"};
    target.append("float", 3.14f);
    target.append("subObject", subObject);
    target.append("string", "Hello");

    ASSERT_EQ(3.14f, target.getFloat("float"));
    MySerializable resultSubObject;
    target.getSerializable("subObject", resultSubObject);
    ASSERT_EQ(2.718f, resultSubObject.myFloat);
    ASSERT_EQ("World", resultSubObject.myString);
    ASSERT_EQ("Hello", target.getString("string"));
}

TEST_F(StringifiedObjectTest, MismatchingSubObjects) {
    struct MySerializable1 : public ISerializable {
        VclType getType() const override { return VclType::DummyIO; }
        void serialize(ISerialObject& serial) const override {}
        void deserialize(ISerialObject& serial) override {}
    };

    struct MySerializable2 : public ISerializable {
        VclType getType() const override { return VclType::DummyRenderer; }
        void serialize(ISerialObject& serial) const override {}
        void deserialize(ISerialObject& serial) override {}
    };

    StringifiedObject target;
    MySerializable1 subObject1;
    target.append("sub", subObject1);
    
    MySerializable2 subObject2;
    ASSERT_THROW(target.getSerializable("sub", subObject2), mocca::Error);
}