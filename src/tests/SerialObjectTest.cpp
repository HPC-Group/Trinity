#include "gtest/gtest.h"

#include "commands/ISerializable.h"
#include "commands/JsonReader.h"
#include "commands/SerializerFactory.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity;


template <typename T> class SerialObjectTest : public ::testing::Test {
protected:
    SerialObjectTest() {}

    virtual ~SerialObjectTest() {}
};

struct MySerializable {
    friend void deserializeObjectImpl(MySerializable& obj, ISerialReader& writer);

    MySerializable()
        : myFloat(.0f)
        , myString() {}
    MySerializable(float f, const std::string s)
        : myFloat(f)
        , myString(s) {}

    float myFloat;
    std::string myString;
};

void serializeObjectImpl(const MySerializable& obj, ISerialWriter& writer) {
    serializeFloat("float", obj.myFloat, writer);
    serializeString("string", obj.myString, writer);
}

void deserializeObjectImpl(MySerializable& obj, ISerialReader& reader) {
    deserializeFloat("float", obj.myFloat, reader);
    deserializeString("string", obj.myString, reader);
}

struct MySerializable2 {
    friend void deserializeObjectImpl(MySerializable2& obj, ISerialReader& writer);

    MySerializable2()
        : myFloat(.0f)
        , myObj()
        , myString() {}
    MySerializable2(float f, const MySerializable& o, const std::string s)
        : myFloat(f)
        , myObj(o)
        , myString(s) {}

    float myFloat;
    MySerializable myObj;
    std::string myString;
};

void serializeObjectImpl(const MySerializable2& obj, ISerialWriter& writer) {
    serializeFloat("float", obj.myFloat, writer);
    serializeObject("obj", obj.myObj, writer);
    serializeString("string", obj.myString, writer);
}

void deserializeObjectImpl(MySerializable2& obj, ISerialReader& reader) {
    deserializeFloat("float", obj.myFloat, reader);
    deserializeObject("obj", obj.myObj, reader);
    deserializeString("string", obj.myString, reader);
}

typedef ::testing::Types<JsonSerializerFactory, SimpleStringSerializerFactory> MyTypes;
TYPED_TEST_CASE(SerialObjectTest, MyTypes);

TYPED_TEST(SerialObjectTest, BasicTypes) {
    TypeParam factory;
    auto writer = factory.createWriter();
    writer->append("float", 3.14f);
    writer->append("int", 42);
    writer->append("string", "Hello");

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    ASSERT_EQ(3.14f, reader->getFloat("float"));
    ASSERT_EQ(42, reader->getInt("int"));
    ASSERT_EQ("Hello", reader->getString("string"));
}

TYPED_TEST(SerialObjectTest, Objects) {
    TypeParam factory;
    MySerializable obj{2.718f, "Hello"};
    auto byteArray = toByteArray(obj, factory);
    auto result = fromByteArray<MySerializable>(byteArray, factory);

    ASSERT_EQ(2.718f, result.myFloat);
    ASSERT_EQ("Hello", result.myString);
}

TYPED_TEST(SerialObjectTest, NestedObjects) {
    TypeParam factory;
    MySerializable obj{ 2.718f, "Hello" };
    MySerializable2 obj2{ 3.14f, obj, "World" };
    auto byteArray = toByteArray(obj2, factory);
    auto result = fromByteArray<MySerializable2>(byteArray, factory);

    ASSERT_EQ(3.14f, result.myFloat);
    ASSERT_EQ(2.718f, result.myObj.myFloat);
    ASSERT_EQ("Hello", result.myObj.myString);
    ASSERT_EQ("World", result.myString);
}

TYPED_TEST(SerialObjectTest, SerializationError) {
    std::string str = "this is not a serialized object";
    // fixme: doesn't work with SimpleStringReader
    ASSERT_THROW(JsonReader{str}, TrinityError);
}
/*
TYPED_TEST(SerialObjectTest, VectorBasicTypes) {
    TypeParam factory;
    auto writer = factory.createWriter();
    writer->append("float", std::vector<float>{0.1f, 0.2f, 0.3f});
    writer->append("int", std::vector<int>{1, 2, 3, 4});
    writer->append("string", std::vector<std::string>{"Hello", "World"});

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);

    auto floatRes = reader->getFloatVec("float");
    ASSERT_EQ(3, floatRes.size());
    ASSERT_EQ(0.1f, floatRes[0]);
    ASSERT_EQ(0.2f, floatRes[1]);
    ASSERT_EQ(0.3f, floatRes[2]);

    auto intRes = reader->getIntVec("int");
    ASSERT_EQ(4, intRes.size());
    ASSERT_EQ(1, intRes[0]);
    ASSERT_EQ(2, intRes[1]);
    ASSERT_EQ(3, intRes[2]);
    ASSERT_EQ(4, intRes[3]);

    auto stringRes = reader->getStringVec("string");
    ASSERT_EQ(2, stringRes.size());
    ASSERT_EQ("Hello", stringRes[0]);
    ASSERT_EQ("World", stringRes[1]);
}

TYPED_TEST(SerialObjectTest, VectorSubObject) {
    TypeParam factory;
    auto writer = factory.createWriter();
    using MyObj = typename SerialObjectTest<TypeParam>::MySerializable;
    auto subObject1 = mocca::make_unique<MyObj>(2.718f, "Hello");
    auto subObject2 = mocca::make_unique<MyObj>(3.14f, "World");
    std::vector<ISerializable*> vec{ subObject1.get(), subObject2.get() };
    writer->append("subObjects", vec);

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    auto res = reader->template getSerializableVec<MyObj>("subObjects");
    ASSERT_EQ(2, res.size());
    ASSERT_EQ(2.718f, res[0].myFloat);
    ASSERT_EQ("Hello", res[0].myString);
    ASSERT_EQ(3.14f, res[1].myFloat);
    ASSERT_EQ("World", res[1].myString);
}
*/

//
// TYPED_TEST(SerialObjectTest, NewTest) {
//    NewTestStruct s{ 42, 3.14f };
//
//    auto b = serialize(s);
//    auto r = deserialize<NewTestStruct>(b);
//
//    ASSERT_EQ(42, r.getX());
//    ASSERT_EQ(3.14f, r.getY());
//}
//
// TYPED_TEST(SerialObjectTest, NewTest2) {
//    NewTestStruct s{ 42, 3.14f };
//    NewTestStruct2 s2{ s, 17 };
//
//    auto b = serialize(s2);
//    auto r = deserialize<NewTestStruct2>(b);
//
//    ASSERT_EQ(42, r.getInner().getX());
//    ASSERT_EQ(3.14f, r.getInner().getY());
//    ASSERT_EQ(17, r.getZ());
//}
//
// TYPED_TEST(SerialObjectTest, NewVecTest) {
//    NewTestStruct s1{ 1, 2.0f };
//    NewTestStruct s2{ 3, 4.0f };
//    NewTestStruct s3{ 5, 6.0f };
//    std::vector<NewTestStruct> vec{ s1, s2, s3 };
//
//    auto b = serialize(vec);
//    auto r = deserializeVec<NewTestStruct>(b);
//
//    ASSERT_EQ(3, r.size());
//}