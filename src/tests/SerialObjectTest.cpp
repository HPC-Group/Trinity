#include "gtest/gtest.h"

#include "commands/ISerializable.h"
#include "commands/SerializerFactory.h"
#include "commands/JsonReader.h"

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
    auto writer = factory.createWriter();
    typename SerialObjectTest<TypeParam>::MySerializable subObject{2.718f, "World"};
    writer->append("float", 3.14f);
    writer->append("subObject", subObject);
    writer->append("string", "Hello");

    auto serialized = writer->write();
    auto reader = factory.createReader(serialized);
    ASSERT_EQ(3.14f, reader->getFloat("float"));
    typename SerialObjectTest<TypeParam>::MySerializable resultSubObject;
    reader->getSerializable("subObject", resultSubObject);
    ASSERT_EQ(2.718f, resultSubObject.myFloat);
    ASSERT_EQ("World", resultSubObject.myString);
    ASSERT_EQ("Hello", reader->getString("string"));
}

TYPED_TEST(SerialObjectTest, SerializationError) {
    std::string str = "this is not a serialized object";
    // fixme: doesn't work with SimpleStringReader
    ASSERT_THROW(JsonReader{ str }, mocca::Error);
}