#include "gtest/gtest.h"

#include "commands/ISerializable.h"
#include "commands/SerializerFactory.h"

#include "silverbullet/math/Vectors.h"

using namespace trinity;

template <typename T> class MathSerializationTest : public ::testing::Test {
protected:
    MathSerializationTest() {}

    virtual ~MathSerializationTest() {}
};

typedef ::testing::Types<
    JsonSerializerFactory,
    SimpleStringSerializerFactory
> MyTypes;

TYPED_TEST_CASE(MathSerializationTest, MyTypes);

TYPED_TEST(MathSerializationTest, Vector2_Int32) {
    TypeParam factory;
    auto writer = factory.createWriter();
    
    int32_t x = 17;
    int32_t y = 42;
    Core::Math::VECTOR2<int32_t> vec(x, y);
    
    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector2_UInt32) {
    TypeParam factory;
    auto writer = factory.createWriter();

    uint32_t x = 17;
    uint32_t y = 42;
    Core::Math::VECTOR2<uint32_t> vec(x, y);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector2_Int64) {
    TypeParam factory;
    auto writer = factory.createWriter();

    int64_t x = 17;
    int64_t y = 42;
    Core::Math::VECTOR2<int64_t> vec(x, y);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector2_UInt64) {
    TypeParam factory;
    auto writer = factory.createWriter();

    uint64_t x = 17;
    uint64_t y = 42;
    Core::Math::VECTOR2<uint64_t> vec(x, y);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector2_Float) {
    TypeParam factory;
    auto writer = factory.createWriter();

    float x = 2.7f;
    float y = 3.1f;
    Core::Math::VECTOR2<float> vec(x, y);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector2_Double) {
    TypeParam factory;
    auto writer = factory.createWriter();

    double x = 2.7;
    double y = 3.1;
    Core::Math::VECTOR2<double> vec(x, y);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector3_Int32) {
    TypeParam factory;
    auto writer = factory.createWriter();

    int32_t x = 17;
    int32_t y = 42;
    int32_t z = 123;
    Core::Math::VECTOR3<int32_t> vec(x, y, z);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector3_UInt32) {
    TypeParam factory;
    auto writer = factory.createWriter();

    uint32_t x = 17;
    uint32_t y = 42;
    uint32_t z = 123;
    Core::Math::VECTOR3<uint32_t> vec(x, y, z);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector3_Int64) {
    TypeParam factory;
    auto writer = factory.createWriter();

    int64_t x = 17;
    int64_t y = 42;
    int64_t z = 123;
    Core::Math::VECTOR3<int64_t> vec(x, y, z);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector3_UInt64) {
    TypeParam factory;
    auto writer = factory.createWriter();

    uint64_t x = 17;
    uint64_t y = 42;
    uint64_t z = 123;
    Core::Math::VECTOR3<uint64_t> vec(x, y, z);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector3_Float) {
    TypeParam factory;
    auto writer = factory.createWriter();

    float x = 2.7f;
    float y = 3.1f;
    float z = 4.2f;
    Core::Math::VECTOR3<float> vec(x, y, z);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

TYPED_TEST(MathSerializationTest, Vector3_Double) {
    TypeParam factory;
    auto writer = factory.createWriter();

    double x = 2.7;
    double y = 3.1;
    double z = 4.2;
    Core::Math::VECTOR3<double> vec(x, y, z);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}