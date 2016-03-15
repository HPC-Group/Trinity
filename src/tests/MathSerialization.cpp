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

TYPED_TEST(MathSerializationTest, Vector2_UInt8) {
    TypeParam factory;
    auto writer = factory.createWriter();

    uint8_t x = 128;
    uint8_t y = 250;
    Core::Math::VECTOR2<uint8_t> vec(x, y);

    writer->appendObject("vec", vec);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}

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

TYPED_TEST(MathSerializationTest, Vector3_UInt8) {
    TypeParam factory;
    auto writer = factory.createWriter();

    uint8_t x = 128;
    uint8_t y = 250;
    uint8_t z = 255;
    Core::Math::VECTOR3<uint8_t> vec(x, y, z);

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

TYPED_TEST(MathSerializationTest, Matrix4_Double) {
    TypeParam factory;
    auto writer = factory.createWriter();

    double m11 = 1.0;
    double m12 = 2.0;
    double m13 = 3.0;
    double m14 = 4.0;

    double m21 = 5.0;
    double m22 = 6.0;
    double m23 = 7.0;
    double m24 = 8.0;

    double m31 = 9.0;
    double m32 = 10.0;
    double m33 = 11.0;
    double m34 = 12.0;

    double m41 = 13.0;
    double m42 = 14.0;
    double m43 = 15.0;
    double m44 = 16.0;

    Core::Math::MATRIX4<double> mat(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);

    writer->appendObject("mat", mat);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(mat)>("mat");
    
    ASSERT_EQ(m11, result.m11);
    ASSERT_EQ(m12, result.m12);
    ASSERT_EQ(m13, result.m13);
    ASSERT_EQ(m14, result.m14);

    ASSERT_EQ(m21, result.m21);
    ASSERT_EQ(m22, result.m22);
    ASSERT_EQ(m23, result.m23);
    ASSERT_EQ(m24, result.m24);

    ASSERT_EQ(m31, result.m31);
    ASSERT_EQ(m32, result.m32);
    ASSERT_EQ(m33, result.m33);
    ASSERT_EQ(m34, result.m34);

    ASSERT_EQ(m41, result.m41);
    ASSERT_EQ(m42, result.m42);
    ASSERT_EQ(m43, result.m43);
    ASSERT_EQ(m44, result.m44);
}

TYPED_TEST(MathSerializationTest, Matrix4_Float) {
    TypeParam factory;
    auto writer = factory.createWriter();

    float m11 = 1.0f;
    float m12 = 2.0f;
    float m13 = 3.0f;
    float m14 = 4.0f;

    float m21 = 5.0f;
    float m22 = 6.0f;
    float m23 = 7.0f;
    float m24 = 8.0f;

    float m31 = 9.0f;
    float m32 = 10.0f;
    float m33 = 11.0f;
    float m34 = 12.0f;

    float m41 = 13.0f;
    float m42 = 14.0f;
    float m43 = 15.0f;
    float m44 = 16.0f;

    Core::Math::MATRIX4<float> mat(m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44);

    writer->appendObject("mat", mat);
    auto byteArray = writer->write();
    auto reader = factory.createReader(byteArray);
    auto result = reader->template getSerializable<decltype(mat)>("mat");

    ASSERT_EQ(m11, result.m11);
    ASSERT_EQ(m12, result.m12);
    ASSERT_EQ(m13, result.m13);
    ASSERT_EQ(m14, result.m14);

    ASSERT_EQ(m21, result.m21);
    ASSERT_EQ(m22, result.m22);
    ASSERT_EQ(m23, result.m23);
    ASSERT_EQ(m24, result.m24);

    ASSERT_EQ(m31, result.m31);
    ASSERT_EQ(m32, result.m32);
    ASSERT_EQ(m33, result.m33);
    ASSERT_EQ(m34, result.m34);

    ASSERT_EQ(m41, result.m41);
    ASSERT_EQ(m42, result.m42);
    ASSERT_EQ(m43, result.m43);
    ASSERT_EQ(m44, result.m44);
}