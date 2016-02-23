#include "gtest/gtest.h"

#include "commands/ISerializable.h"
#include "commands/JsonObject.h"
#include "commands/StringifiedObject.h"

using namespace trinity::commands;

template <typename T> class SerialObjectTest : public ::testing::Test {
protected:
    SerialObjectTest() {}

    virtual ~SerialObjectTest() {}

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
        void deserialize(const ISerialObject& serial) override {
            myFloat = serial.getFloat("subFloat");
            myString = serial.getString("subString");
        }

        float myFloat;
        std::string myString;
    };
};

typedef ::testing::Types<StringifiedObject, JsonObject> MyTypes;
TYPED_TEST_CASE(SerialObjectTest, MyTypes);

TYPED_TEST(SerialObjectTest, BasicTypes) {
    TypeParam target;
    target.append("float", 3.14f);
    target.append("int", 42);
    target.append("string", "Hello");

    ASSERT_EQ(3.14f, target.getFloat("float"));
    ASSERT_EQ(42, target.getInt("int"));
    ASSERT_EQ("Hello", target.getString("string"));
}

TYPED_TEST(SerialObjectTest, SubObjects) {
    TypeParam target;
    typename SerialObjectTest<TypeParam>::MySerializable subObject{2.718f, "World"};
    target.append("float", 3.14f);
    target.append("subObject", subObject);
    target.append("string", "Hello");

    ASSERT_EQ(3.14f, target.getFloat("float"));
    typename SerialObjectTest<TypeParam>::MySerializable resultSubObject;
    target.getSerializable("subObject", resultSubObject);
    ASSERT_EQ(2.718f, resultSubObject.myFloat);
    ASSERT_EQ("World", resultSubObject.myString);
    ASSERT_EQ("Hello", target.getString("string"));
}

TYPED_TEST(SerialObjectTest, MismatchingSubObjects) {
    struct MySerializable1 : public ISerializable {
        VclType getType() const override { return VclType::DummyIO; }
        void serialize(ISerialObject& serial) const override {}
        void deserialize(const ISerialObject& serial) override {}
    };

    struct MySerializable2 : public ISerializable {
        VclType getType() const override { return VclType::DummyRenderer; }
        void serialize(ISerialObject& serial) const override {}
        void deserialize(const ISerialObject& serial) override {}
    };

    TypeParam target;
    MySerializable1 subObject1;
    target.append("sub", subObject1);

    MySerializable2 subObject2;
    ASSERT_THROW(target.getSerializable("sub", subObject2), mocca::Error);
}

TYPED_TEST(SerialObjectTest, ReadWrite) {
    TypeParam obj1;
    obj1.setType(VclType::DummyIO);
    typename SerialObjectTest<TypeParam>::MySerializable subObject{2.718f, "World"};
    obj1.append("float", 3.14f);
    obj1.append("subObject", subObject);
    obj1.append("string", "Hello");

    std::stringstream stream1;
    obj1.writeTo(stream1);

    std::stringstream stream2(stream1.str());
    TypeParam obj2;
    obj2.readFrom(stream2);

    ASSERT_EQ(3.14f, obj2.getFloat("float"));
    typename SerialObjectTest<TypeParam>::MySerializable resultSubObject;
    obj2.getSerializable("subObject", resultSubObject);
    ASSERT_EQ(2.718f, resultSubObject.myFloat);
    ASSERT_EQ("World", resultSubObject.myString);
    ASSERT_EQ("Hello", obj2.getString("string"));
}