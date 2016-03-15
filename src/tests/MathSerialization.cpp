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

TYPED_TEST(MathSerializationTest, Vector2_Int32t) {
    TypeParam factory;
    auto writer = factory.createWriter();
    
    int32_t x = 17;
    int32_t y = 42;
    Core::Math::VECTOR2<int32_t> vec(x, y);
    
    writer->appendObject("vec", vec);
    auto reader = factory.createReader(writer->write());
    auto result = reader->getSerializable<decltype(vec)>("vec");
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
    auto reader = factory.createReader(writer->write());
    auto result = reader->getSerializable<decltype(vec)>("vec");
    ASSERT_EQ(x, result.x);
    ASSERT_EQ(y, result.y);
}