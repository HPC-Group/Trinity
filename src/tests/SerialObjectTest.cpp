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

    struct MySerializable : public SerializableTemplate<MySerializable> {
        MySerializable()
            : myFloat(.0f)
            , myString() {}
        MySerializable(float f, const std::string s)
            : myFloat(f)
            , myString(s) {}

        void serialize(ISerialWriter& writer) const override {
            writer.appendFloat("subFloat", myFloat);
            writer.appendString("subString", myString);
        }
        void deserialize(const ISerialReader& reader) override {
            myFloat = reader.getFloat("subFloat");
            myString = reader.getString("subString");
        }

        float myFloat;
        std::string myString;
    };
};

typedef ::testing::Types<JsonSerializerFactory/*, SimpleStringSerializerFactory*/> MyTypes; // fixme dmc
TYPED_TEST_CASE(SerialObjectTest, MyTypes);

TYPED_TEST(SerialObjectTest, BasicTypes) {
    TypeParam factory;
    auto writer = factory.createWriter();
    writer->appendFloat("float", 3.14f);
    writer->appendInt("uint8", static_cast<uint8_t>(128));
    writer->appendInt("int32", -17);
    writer->appendInt("uint32", 42u);
    writer->appendInt("int64", static_cast<int64_t>(-23));
    writer->appendInt("uint64", static_cast<uint64_t>(123u));
    writer->appendBool("boolTrue", true);
    writer->appendBool("boolFalse", false);
    writer->appendString("string", "Hello");

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    ASSERT_EQ(3.14f, reader->getFloat("float"));
    ASSERT_EQ(static_cast<uint8_t>(128), reader->getUInt8("uint8"));
    ASSERT_EQ(-17, reader->getInt32("int32"));
    ASSERT_EQ(42u, reader->getUInt32("uint32"));
    ASSERT_EQ(static_cast<int64_t>(-23), reader->getInt64("int64"));
    ASSERT_EQ(static_cast<uint64_t>(123u), reader->getUInt64("uint64"));
    ASSERT_EQ(true, reader->getBool("boolTrue"));
    ASSERT_EQ(false, reader->getBool("boolFalse"));
    ASSERT_EQ("Hello", reader->getString("string"));
}

TYPED_TEST(SerialObjectTest, SubObjects) {
    TypeParam factory;
    using MyObj = typename SerialObjectTest<TypeParam>::MySerializable;
    auto writer = factory.createWriter();
    MyObj subObject{2.718f, "World"};
    writer->appendFloat("float", 3.14f);
    writer->appendObject("subObject", subObject);
    writer->appendString("string", "Hello");

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    ASSERT_EQ(3.14f, reader->getFloat("float"));
    MyObj resultSubObject;
    resultSubObject = reader->template getSerializable<MyObj>("subObject");
    ASSERT_EQ(2.718f, resultSubObject.myFloat);
    ASSERT_EQ("World", resultSubObject.myString);
    ASSERT_EQ("Hello", reader->getString("string"));
}

TYPED_TEST(SerialObjectTest, SerializationError) {
    std::string str = "this is not a serialized object";
    // fixme: doesn't work with SimpleStringReader
    ASSERT_THROW(JsonReader{str}, TrinityError);
}

TYPED_TEST(SerialObjectTest, VectorBasicTypes) {
    TypeParam factory;
    auto writer = factory.createWriter();
    writer->appendFloatVec("float", std::vector<float>{0.1f, 0.2f, 0.3f});
    writer->appendIntVec("int32", std::vector<int>{1, 2, 3, 4});
    writer->appendIntVec("uint64", std::vector<uint64_t>{5, 6, 7, 8});
    writer->appendBoolVec("bool", std::vector<bool>{true, false, true});
    writer->appendStringVec("string", std::vector<std::string>{"Hello", "World"});

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);

    auto floatRes = reader->getFloatVec("float");
    ASSERT_EQ(3, floatRes.size());
    ASSERT_EQ(0.1f, floatRes[0]);
    ASSERT_EQ(0.2f, floatRes[1]);
    ASSERT_EQ(0.3f, floatRes[2]);

    auto int32Res = reader->getInt32Vec("int32");
    ASSERT_EQ(4, int32Res.size());
    ASSERT_EQ(1, int32Res[0]);
    ASSERT_EQ(2, int32Res[1]);
    ASSERT_EQ(3, int32Res[2]);
    ASSERT_EQ(4, int32Res[3]);

    auto uint64Res = reader->getUInt64Vec("uint64");
    ASSERT_EQ(4, uint64Res.size());
    ASSERT_EQ(5, uint64Res[0]);
    ASSERT_EQ(6, uint64Res[1]);
    ASSERT_EQ(7, uint64Res[2]);
    ASSERT_EQ(8, uint64Res[3]);

    auto boolRes = reader->getBoolVec("bool");
    ASSERT_EQ(3, boolRes.size());
    ASSERT_EQ(true, boolRes[0]);
    ASSERT_EQ(false, boolRes[1]);
    ASSERT_EQ(true, boolRes[2]);

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
    writer->appendObjectVec("subObjects", vec);

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    auto res = reader->template getSerializableVec<MyObj>("subObjects");
    ASSERT_EQ(2, res.size());
    ASSERT_EQ(2.718f, res[0].myFloat);
    ASSERT_EQ("Hello", res[0].myString);
    ASSERT_EQ(3.14f, res[1].myFloat);
    ASSERT_EQ("World", res[1].myString);
}

TYPED_TEST(SerialObjectTest, EmptyVector) {
    TypeParam factory;
    auto writer = factory.createWriter();
    writer->appendIntVec("int32", std::vector<int>{});
    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    auto res = reader->getInt32Vec("int32");
    ASSERT_TRUE(res.empty());
}

TYPED_TEST(SerialObjectTest, BinaryData) {
    TypeParam factory;
    auto writer = factory.createWriter();
    writer->appendString("string1", "Hello");
    writer->appendString("string2", "World");
    auto binary = std::make_shared<std::vector<uint8_t>>();
    *binary = { 0x11, 0x22, 0x33 };
    writer->setBinary(binary);

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    ASSERT_EQ("Hello", reader->getString("string1"));
    ASSERT_EQ("World", reader->getString("string2"));
    ASSERT_EQ(*binary, *reader->getBinary());
}