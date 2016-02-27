#include "gtest/gtest.h"

#include "commands/ISerializable.h"
#include "commands/JsonReader.h"
#include "commands/SerializerFactory.h"

#include "mocca/base/Memory.h"

using namespace trinity::commands;

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
            writer.append("subFloat", myFloat);
            writer.append("subString", myString);
        }
        void deserialize(const ISerialReader& reader) override {
            myFloat = reader.getFloat("subFloat");
            myString = reader.getString("subString");
        }

        float myFloat;
        std::string myString;
    };
};

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

TYPED_TEST(SerialObjectTest, SubObjects) {
    TypeParam factory;
    using MyObj = typename SerialObjectTest<TypeParam>::MySerializable;
    auto writer = factory.createWriter();
    MyObj subObject{2.718f, "World"};
    writer->append("float", 3.14f);
    writer->append("subObject", subObject);
    writer->append("string", "Hello");

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
    ASSERT_THROW(JsonReader{str}, mocca::Error);
}

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
    std::vector<std::unique_ptr<ISerializable>> vec;
    vec.push_back(mocca::make_unique<MyObj>(2.718f, "Hello"));
    vec.push_back(mocca::make_unique<MyObj>(3.14f, "World"));
    writer->append("subObjects", vec);

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    auto res = reader->template getSerializableVec<MyObj>("subObjects");
    ASSERT_EQ(2, res.size());
    ASSERT_EQ(2.718f, res[0]->myFloat);
    ASSERT_EQ("Hello", res[0]->myString);
    ASSERT_EQ(3.14f, res[1]->myFloat);
    ASSERT_EQ("World", res[1]->myString);
}