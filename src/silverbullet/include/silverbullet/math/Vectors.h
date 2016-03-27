#pragma once

#include "commands/ISerializable.h"
#include "common/TrinityError.h"

#include "silverbullet/base/SilverBulletBase.h"

#include <cassert>
//#define _USE_MATH_DEFINES
#include <cmath>
#include <iomanip>
#include <istream>
#include <limits>
#include <memory>
#include <type_traits>
#include <vector>

#ifdef DETECTED_OS_WINDOWS
#pragma warning(disable : 4995) // disable deprecated warning
#endif

#include <ostream>

#ifdef DETECTED_OS_WINDOWS
#pragma warning(default : 4995)
#endif

#ifdef DETECTED_OS_WINDOWS
#pragma warning(disable : 4201) // Disable warning messages about nameless union
#endif

namespace Core {
  namespace Math {
    template <class T> class MATRIX2;
    template <class T> class MATRIX3;
    template <class T> class MATRIX4;
    
    template <class T = int32_t> class VECTOR2 : public trinity::ISerializable {
    public:
      T x, y;
      
      VECTOR2<T>()
      : x(0)
      , y(0) {}
      template <class S> explicit VECTOR2<T>(const std::vector<S>& v, const T& defaultVal = T(0)) {
        x = T(v.size() > 0 ? v[0] : defaultVal);
        y = T(v.size() > 1 ? v[1] : defaultVal);
      }
      
      // We do not want the implicit copy c'tor to fulfill the std::is_trivially_copyable properties!
      // VECTOR2<T>(const VECTOR2<T> &other): x(other.x), y(other.y) {}
      template <class S>
      explicit VECTOR2<T>(const VECTOR2<S>& other)
      : x(T(other.x))
      , y(T(other.y)) {}
      VECTOR2<T>(const T _x, const T _y)
      : x(_x)
      , y(_y) {}
      VECTOR2<T>(const T* vec)
      : x(vec[0])
      , y(vec[1]) {}
      
      bool operator==(const VECTOR2<T>& other) const { return (other.x == x && other.y == y); }
      bool operator!=(const VECTOR2<T>& other) const { return (other.x != x || other.y != y); }
      
      // binary operators with scalars
      VECTOR2<T> operator+(T scalar) const { return VECTOR2<T>(x + scalar, y + scalar); }
      VECTOR2<T> operator-(T scalar) const { return VECTOR2<T>(x - scalar, y - scalar); }
      VECTOR2<T> operator*(T scalar) const { return VECTOR2<T>(x * scalar, y * scalar); }
      VECTOR2<T> operator/(T scalar) const { return VECTOR2<T>(x / scalar, y / scalar); }
      VECTOR2<T> operator%(T scalar) const { return VECTOR2<T>(x % scalar, y % scalar); }
      
      // binary operators with vectors
      VECTOR2<T> operator+(const VECTOR2& other) const { return VECTOR2<T>(x + other.x, y + other.y); }
      VECTOR2<T> operator-(const VECTOR2& other) const { return VECTOR2<T>(x - other.x, y - other.y); }
      VECTOR2<T> operator/(const VECTOR2& other) const { return VECTOR2<T>(x / other.x, y / other.y); }
      VECTOR2<T> operator*(const VECTOR2& other) const { return VECTOR2<T>(x * other.x, y * other.y); } // component product
      VECTOR2<T> operator%(const VECTOR2& other) const { return VECTOR2<T>(x % other.x, y % other.y); } // component modulo
      T operator^(const VECTOR2<T>& other) const { return T(x * other.x + y * other.y); }               // dot product
      
      // unary operators
      VECTOR2<T> operator+() const { return *this; }
      VECTOR2<T> operator-() const { return *this * -1; }
      VECTOR2<T> operator~() const { return VECTOR2<T>(T(1) / x, T(1) / y); }
      
      // binary operators with a matrix
      VECTOR2<T> operator*(const MATRIX2<T>& matrix) const {
        return VECTOR2<T>(x * matrix.m11 + y * matrix.m21, x * matrix.m12 + y * matrix.m22);
      }
      
      // We do not want to implement the copy assignment operator to fulfill the std::is_trivially_copyable properties!
      // VECTOR2<T>& operator=(const VECTOR2<T>& other)  { x = other.x; y = other.y;return *this; }
      VECTOR2<T>& operator+=(const VECTOR2<T>& other) {
        x += other.x;
        y += other.y;
        return *this;
      }
      VECTOR2<T>& operator-=(const VECTOR2<T>& other) {
        x -= other.x;
        y -= other.y;
        return *this;
      }
      VECTOR2<T>& operator*=(const VECTOR2<T>& other) {
        x *= other.x;
        y *= other.y;
        return *this;
      }
      VECTOR2<T>& operator/=(const VECTOR2<T>& other) {
        x /= other.x;
        y /= other.y;
        return *this;
      }
      VECTOR2<T>& operator=(const T& other) {
        x = other;
        y = other;
        return *this;
      }
      VECTOR2<T>& operator+=(const T& other) {
        x += other;
        y += other;
        return *this;
      }
      VECTOR2<T>& operator-=(const T& other) {
        x -= other;
        y -= other;
        return *this;
      }
      VECTOR2<T>& operator*=(const T& other) {
        x *= other;
        y *= other;
        return *this;
      }
      VECTOR2<T>& operator/=(const T& other) {
        x /= other;
        y /= other;
        return *this;
      }
      
      friend std::ostream& operator<<(std::ostream& os, const VECTOR2<T>& v) {
        os << "[" << v.x << ", " << v.y << "]";
        return os;
      }
      
      T& operator[](size_t i) {
        assert(i <= 1);
        return (i == 0) ? this->x : this->y;
      }
      const T& operator[](size_t i) const {
        assert(i <= 1);
        return (i == 0) ? this->x : this->y;
      }
      
      VECTOR2<T> abs() const { return VECTOR2<T>(fabs(x), fabs(y)); }
      T area() const { return x * y; }
      T length() const { return sqrt(sqLength()); }
      T sqLength() const { return T(x * x + y * y); }
      void normalize() {
        T len = length();
        x /= len;
        y /= len;
      }
      T maxVal() const { return std::max<T>(x, y); }
      T minVal() const { return std::min<T>(x, y); }
      
      VECTOR2<T> makepow2() const {
        VECTOR2<T> vOut;
        vOut.x = T(1 << int(ceil(log(double(x)) / log(2.0))));
        vOut.y = T(1 << int(ceil(log(double(y)) / log(2.0))));
        return vOut;
      }
      
      void StoreMin(const VECTOR2<T>& other) {
        x = std::min<T>(x, other.x);
        y = std::min<T>(y, other.y);
      }
      
      void StoreMax(const VECTOR2<T>& other) {
        x = std::max<T>(x, other.x);
        y = std::max<T>(y, other.y);
      }
      
      std::unique_ptr<trinity::ISerializable> clone() const override { return std::unique_ptr<ISerializable>(new VECTOR2(*this)); }
      void serialize(trinity::ISerialWriter& writer) const override { serializeImpl(writer); }
      void deserialize(const trinity::ISerialReader& reader) override { deserializeImpl(reader); }
      
    private:
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, bool>::value>::type* = nullptr) const {
        writer.appendBool("x", x);
        writer.appendBool("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, uint8_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, int32_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, uint32_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, int64_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, uint64_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, float>::value>::type* = nullptr) const {
        writer.appendFloat("x", x);
        writer.appendFloat("y", y);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, double>::value>::type* = nullptr) const {
        writer.appendDouble("x", x);
        writer.appendDouble("y", y);
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, bool>::value>::type* = nullptr) {
        x = reader.getBool("x");
        y = reader.getBool("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, uint8_t>::value>::type* = nullptr) {
        x = reader.getUInt8("x");
        y = reader.getUInt8("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, int32_t>::value>::type* = nullptr) {
        x = reader.getInt32("x");
        y = reader.getInt32("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, uint32_t>::value>::type* = nullptr) {
        x = reader.getUInt32("x");
        y = reader.getUInt32("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, int64_t>::value>::type* = nullptr) {
        x = reader.getInt64("x");
        y = reader.getInt64("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, uint64_t>::value>::type* = nullptr) {
        x = reader.getUInt64("x");
        y = reader.getUInt64("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, float>::value>::type* = nullptr) {
        x = reader.getFloat("x");
        y = reader.getFloat("y");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, double>::value>::type* = nullptr) {
        x = reader.getDouble("x");
        y = reader.getDouble("y");
      }
    };
    
    
    template <class T> VECTOR2<T> operator+(T scalar, const VECTOR2<T>& vec) {
      return VECTOR2<T>(scalar + vec.x, scalar + vec.y);
    }
    template <class T> VECTOR2<T> operator-(T scalar, const VECTOR2<T>& vec) {
      return VECTOR2<T>(scalar - vec.x, scalar - vec.y);
    }
    template <class T> VECTOR2<T> operator*(T scalar, const VECTOR2<T>& vec) {
      return VECTOR2<T>(scalar * vec.x, scalar * vec.y);
    }
    template <class T> VECTOR2<T> operator/(T scalar, const VECTOR2<T>& vec) {
      return VECTOR2<T>(scalar / vec.x, scalar / vec.y);
    }
    template <class T> VECTOR2<T> operator%(T scalar, const VECTOR2<T>& vec) {
      return VECTOR2<T>(scalar % vec.x, scalar % vec.y);
    }
    
    
    template <class T = int32_t> class VECTOR3 : public trinity::ISerializable {
    public:
      T x, y, z;
      
      VECTOR3<T>()
      : x(0)
      , y(0)
      , z(0) {}
      template <class S> explicit VECTOR3<T>(const std::vector<S>& v, const T& defaultVal = T(0)) {
        x = T(v.size() > 0 ? v[0] : defaultVal);
        y = T(v.size() > 1 ? v[1] : defaultVal);
        z = T(v.size() > 2 ? v[2] : defaultVal);
      }
      
      // We do not want the implicit copy c'tor to fulfill the std::is_trivially_copyable properties!
      // VECTOR3<T>(const VECTOR3<T> &other): x(other.x), y(other.y), z(other.z) {}
      template <class S>
      explicit VECTOR3<T>(const VECTOR3<S>& other)
      : x(T(other.x))
      , y(T(other.y))
      , z(T(other.z)) {}
      VECTOR3<T>(const VECTOR2<T>& other, const T _z)
      : x(other.x)
      , y(other.y)
      , z(_z) {}
      VECTOR3<T>(const T _x, const T _y, const T _z)
      : x(_x)
      , y(_y)
      , z(_z) {}
      VECTOR3<T>(const T* vec)
      : x(vec[0])
      , y(vec[1])
      , z(vec[2]) {}
      
      bool operator==(const VECTOR3<T>& other) const { return (other.x == x && other.y == y && other.z == z); }
      bool operator!=(const VECTOR3<T>& other) const { return (other.x != x || other.y != y || other.z != z); }
      
      // binary operators with scalars
      VECTOR3<T> operator+(T scalar) const { return VECTOR3<T>(x + scalar, y + scalar, z + scalar); }
      VECTOR3<T> operator-(T scalar) const { return VECTOR3<T>(x - scalar, y - scalar, z - scalar); }
      VECTOR3<T> operator*(T scalar) const { return VECTOR3<T>(x * scalar, y * scalar, z * scalar); }
      VECTOR3<T> operator/(T scalar) const { return VECTOR3<T>(x / scalar, y / scalar, z / scalar); }
      VECTOR3<T> operator%(T scalar) const { return VECTOR3<T>(x % scalar, y % scalar, z % scalar); }
      
      // binary operators with vectors
      VECTOR3<T> operator+(const VECTOR3<T>& other) const { return VECTOR3<T>(x + other.x, y + other.y, z + other.z); }
      VECTOR3<T> operator-(const VECTOR3<T>& other) const { return VECTOR3<T>(x - other.x, y - other.y, z - other.z); }
      VECTOR3<T> operator/(const VECTOR3<T>& other) const { return VECTOR3<T>(x / other.x, y / other.y, z / other.z); }
      VECTOR3<T> operator*(const VECTOR3<T>& other) const { return VECTOR3<T>(x * other.x, y * other.y, z * other.z); } // component product
      VECTOR3<T> operator%(const VECTOR3<T>& other) const {
        return VECTOR3<T>(y * other.z - z * other.y, z * other.x - x * other.z, x * other.y - y * other.x);
      }                                                                                                 // cross product
      T operator^(const VECTOR3<T>& other) const { return T(x * other.x + y * other.y + z * other.z); } // dot product
      
      // unary operators
      VECTOR3<T> operator+() const { return *this; }
      VECTOR3<T> operator-() const { return *this * -1; }
      VECTOR3<T> operator~() const { return VECTOR3<T>(T(1) / x, T(1) / y, T(1) / z); }
      
      // binary operators with a matrix
      VECTOR3<T> operator*(const MATRIX3<T>& matrix) const {
        return VECTOR3<T>(x * matrix.m11 + y * matrix.m21 + z * matrix.m31, x * matrix.m12 + y * matrix.m22 + z * matrix.m32,
                          x * matrix.m13 + y * matrix.m23 + z * matrix.m33);
      }
      
      // We do not want to implement the copy assignment operator to fulfill the std::is_trivially_copyable properties!
      // VECTOR3<T>& operator=(const VECTOR3<T>& other)  { x = other.x; y = other.y; z = other.z; return *this; }
      VECTOR3<T>& operator+=(const VECTOR3<T>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
      }
      VECTOR3<T>& operator-=(const VECTOR3<T>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
      }
      VECTOR3<T>& operator*=(const VECTOR3<T>& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        return *this;
      }
      VECTOR3<T>& operator/=(const VECTOR3<T>& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        return *this;
      }
      VECTOR3<T>& operator=(const T& other) {
        x = other;
        y = other;
        z = other;
        return *this;
      }
      VECTOR3<T>& operator+=(const T& other) {
        x += other;
        y += other;
        z += other;
        return *this;
      }
      VECTOR3<T>& operator-=(const T& other) {
        x -= other;
        y -= other;
        z -= other;
        return *this;
      }
      VECTOR3<T>& operator*=(const T& other) {
        x *= other;
        y *= other;
        z *= other;
        return *this;
      }
      VECTOR3<T>& operator/=(const T& other) {
        x /= other;
        y /= other;
        z /= other;
        return *this;
      }
      
      friend std::ostream& operator<<(std::ostream& os, const VECTOR3<T>& v) {
        os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
        return os;
      }
      
      T& operator[](size_t i) {
        assert(i <= 2);
        switch (i) {
          case 0:
            return this->x;
          case 1:
            return this->y;
          default:
            return this->z;
        }
      }
      const T& operator[](size_t i) const {
        assert(i <= 2);
        switch (i) {
          case 0:
            return this->x;
          case 1:
            return this->y;
          default:
            return this->z;
        }
      }
      
      VECTOR3<T> abs() const { return VECTOR3<T>(fabs(x), fabs(y), fabs(z)); }
      T maxVal() const { return std::max<T>(x, std::max<T>(y, z)); }
      T minVal() const { return std::min<T>(x, std::min<T>(y, z)); }
      T volume() const { return x * y * z; }
      T length() const { return sqrt(sqLength()); }
      T sqLength() const { return T(x * x + y * y + z * z); }
      void normalize() {
        T len = length();
        x /= len;
        y /= len;
        z /= len;
      }
      void normalize(T epsilon, const VECTOR3<T> replacement = VECTOR3<T>(T(0), T(0), T(1))) {
        T len = length();
        if (len > epsilon) {
          x /= len;
          y /= len;
          z /= len;
        } else { // specify some arbitrary normal
          x = replacement.x;
          y = replacement.y;
          z = replacement.z;
        }
      }
      VECTOR3<T> normalized() const {
        T len = length();
        return VECTOR3<T>(x / len, y / len, z / len);
      }
      VECTOR3<T> normalized(T epsilon, const VECTOR3<T> replacement = VECTOR3<T>(T(0), T(0), T(1))) const {
        T len = length();
        if (len > epsilon) {
          return VECTOR3<T>(x / len, y / len, z / len);
        } else { // specify some arbitrary normal
          return replacement;
        }
      }
      
      
      VECTOR2<T> xx() const { return VECTOR2<T>(x, x); }
      VECTOR2<T> xy() const { return VECTOR2<T>(x, y); }
      VECTOR2<T> xz() const { return VECTOR2<T>(x, z); }
      VECTOR2<T> yx() const { return VECTOR2<T>(y, x); }
      VECTOR2<T> yy() const { return VECTOR2<T>(y, y); }
      VECTOR2<T> yz() const { return VECTOR2<T>(y, z); }
      VECTOR2<T> zx() const { return VECTOR2<T>(z, x); }
      VECTOR2<T> zy() const { return VECTOR2<T>(z, y); }
      VECTOR2<T> zz() const { return VECTOR2<T>(z, z); }
      
      VECTOR3<T> makepow2() const {
        VECTOR3<T> vOut;
        vOut.x = T(1 << int(ceil(log(double(x)) / log(2.0))));
        vOut.y = T(1 << int(ceil(log(double(y)) / log(2.0))));
        vOut.z = T(1 << int(ceil(log(double(z)) / log(2.0))));
        return vOut;
      }
      
      void StoreMin(const VECTOR3<T>& other) {
        x = std::min<T>(x, other.x);
        y = std::min<T>(y, other.y);
        z = std::min<T>(z, other.z);
      }
      
      void StoreMax(const VECTOR3<T>& other) {
        x = std::max<T>(x, other.x);
        y = std::max<T>(y, other.y);
        z = std::max<T>(z, other.z);
      }
      
      std::unique_ptr<trinity::ISerializable> clone() const override { return std::unique_ptr<ISerializable>(new VECTOR3(*this)); }
      void serialize(trinity::ISerialWriter& writer) const override { serializeImpl(writer); }
      void deserialize(const trinity::ISerialReader& reader) override { deserializeImpl(reader); }
      
    private:
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, bool>::value>::type* = nullptr) const {
        writer.appendBool("x", x);
        writer.appendBool("y", y);
        writer.appendBool("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, uint8_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
        writer.appendInt("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, int32_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
        writer.appendInt("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, uint32_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
        writer.appendInt("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, int64_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
        writer.appendInt("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, uint64_t>::value>::type* = nullptr) const {
        writer.appendInt("x", x);
        writer.appendInt("y", y);
        writer.appendInt("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, float>::value>::type* = nullptr) const {
        writer.appendFloat("x", x);
        writer.appendFloat("y", y);
        writer.appendFloat("z", z);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, double>::value>::type* = nullptr) const {
        writer.appendDouble("x", x);
        writer.appendDouble("y", y);
        writer.appendDouble("z", z);
      }
      
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, bool>::value>::type* = nullptr) {
        x = reader.getBool("x");
        y = reader.getBool("y");
        z = reader.getBool("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, uint8_t>::value>::type* = nullptr) {
        x = reader.getUInt8("x");
        y = reader.getUInt8("y");
        z = reader.getUInt8("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, int32_t>::value>::type* = nullptr) {
        x = reader.getInt32("x");
        y = reader.getInt32("y");
        z = reader.getInt32("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, uint32_t>::value>::type* = nullptr) {
        x = reader.getUInt32("x");
        y = reader.getUInt32("y");
        z = reader.getUInt32("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, int64_t>::value>::type* = nullptr) {
        x = reader.getInt64("x");
        y = reader.getInt64("y");
        z = reader.getInt64("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, uint64_t>::value>::type* = nullptr) {
        x = reader.getUInt64("x");
        y = reader.getUInt64("y");
        z = reader.getUInt64("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, float>::value>::type* = nullptr) {
        x = reader.getFloat("x");
        y = reader.getFloat("y");
        z = reader.getFloat("z");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, double>::value>::type* = nullptr) {
        x = reader.getDouble("x");
        y = reader.getDouble("y");
        z = reader.getDouble("z");
      }
    };
    
    template <class T> VECTOR3<T> operator+(T scalar, const VECTOR3<T>& vec) {
      return VECTOR3<T>(scalar + vec.x, scalar + vec.y, scalar + vec.z);
    }
    template <class T> VECTOR3<T> operator-(T scalar, const VECTOR3<T>& vec) {
      return VECTOR3<T>(scalar - vec.x, scalar - vec.y, scalar - vec.z);
    }
    template <class T> VECTOR3<T> operator*(T scalar, const VECTOR3<T>& vec) {
      return VECTOR3<T>(scalar * vec.x, scalar * vec.y, scalar * vec.z);
    }
    template <class T> VECTOR3<T> operator/(T scalar, const VECTOR3<T>& vec) {
      return VECTOR3<T>(scalar / vec.x, scalar / vec.y, scalar / vec.z);
    }
    template <class T> VECTOR3<T> operator%(T scalar, const VECTOR3<T>& vec) {
      return VECTOR3<T>(scalar % vec.x, scalar % vec.y, scalar % vec.z);
    }
    
    // RedHat's patched gcc warns on the operator>> below if we don't explicitly
    // mark the symbol as hidden, despite -fvisibility settings.
#if defined(__GNUC__) && __GNUC__ >= 4
#define HIDDEN __attribute__((visibility("hidden")))
#else
#define HIDDEN /* nothing */
#endif
    
    template <class T = int32_t> class VECTOR4 {
      template <class U> HIDDEN friend std::istream& operator>>(std::istream&, VECTOR4<U>&);
      
    public:
      T x, y, z, w;
      
      VECTOR4<T>()
      : x(0)
      , y(0)
      , z(0)
      , w(0) {}
      template <class S> explicit VECTOR4<T>(const std::vector<S>& v, const T& defaultVal = T(0)) {
        x = T(v.size() > 0 ? v[0] : defaultVal);
        y = T(v.size() > 1 ? v[1] : defaultVal);
        z = T(v.size() > 2 ? v[2] : defaultVal);
        w = T(v.size() > 3 ? v[3] : defaultVal);
      }
      VECTOR4<T>(const VECTOR2<T>& other, const T _z, const T _w)
      : x(other.x)
      , y(other.y)
      , z(_z)
      , w(_w) {}
      VECTOR4<T>(const VECTOR3<T>& other, const T _w)
      : x(other.x)
      , y(other.y)
      , z(other.z)
      , w(_w) {}
      
      // We do not want the implicit copy c'tor to fulfill the std::is_trivially_copyable properties!
      // VECTOR4<T>(const VECTOR4<T> &other): x(other.x), y(other.y), z(other.z), w(other.w) {}
      template <class S>
      explicit VECTOR4<T>(const VECTOR4<S>& other)
      : x(T(other.x))
      , y(T(other.y))
      , z(T(other.z))
      , w(T(other.w)) {}
      VECTOR4<T>(const VECTOR3<T>& other)
      : x(other.x)
      , y(other.y)
      , z(other.z)
      , w(1) {}
      VECTOR4<T>(const T _x, const T _y, const T _z, const T _w)
      : x(_x)
      , y(_y)
      , z(_z)
      , w(_w) {}
      VECTOR4<T>(const T* vec)
      : x(vec[0])
      , y(vec[1])
      , z(vec[2])
      , w(vec[3]) {}
      
      bool operator==(const VECTOR4<T>& other) const { return (other.x == x && other.y == y && other.z == z && other.w == w); }
      bool operator!=(const VECTOR4<T>& other) const { return (other.x != x || other.y != y || other.z != z || other.w != w); }
      
      // binary operators with scalars
      VECTOR4<T> operator+(T scalar) const { return VECTOR4<T>(x + scalar, y + scalar, z + scalar, w + scalar); }
      VECTOR4<T> operator-(T scalar) const { return VECTOR4<T>(x - scalar, y - scalar, z - scalar, w - scalar); }
      VECTOR4<T> operator*(T scalar) const { return VECTOR4<T>(x * scalar, y * scalar, z * scalar, w * scalar); }
      VECTOR4<T> operator/(T scalar) const { return VECTOR4<T>(x / scalar, y / scalar, z / scalar, w / scalar); }
      VECTOR4<T> operator%(T scalar) const { return VECTOR4<T>(x % scalar, y % scalar, z % scalar, w % scalar); }
      
      // binary operators with vectors
      VECTOR4<T> operator+(const VECTOR4<T>& other) const { return VECTOR4<T>(x + other.x, y + other.y, z + other.z, w + other.w); }
      VECTOR4<T> operator-(const VECTOR4<T>& other) const { return VECTOR4<T>(x - other.x, y - other.y, z - other.z, w - other.w); }
      VECTOR4<T> operator*(const VECTOR4<T>& other) const { return VECTOR4<T>(x * other.x, y * other.y, z * other.z, w * other.w); }
      VECTOR4<T> operator/(const VECTOR4<T>& other) const { return VECTOR4<T>(x / other.x, y / other.y, z / other.z, w / other.w); }
      T operator^(const VECTOR4<T>& other) const { return T(x * other.x + y * other.y + z * other.z + w * other.w); } // dot product
      
      // binary operators with a matrix
      VECTOR4<T> operator*(const MATRIX4<T>& matrix) const {
        return VECTOR4<T>(x * matrix.m11 + y * matrix.m21 + z * matrix.m31 + w * matrix.m41,
                          x * matrix.m12 + y * matrix.m22 + z * matrix.m32 + w * matrix.m42,
                          x * matrix.m13 + y * matrix.m23 + z * matrix.m33 + w * matrix.m43,
                          x * matrix.m14 + y * matrix.m24 + z * matrix.m34 + w * matrix.m44);
      }
      
      // unary opartors
      VECTOR4<T> operator+() const { return *this; }
      VECTOR4<T> operator-() const { return *this * -1; }
      VECTOR4<T> operator~() const { return VECTOR4<T>(T(1) / x, T(1) / y, T(1) / z, T(1) / w); }
      
      T& operator[](size_t i) {
        assert(i <= 3);
        switch (i) {
          case 0:
            return this->x;
          case 1:
            return this->y;
          case 2:
            return this->z;
          default:
            return this->w;
        }
      }
      const T& operator[](size_t i) const {
        assert(i <= 3);
        switch (i) {
          case 0:
            return this->x;
          case 1:
            return this->y;
          case 2:
            return this->z;
          default:
            return this->w;
        }
      }
      
      // We do not want to implement the copy assignment operator to fulfill the std::is_trivially_copyable properties!
      // VECTOR4<T>& operator=(const VECTOR4<T>& other)  { x = other.x; y = other.y; z = other.z; w = other.w; return *this; }
      VECTOR4<T>& operator+=(const VECTOR4<T>& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        w += other.w;
        return *this;
      }
      VECTOR4<T>& operator-=(const VECTOR4<T>& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        w -= other.w;
        return *this;
      }
      VECTOR4<T>& operator*=(const VECTOR4<T>& other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
        w *= other.w;
        return *this;
      }
      VECTOR4<T>& operator/=(const VECTOR4<T>& other) {
        x /= other.x;
        y /= other.y;
        z /= other.z;
        w /= other.w;
        return *this;
      }
      VECTOR4<T>& operator=(const T& other) {
        x = other;
        y = other;
        z = other;
        w = other;
        return *this;
      }
      VECTOR4<T>& operator+=(const T& other) {
        x += other;
        y += other;
        z += other;
        w += other;
        return *this;
      }
      VECTOR4<T>& operator-=(const T& other) {
        x -= other;
        y -= other;
        z -= other;
        w -= other;
        return *this;
      }
      VECTOR4<T>& operator*=(const T& other) {
        x *= other;
        y *= other;
        z *= other;
        w *= other;
        return *this;
      }
      VECTOR4<T>& operator/=(const T& other) {
        x /= other;
        y /= other;
        z /= other;
        w /= other;
        return *this;
      }
      
      friend std::ostream& operator<<(std::ostream& os, const VECTOR4<T>& v) {
        os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
        return os;
      }
      
      
      T Max() const { return std::max<T>(std::max<T>(x, y), std::max<T>(z, w)); }
      T Min() const { return std::min<T>(std::min<T>(x, y), std::min<T>(z, w)); }
      
      VECTOR4<T> abs() const { return VECTOR4<T>(fabs(x), fabs(y), fabs(z), fabs(w)); }
      
      VECTOR4<T> makepow2() const {
        VECTOR4<T> vOut;
        vOut.x = T(1 << int(ceil(log(double(x)) / log(2.0))));
        vOut.y = T(1 << int(ceil(log(double(y)) / log(2.0))));
        vOut.z = T(1 << int(ceil(log(double(z)) / log(2.0))));
        vOut.w = T(1 << int(ceil(log(double(w)) / log(2.0))));
        return vOut;
      }
      
      VECTOR3<T> dehomo() const {
        VECTOR3<T> vOut;
        vOut.x = x / w;
        vOut.y = y / w;
        vOut.z = z / w;
        return vOut;
      }
      
      void StoreMin(const VECTOR4<T>& other) {
        x = std::min<T>(x, other.x);
        y = std::min<T>(y, other.y);
        z = std::min<T>(z, other.z);
        w = std::min<T>(w, other.w);
      }
      
      void StoreMax(const VECTOR4<T>& other) {
        x = std::max<T>(x, other.x);
        y = std::max<T>(y, other.y);
        z = std::max<T>(z, other.z);
        w = std::max<T>(w, other.w);
      }
      
      VECTOR2<T> xx() const { return VECTOR2<T>(x, x); }
      VECTOR2<T> xy() const { return VECTOR2<T>(x, y); }
      VECTOR2<T> xz() const { return VECTOR2<T>(x, z); }
      VECTOR2<T> yx() const { return VECTOR2<T>(y, x); }
      VECTOR2<T> yy() const { return VECTOR2<T>(y, y); }
      VECTOR2<T> yz() const { return VECTOR2<T>(y, z); }
      VECTOR2<T> zx() const { return VECTOR2<T>(z, x); }
      VECTOR2<T> zy() const { return VECTOR2<T>(z, y); }
      VECTOR2<T> zz() const { return VECTOR2<T>(z, z); }
      VECTOR2<T> zw() const { return VECTOR2<T>(z, w); }
      
      VECTOR3<T> xyz() const { return VECTOR3<T>(x, y, z); }
    };
    
    template <class T> std::istream& operator>>(std::istream& is, VECTOR4<T>& v4) {
      is >> v4[0];
      is >> v4[1];
      is >> v4[2];
      is >> v4[3];
      return is;
    }
    
    template <class T> VECTOR4<T> operator+(T scalar, const VECTOR4<T>& vec) {
      return VECTOR4<T>(scalar + vec.x, scalar + vec.y, scalar + vec.z, scalar + vec.w);
    }
    template <class T> VECTOR4<T> operator-(T scalar, const VECTOR4<T>& vec) {
      return VECTOR4<T>(scalar - vec.x, scalar - vec.y, scalar - vec.z, scalar - vec.w);
    }
    template <class T> VECTOR4<T> operator*(T scalar, const VECTOR4<T>& vec) {
      return VECTOR4<T>(scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w);
    }
    template <class T> VECTOR4<T> operator/(T scalar, const VECTOR4<T>& vec) {
      return VECTOR4<T>(scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w);
    }
    template <class T> VECTOR4<T> operator%(T scalar, const VECTOR4<T>& vec) {
      return VECTOR4<T>(scalar % vec.x, scalar % vec.y, scalar % vec.z, scalar % vec.w);
    }
    
    typedef VECTOR2<int32_t> Vec2i;
    typedef VECTOR3<int32_t> Vec3i;
    typedef VECTOR4<int32_t> Vec4i;
    
    typedef VECTOR2<uint32_t> Vec2ui;
    typedef VECTOR3<uint32_t> Vec3ui;
    typedef VECTOR4<uint32_t> Vec4ui;
    
    typedef VECTOR2<int8_t> Vec2i8;
    typedef VECTOR3<int8_t> Vec3i8;
    typedef VECTOR4<int8_t> Vec4i8;
    
    typedef VECTOR2<int16_t> Vec2i16;
    typedef VECTOR3<int16_t> Vec3i16;
    typedef VECTOR4<int16_t> Vec4i16;
    
    typedef VECTOR2<int32_t> Vec2i32;
    typedef VECTOR3<int32_t> Vec3i32;
    typedef VECTOR4<int32_t> Vec4i32;
    
    typedef VECTOR2<int64_t> Vec2i64;
    typedef VECTOR3<int64_t> Vec3i64;
    typedef VECTOR4<int64_t> Vec4i64;
    
    typedef VECTOR2<uint8_t> Vec2ui8;
    typedef VECTOR3<uint8_t> Vec3ui8;
    typedef VECTOR4<uint8_t> Vec4ui8;
    
    typedef VECTOR2<uint16_t> Vec2ui16;
    typedef VECTOR3<uint16_t> Vec3ui16;
    typedef VECTOR4<uint16_t> Vec4ui16;
    
    typedef VECTOR2<uint32_t> Vec2ui32;
    typedef VECTOR3<uint32_t> Vec3ui32;
    typedef VECTOR4<uint32_t> Vec4ui32;
    
    typedef VECTOR2<uint64_t> Vec2ui64;
    typedef VECTOR3<uint64_t> Vec3ui64;
    typedef VECTOR4<uint64_t> Vec4ui64;
    
    typedef VECTOR2<float> Vec2f;
    typedef VECTOR3<float> Vec3f;
    typedef VECTOR4<float> Vec4f;
    
    typedef VECTOR2<double> Vec2d;
    typedef VECTOR3<double> Vec3d;
    typedef VECTOR4<double> Vec4d;
    
    // old "ugly" deprecated typedefs
    typedef VECTOR2<int32_t> INTVECTOR2;
    typedef VECTOR3<int32_t> INTVECTOR3;
    typedef VECTOR4<int32_t> INTVECTOR4;
    
    typedef VECTOR2<uint32_t> UINTVECTOR2;
    typedef VECTOR3<uint32_t> UINTVECTOR3;
    typedef VECTOR4<uint32_t> UINTVECTOR4;
    
    typedef VECTOR2<uint16_t> USHORTVECTOR2;
    typedef VECTOR3<uint16_t> USHORTVECTOR3;
    typedef VECTOR4<uint16_t> USHORTVECTOR4;
    
    typedef VECTOR2<float> FLOATVECTOR2;
    typedef VECTOR3<float> FLOATVECTOR3;
    typedef VECTOR4<float> FLOATVECTOR4;
    
    typedef VECTOR2<double> DOUBLEVECTOR2;
    typedef VECTOR3<double> DOUBLEVECTOR3;
    typedef VECTOR4<double> DOUBLEVECTOR4;
    
    
    template <class T = int32_t> class MATRIX2 {
    public:
      union {
        struct {
          T m11, m12, m21, m22;
        };
        T array[4];
      };
      
      MATRIX2()
      : m11(1)
      , m12(0)
      , m21(0)
      , m22(1){};
      MATRIX2(const T* e)
      : m11(e[0])
      , m12(e[1])
      , m21(e[4])
      , m22(e[5]){};
      // We do not want the implicit copy c'tor to fullfill the std::is_trivially_copyable properties!
      // MATRIX2( const MATRIX2<T>& other ) : m11(other.m11), m12(other.m12),
      // m21(other.m21), m22(other.m22) {};
      MATRIX2(const MATRIX3<T>& other)
      : m11(other.m11)
      , m12(other.m12)
      , m21(other.m21)
      , m22(other.m22){};
      MATRIX2(const MATRIX4<T>& other)
      : m11(other.m11)
      , m12(other.m12)
      , m21(other.m21)
      , m22(other.m22){};
      MATRIX2(const VECTOR2<T>* rows)
      : m11(rows[0].x)
      , m12(rows[0].y)
      , m21(rows[1].x)
      , m22(rows[1].y){};
      MATRIX2(T _m11, T _m12, T _m21, T _m22)
      : m11(_m11)
      , m12(_m12)
      , m21(_m21)
      , m22(_m22){};
      
      bool operator==(const MATRIX2<T>& other) const {
        return (other.m11 == m11 && other.m12 == m12 && other.m21 == m21 && other.m22 == m22);
      }
      bool operator!=(const MATRIX2<T>& other) const {
        return (other.m11 != m11 || other.m12 != m12 || other.m21 != m21 || other.m22 != m22);
      }
      
      friend std::ostream& operator<<(std::ostream& os, const MATRIX2<T>& m) {
        os << "[" << m.m11 << ", " << m.m12 << "; " << m.m21 << ", " << m.m22 << "]";
        return os;
      }
      
      // binary operators with scalars
      MATRIX2<T> operator*(T scalar) const { return MATRIX2<T>(m11 * scalar, m12 * scalar, m21 * scalar, m22 * scalar); }
      MATRIX2<T> operator+(T scalar) const { return MATRIX2<T>(m11 + scalar, m12 + scalar, m21 + scalar, m22 + scalar); }
      MATRIX2<T> operator-(T scalar) const { return MATRIX2<T>(m11 - scalar, m12 - scalar, m21 - scalar, m22 - scalar); }
      MATRIX2<T> operator/(T scalar) const { return MATRIX2<T>(m11 / scalar, m12 / scalar, m21 / scalar, m22 / scalar); }
      
      
      // binary operators with matrices
      MATRIX2<T> operator*(const MATRIX4<T>& other) const {
        MATRIX2<T> result;
        
        for (int x = 0; x < 4; x += 2)
          for (int y = 0; y < 2; y++)
            result[x + y] = array[0 + x] * other.array[0 + y] + array[1 + x] * other.array[2 + y];
        
        return result;
      }
      
      // binary operators with vectors
      VECTOR2<T> operator*(const VECTOR2<T>& other) const { return VECTOR2<T>(other.x * m11 + other.y * m12, other.x * m22 + other.y * m22); }
      
      MATRIX2<T> inverse() const {
        T determinant = m11 * m22 - m12 * m21;
        return MATRIX2<T>(m22 / determinant, -m12 / determinant, -m21 / determinant, m11 / determinant);
      }
    };
    
    template <class T = int32_t> class MATRIX3 {
    public:
      union {
        struct {
          T m11, m12, m13, m21, m22, m23, m31, m32, m33;
        };
        T array[9];
      };
      
      MATRIX3()
      : m11(1)
      , m12(0)
      , m13(0)
      , m21(0)
      , m22(1)
      , m23(0)
      , m31(0)
      , m32(0)
      , m33(1){};
      MATRIX3(const T* e)
      : m11(e[0])
      , m12(e[1])
      , m13(e[2])
      , m21(e[3])
      , m22(e[4])
      , m23(e[5])
      , m31(e[6])
      , m32(e[7])
      , m33(e[8]){};
      // We do not want the implicit copy c'tor to fullfill the std::is_trivially_copyable properties!
      // MATRIX3( const MATRIX3<T>& other ) : m11(other.m11), m12(other.m12), m13(other.m13),
      // m21(other.m21), m22(other.m22), m23(other.m23),
      // m31(other.m31), m32(other.m32), m33(other.m33) {};
      MATRIX3(const MATRIX4<T>& other)
      : m11(other.m11)
      , m12(other.m12)
      , m13(other.m13)
      , m21(other.m21)
      , m22(other.m22)
      , m23(other.m23)
      , m31(other.m31)
      , m32(other.m32)
      , m33(other.m33){};
      MATRIX3(const VECTOR3<T>* rows)
      : m11(rows[0].x)
      , m12(rows[0].y)
      , m13(rows[0].z)
      , m21(rows[1].x)
      , m22(rows[1].y)
      , m23(rows[1].z)
      , m31(rows[2].x)
      , m32(rows[2].y)
      , m33(rows[2].z){};
      MATRIX3(T _m11, T _m12, T _m13, T _m21, T _m22, T _m23, T _m31, T _m32, T _m33)
      : m11(_m11)
      , m12(_m12)
      , m13(_m13)
      , m21(_m21)
      , m22(_m22)
      , m23(_m23)
      , m31(_m31)
      , m32(_m32)
      , m33(_m33){};
      
      bool operator==(const MATRIX3<T>& other) const {
        return (other.m11 == m11 && other.m12 == m12 && other.m13 == m13 && other.m21 == m21 && other.m22 == m22 && other.m23 == m23 &&
                other.m31 == m31 && other.m32 == m32 && other.m33 == m33);
      }
      bool operator!=(const MATRIX3<T>& other) const {
        return (other.m11 != m11 || other.m12 != m12 || other.m13 != m13 || other.m21 != m21 || other.m22 != m22 || other.m23 != m23 ||
                other.m31 != m31 || other.m32 != m32 || other.m33 != m33);
      }
      
      
      friend std::ostream& operator<<(std::ostream& os, const MATRIX3<T>& m) {
        os << "[" << m.m11 << ", " << m.m12 << ", " << m.m13 << "; " << m.m21 << ", " << m.m22 << ", " << m.m23 << "; " << m.m31 << ", "
        << m.m32 << ", " << m.m33 << "]";
        return os;
      }
      
      // binary operators with matrices
      MATRIX3<T> operator*(const MATRIX3<T>& other) const {
        MATRIX3<T> result;
        for (int x = 0; x < 9; x += 3)
          for (int y = 0; y < 3; y++)
            result[x + y] = array[1 + x] * other.array[0 + y] + array[2 + x] * other.array[3 + y] + array[3 + x] * other.array[6 + y];
        
        return result;
      }
      
      // binary operators with scalars
      MATRIX3<T> operator*(T scalar) const {
        return MATRIX3<T>(m11 * scalar, m12 * scalar, m13 * scalar, m21 * scalar, m22 * scalar, m23 * scalar, m31 * scalar, m32 * scalar,
                          m33 * scalar);
      }
      MATRIX3<T> operator+(T scalar) const {
        return MATRIX3<T>(m11 + scalar, m12 + scalar, m13 + scalar, m21 + scalar, m22 + scalar, m23 + scalar, m31 + scalar, m32 + scalar,
                          m33 + scalar);
      }
      MATRIX3<T> operator-(T scalar) const {
        return MATRIX3<T>(m11 - scalar, m12 - scalar, m13 - scalar, m21 - scalar, m22 - scalar, m23 - scalar, m31 - scalar, m32 - scalar,
                          m33 - scalar);
      }
      MATRIX3<T> operator/(T scalar) const {
        return MATRIX3<T>(m11 / scalar, m12 / scalar, m13 / scalar, m21 / scalar, m22 / scalar, m23 / scalar, m31 / scalar, m32 / scalar,
                          m33 / scalar);
      }
      
      // binary operators with vectors
      VECTOR3<T> operator*(const VECTOR3<T>& other) const {
        return VECTOR3<T>(other.x * m11 + other.y * m12 + other.z * m13, other.x * m21 + other.y * m22 + other.z * m23,
                          other.x * m31 + other.y * m32 + other.z * m33);
      }
      
      void Scaling(T x, T y, T z) {
        m11 = x;
        m12 = 0;
        m13 = 0;
        m21 = 0;
        m22 = y;
        m23 = 0;
        m31 = 0;
        m32 = 0;
        m33 = z;
      }
      
      void RotationX(double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        
        m11 = 1;
        m12 = 0;
        m13 = 0;
        m21 = 0;
        m22 = dCosAngle;
        m23 = dSinAngle;
        m31 = 0;
        m32 = -dSinAngle;
        m33 = dCosAngle;
      }
      
      void RotationY(double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        
        m11 = dCosAngle;
        m12 = 0;
        m13 = -dSinAngle;
        m21 = 0;
        m22 = 1;
        m23 = 0;
        m31 = dSinAngle;
        m32 = 0;
        m33 = dCosAngle;
      }
      
      void RotationZ(double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        
        m11 = dCosAngle;
        m12 = dSinAngle;
        m13 = 0;
        m21 = -dSinAngle;
        m22 = dCosAngle;
        m23 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 1;
      }
      
      void RotationAxis(const VECTOR3<T>& axis, double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        T dOneMinusCosAngle = 1 - dCosAngle;
        
        VECTOR3<T> sqrAxis = axis * axis;
        
        m11 = dCosAngle + dOneMinusCosAngle * sqrAxis.x;
        m12 = dOneMinusCosAngle * axis.x * axis.y - dSinAngle * axis.z;
        m13 = dOneMinusCosAngle * axis.x * axis.z + dSinAngle * axis.y;
        m21 = dOneMinusCosAngle * axis.x * axis.y + dSinAngle * axis.z;
        m22 = dCosAngle + dOneMinusCosAngle * sqrAxis.y;
        m23 = dOneMinusCosAngle * axis.y * axis.z - dSinAngle * axis.x;
        m31 = dOneMinusCosAngle * axis.x * axis.z - dSinAngle * axis.y;
        m32 = dOneMinusCosAngle * axis.y * axis.z + dSinAngle * axis.x;
        m33 = dCosAngle + dOneMinusCosAngle * sqrAxis.z;
      }
      
      MATRIX3<T> inverse() const {
        T determ = 1.0f / (array[0] * (array[4] * array[8] - array[5] * array[7]) - array[1] * (array[3] * array[8] - array[5] * array[6]) +
                           array[2] * (array[3] * array[7] - array[4] * array[6]));
        
        MATRIX3<T> result;
        
        result.array[0] = determ * (array[4] * array[8] - array[5] * array[7]);
        result.array[3] = determ * (array[2] * array[7] - array[1] * array[8]);
        result.array[6] = determ * (array[1] * array[5] - array[2] * array[4]);
        result.array[1] = determ * (array[5] * array[6] - array[3] * array[8]);
        result.array[4] = determ * (array[0] * array[8] - array[2] * array[6]);
        result.array[7] = determ * (array[2] * array[3] - array[0] * array[5]);
        result.array[2] = determ * (array[3] * array[7] - array[4] * array[6]);
        result.array[5] = determ * (array[1] * array[6] - array[0] * array[7]);
        result.array[8] = determ * (array[0] * array[4] - array[1] * array[3]);
        
        return result;
      }
    };
    
    template <class T = int32_t> class MATRIX4 : public trinity::ISerializable {
    public:
      union {
        struct {
          T m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44;
        };
        T array[16];
      };
      
      MATRIX4()
      : m11(1)
      , m12(0)
      , m13(0)
      , m14(0)
      , m21(0)
      , m22(1)
      , m23(0)
      , m24(0)
      , m31(0)
      , m32(0)
      , m33(1)
      , m34(0)
      , m41(0)
      , m42(0)
      , m43(0)
      , m44(1){};
      MATRIX4(const T* e)
      : m11(e[0])
      , m12(e[1])
      , m13(e[2])
      , m14(e[3])
      , m21(e[4])
      , m22(e[5])
      , m23(e[6])
      , m24(e[7])
      , m31(e[8])
      , m32(e[9])
      , m33(e[10])
      , m34(e[11])
      , m41(e[12])
      , m42(e[13])
      , m43(e[14])
      , m44(e[15]){};
      // We do not want the implicit copy c'tor to fullfill the std::is_trivially_copyable properties!
      // MATRIX4( const MATRIX4<T>& other ) : m11(other.m11), m12(other.m12), m13(other.m13), m14(other.m14),
      // m21(other.m21), m22(other.m22), m23(other.m23), m24(other.m24),
      // m31(other.m31), m32(other.m32), m33(other.m33), m34(other.m34),
      // m41(other.m41), m42(other.m42), m43(other.m43), m44(other.m44) {};
      MATRIX4(const MATRIX3<T>& other)
      : m11(other.m11)
      , m12(other.m12)
      , m13(other.m13)
      , m14(0)
      , m21(other.m21)
      , m22(other.m22)
      , m23(other.m23)
      , m24(0)
      , m31(other.m31)
      , m32(other.m32)
      , m33(other.m33)
      , m34(0)
      , m41(0)
      , m42(0)
      , m43(0)
      , m44(1){};
      MATRIX4(const VECTOR4<T>* rows)
      : m11(rows[0].x)
      , m12(rows[0].y)
      , m13(rows[0].z)
      , m14(rows[0].w)
      , m21(rows[1].x)
      , m22(rows[1].y)
      , m23(rows[1].z)
      , m24(rows[1].w)
      , m31(rows[2].x)
      , m32(rows[2].y)
      , m33(rows[2].z)
      , m34(rows[2].w)
      , m41(rows[3].x)
      , m42(rows[3].y)
      , m43(rows[3].z)
      , m44(rows[3].w){};
      MATRIX4(T _m11, T _m12, T _m13, T _m14, T _m21, T _m22, T _m23, T _m24, T _m31, T _m32, T _m33, T _m34, T _m41, T _m42, T _m43, T _m44)
      : m11(_m11)
      , m12(_m12)
      , m13(_m13)
      , m14(_m14)
      , m21(_m21)
      , m22(_m22)
      , m23(_m23)
      , m24(_m24)
      , m31(_m31)
      , m32(_m32)
      , m33(_m33)
      , m34(_m34)
      , m41(_m41)
      , m42(_m42)
      , m43(_m43)
      , m44(_m44){};
      
      bool operator==(const MATRIX4<T>& other) const {
        return (other.m11 == m11 && other.m12 == m12 && other.m13 == m13 && other.m14 == m14 && other.m21 == m21 && other.m22 == m22 &&
                other.m23 == m23 && other.m24 == m24 && other.m31 == m31 && other.m32 == m32 && other.m33 == m33 && other.m34 == m34 &&
                other.m41 == m41 && other.m42 == m42 && other.m43 == m43 && other.m44 == m44);
      }
      bool operator!=(const MATRIX4<T>& other) const {
        return (other.m11 != m11 || other.m12 != m12 || other.m13 != m13 || other.m14 != m14 || other.m21 != m21 || other.m22 != m22 ||
                other.m23 != m23 || other.m24 != m24 || other.m31 != m31 || other.m32 != m32 || other.m33 != m33 || other.m34 != m34 ||
                other.m41 != m41 || other.m42 != m42 || other.m43 != m43 || other.m44 != m44);
      }
      
      friend std::ostream& operator<<(std::ostream& os, const MATRIX4<T>& m) {
        os << "[" << m.m11 << ", " << m.m12 << ", " << m.m13 << ", " << m.m14 << "; " << m.m21 << ", " << m.m22 << ", " << m.m23 << ", "
        << m.m24 << "; " << m.m31 << ", " << m.m32 << ", " << m.m33 << ", " << m.m34 << "; " << m.m41 << ", " << m.m42 << ", " << m.m43
        << ", " << m.m44 << "]";
        return os;
      }
      
      operator T*(void) { return &m11; }
      const T* operator*(void) const { return &m11; }
      T* operator*(void) { return &m11; }
      
      // binary operators with scalars
      MATRIX4<T> operator*(T scalar) const {
        return MATRIX4<T>(m11 * scalar, m12 * scalar, m13 * scalar, m14 * scalar, m21 * scalar, m22 * scalar, m23 * scalar, m24 * scalar,
                          m31 * scalar, m32 * scalar, m33 * scalar, m34 * scalar, m41 * scalar, m42 * scalar, m43 * scalar, m44 * scalar);
      }
      MATRIX4<T> operator+(T scalar) const {
        return MATRIX4<T>(m11 + scalar, m12 + scalar, m13 + scalar, m14 + scalar, m21 + scalar, m22 + scalar, m23 + scalar, m24 + scalar,
                          m31 + scalar, m32 + scalar, m33 + scalar, m34 + scalar, m41 + scalar, m42 + scalar, m43 + scalar, m44 + scalar);
      }
      MATRIX4<T> operator-(T scalar) const {
        return MATRIX4<T>(m11 - scalar, m12 - scalar, m13 - scalar, m14 - scalar, m21 - scalar, m22 - scalar, m23 - scalar, m24 - scalar,
                          m31 - scalar, m32 - scalar, m33 - scalar, m34 - scalar, m41 - scalar, m42 - scalar, m43 - scalar, m44 - scalar);
      }
      MATRIX4<T> operator/(T scalar) const {
        return MATRIX4<T>(m11 / scalar, m12 / scalar, m13 / scalar, m14 / scalar, m21 / scalar, m22 / scalar, m23 / scalar, m24 / scalar,
                          m31 / scalar, m32 / scalar, m33 / scalar, m34 / scalar, m41 / scalar, m42 / scalar, m43 / scalar, m44 / scalar);
      }
      
      
      // binary operators with matrices
      MATRIX4<T> operator*(const MATRIX4<T>& other) const {
        MATRIX4<T> result;
        for (int x = 0; x < 16; x += 4)
          for (int y = 0; y < 4; y++)
            result[x + y] = array[0 + x] * other.array[0 + y] + array[1 + x] * other.array[4 + y] + array[2 + x] * other.array[8 + y] +
            array[3 + x] * other.array[12 + y];
        
        return result;
      }
      
      // binary operators with vectors
      VECTOR4<T> operator*(const VECTOR4<T>& other) const {
        return VECTOR4<T>(
                          other.x * m11 + other.y * m12 + other.z * m13 + other.w * m14, other.x * m21 + other.y * m22 + other.z * m23 + other.w * m24,
                          other.x * m31 + other.y * m32 + other.z * m33 + other.w * m34, other.x * m41 + other.y * m42 + other.z * m43 + other.w * m44);
      }
      
      VECTOR3<T> operator*(const VECTOR3<T>& other) const {
        return VECTOR3<T>(other.x * m11 + other.y * m12 + other.z * m13, other.x * m21 + other.y * m22 + other.z * m23,
                          other.x * m31 + other.y * m32 + other.z * m33);
      }
      
      void Translation(VECTOR3<T> trans) { Translation(trans.x, trans.y, trans.z); }
      
      void Scaling(VECTOR3<T> scale) { Scaling(scale.x, scale.y, scale.z); }
      
      void Translation(T x, T y, T z) {
        m11 = 1;
        m12 = 0;
        m13 = 0;
        m14 = 0;
        m21 = 0;
        m22 = 1;
        m23 = 0;
        m24 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 1;
        m34 = 0;
        m41 = x;
        m42 = y;
        m43 = z;
        m44 = 1;
      }
      
      void Scaling(T x, T y, T z) {
        m11 = x;
        m12 = 0;
        m13 = 0;
        m14 = 0;
        m21 = 0;
        m22 = y;
        m23 = 0;
        m24 = 0;
        m31 = 0;
        m32 = 0;
        m33 = z;
        m34 = 0;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 1;
      }
      
      void RotationX(double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        
        m11 = 1;
        m12 = 0;
        m13 = 0;
        m14 = 0;
        m21 = 0;
        m22 = dCosAngle;
        m23 = dSinAngle;
        m24 = 0;
        m31 = 0;
        m32 = -dSinAngle;
        m33 = dCosAngle;
        m34 = 0;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 1;
      }
      
      void RotationY(double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        
        m11 = dCosAngle;
        m12 = 0;
        m13 = -dSinAngle;
        m14 = 0;
        m21 = 0;
        m22 = 1;
        m23 = 0;
        m24 = 0;
        m31 = dSinAngle;
        m32 = 0;
        m33 = dCosAngle;
        m34 = 0;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 1;
      }
      
      void RotationZ(double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        
        m11 = dCosAngle;
        m12 = dSinAngle;
        m13 = 0;
        m21 = -dSinAngle;
        m22 = dCosAngle;
        m23 = 0;
        m31 = 0;
        m32 = 0;
        m33 = 1;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 1;
      }
      
      void RotationAxis(const VECTOR3<T>& axis, double angle) {
        T dCosAngle = T(cos(angle));
        T dSinAngle = T(sin(angle));
        T dOneMinusCosAngle = 1 - dCosAngle;
        
        VECTOR3<T> sqrAxis = axis * axis;
        
        m11 = dCosAngle + dOneMinusCosAngle * sqrAxis.x;
        m12 = dOneMinusCosAngle * axis.x * axis.y - dSinAngle * axis.z;
        m13 = dOneMinusCosAngle * axis.x * axis.z + dSinAngle * axis.y;
        m14 = 0;
        m21 = dOneMinusCosAngle * axis.x * axis.y + dSinAngle * axis.z;
        m22 = dCosAngle + dOneMinusCosAngle * sqrAxis.y;
        m23 = dOneMinusCosAngle * axis.y * axis.z - dSinAngle * axis.x;
        m24 = 0;
        m31 = dOneMinusCosAngle * axis.x * axis.z - dSinAngle * axis.y;
        m32 = dOneMinusCosAngle * axis.y * axis.z + dSinAngle * axis.x;
        m33 = dCosAngle + dOneMinusCosAngle * sqrAxis.z;
        m34 = 0;
        m41 = 0;
        m42 = 0;
        m43 = 0;
        m44 = 1;
      }
      
      MATRIX4<T> Transpose() const {
        MATRIX4<T> result;
        
        result.m41 = m14;
        result.m31 = m13;
        result.m42 = m24;
        result.m21 = m12;
        result.m32 = m23;
        result.m43 = m34;
        result.m11 = m11;
        result.m22 = m22;
        result.m33 = m33;
        result.m44 = m44;
        result.m12 = m21;
        result.m23 = m32;
        result.m34 = m43;
        result.m13 = m31;
        result.m24 = m42;
        result.m14 = m41;
        
        return result;
      }
      
      
      MATRIX4<T> inverse() const {
        
        MATRIX4<T> result;
        
        T Q = m21 * (m34 * (m12 * m43 - m13 * m42) + m14 * (-m32 * m43 + m42 * m33) + m44 * (m13 * m32 - m12 * m33)) +
        m24 * (m11 * (m32 * m43 - m42 * m33) + m13 * (-m41 * m32 + m31 * m42) + m12 * (-m31 * m43 + m41 * m33)) +
        m44 * (m22 * (-m31 * m13 + m11 * m33) + m23 * (-m11 * m32 + m12 * m31)) +
        m34 * (m11 * (-m22 * m43 + m23 * m42) + m41 * (m13 * m22 - m23 * m12)) +
        m14 * (m23 * (m32 * m41 - m42 * m31) + m22 * (m31 * m43 - m41 * m33));
        
        result.m11 = (m24 * m32 * m43 + m44 * m22 * m33 - m44 * m23 * m32 - m34 * m22 * m43 - m24 * m42 * m33 + m34 * m23 * m42) / Q;
        result.m21 = -(m21 * m44 * m33 - m21 * m34 * m43 - m44 * m23 * m31 + m34 * m23 * m41 + m24 * m31 * m43 - m24 * m41 * m33) / Q;
        result.m31 = (-m21 * m34 * m42 + m21 * m44 * m32 - m44 * m31 * m22 - m24 * m41 * m32 + m34 * m41 * m22 + m24 * m31 * m42) / Q;
        result.m41 = -(m21 * m32 * m43 - m21 * m42 * m33 + m41 * m22 * m33 - m32 * m23 * m41 - m31 * m22 * m43 + m42 * m23 * m31) / Q;
        /// 2
        result.m12 = (-m12 * m44 * m33 + m12 * m34 * m43 - m34 * m13 * m42 - m14 * m32 * m43 + m44 * m13 * m32 + m14 * m42 * m33) / Q;
        
        result.m22 = (-m44 * m13 * m31 + m44 * m11 * m33 - m34 * m11 * m43 - m14 * m41 * m33 + m34 * m13 * m41 + m14 * m31 * m43) / Q;
        
        result.m32 = -(-m12 * m44 * m31 + m12 * m34 * m41 + m44 * m11 * m32 - m14 * m32 * m41 + m14 * m42 * m31 - m34 * m11 * m42) / Q;
        
        result.m42 = (-m12 * m31 * m43 + m12 * m41 * m33 + m11 * m32 * m43 - m11 * m42 * m33 - m41 * m13 * m32 + m31 * m13 * m42) / Q;
        /// 3
        result.m13 = -(m44 * m13 * m22 - m24 * m13 * m42 - m14 * m22 * m43 + m12 * m24 * m43 - m12 * m44 * m23 + m14 * m42 * m23) / Q;
        
        result.m23 = (-m21 * m14 * m43 + m21 * m44 * m13 + m24 * m11 * m43 - m44 * m23 * m11 - m24 * m41 * m13 + m14 * m23 * m41) / Q;
        
        result.m33 = -(-m44 * m11 * m22 + m44 * m12 * m21 + m14 * m41 * m22 + m24 * m11 * m42 - m24 * m12 * m41 - m14 * m21 * m42) / Q;
        
        result.m43 = -(m43 * m11 * m22 - m43 * m12 * m21 - m13 * m41 * m22 - m23 * m11 * m42 + m23 * m12 * m41 + m13 * m21 * m42) / Q;
        /// 4
        result.m14 = (-m12 * m34 * m23 + m12 * m24 * m33 - m24 * m13 * m32 - m14 * m22 * m33 + m34 * m13 * m22 + m14 * m32 * m23) / Q;
        
        result.m24 = -(-m21 * m14 * m33 + m21 * m34 * m13 + m24 * m11 * m33 - m34 * m23 * m11 + m14 * m23 * m31 - m24 * m31 * m13) / Q;
        
        result.m34 = (-m34 * m11 * m22 + m34 * m12 * m21 + m14 * m31 * m22 + m24 * m11 * m32 - m24 * m12 * m31 - m14 * m21 * m32) / Q;
        
        result.m44 = (m33 * m11 * m22 - m33 * m12 * m21 - m13 * m31 * m22 - m23 * m11 * m32 + m23 * m12 * m31 + m13 * m21 * m32) / Q;
        return result;
      }
      
      MATRIX4<T> HandnessConvered() const {
        MATRIX4<T> M(T(1), T(0), T(0), T(0), T(0), T(1), T(0), T(0), T(0), T(0), T(-1), T(0), T(0), T(0), T(0), T(1));
        return M * (*this) * M;
      }
      
      
      static void BuildStereoLookAtAndProjection(const VECTOR3<T> vEye, const VECTOR3<T> vAt, const VECTOR3<T> vUp, T fFOVY, T fAspect,
                                                 T fZNear, T fZFar, T fFocalLength, T fEyeDist, int iEyeID, MATRIX4<T>& mView,
                                                 MATRIX4<T>& mProj) {
        
        T radians = T(3.14159265358979323846 / 180.0) * fFOVY / T(2);
        T wd2 = fZNear * T(tan(radians));
        T nfdl = fZNear / fFocalLength;
        T fShift = fEyeDist / 2 * nfdl;
        T left = -fAspect * wd2 + T(iEyeID) * fShift;
        T right = fAspect * wd2 + T(iEyeID) * fShift;
        T top = wd2;
        T bottom = -wd2;
        
        // projection matrix
        mProj.MatrixPerspectiveOffCenter(left, right, bottom, top, fZNear, fZFar);
        
        // view matrix
        mView.BuildLookAt(vEye, vAt, vUp);
        MATRIX4<T> mTranslate;
        mTranslate.Translation(fEyeDist / 2 * T(iEyeID), 0.0f, 0.0f);
        mView = mTranslate * mView;
      }
      
      
      static void BuildStereoLookAtAndProjection(const VECTOR3<T> vEye, const VECTOR3<T> vAt, const VECTOR3<T> vUp, T fFOVY, T fAspect,
                                                 T fZNear, T fZFar, T fFocalLength, T fEyeDist, MATRIX4<T>& mViewLeft, MATRIX4<T>& mViewRight,
                                                 MATRIX4<T>& mProjLeft, MATRIX4<T>& mProjRight) {
        
        T radians = T(3.14159265358979323846 / 180.0) * fFOVY / 2;
        T wd2 = fZNear * T(tan(radians));
        T nfdl = fZNear / fFocalLength;
        T fShift = fEyeDist * nfdl;
        T left = -fAspect * wd2 - fShift;
        T right = fAspect * wd2 - fShift;
        T top = wd2;
        T bottom = -wd2;
        
        // projection matrices
        mProjLeft.MatrixPerspectiveOffCenter(left, right, bottom, top, fZNear, fZFar);
        left = -fAspect * wd2 + fShift;
        right = fAspect * wd2 + fShift;
        mProjRight.MatrixPerspectiveOffCenter(left, right, bottom, top, fZNear, fZFar);
        
        // view matrices
        mViewLeft.BuildLookAt(vEye, vAt, vUp);
        mViewRight.BuildLookAt(vEye, vAt, vUp);
        
        // eye translation
        MATRIX4<T> mTranslate;
        
        mTranslate.Translation(-fEyeDist / 2, 0.0f, 0.0f);
        mViewLeft = mTranslate * mViewLeft;
        
        mTranslate.Translation(fEyeDist / 2, 0.0f, 0.0f);
        mViewRight = mTranslate * mViewRight;
      }
      
      void BuildLookAt(const VECTOR3<T> vEye, const VECTOR3<T> vAt, const VECTOR3<T> vUp) {
        VECTOR3<T> F = vAt - vEye;
        VECTOR3<T> U = vUp;
        VECTOR3<T> S = F % U;
        U = S % F;
        
        F.normalize();
        U.normalize();
        S.normalize();
        
        array[0] = S[0];
        array[4] = S[1];
        array[8] = S[2];
        array[12] = -(S ^ vEye);
        array[1] = U[0];
        array[5] = U[1];
        array[9] = U[2];
        array[13] = -(U ^ vEye);
        array[2] = -F[0];
        array[6] = -F[1];
        array[10] = -F[2];
        array[14] = (F ^ vEye);
        array[3] = T(0);
        array[7] = T(0);
        array[11] = T(0);
        array[15] = T(1);
      }
      
      void Perspective(T fovy, T aspect, T n, T f) {
        // deg 2 rad
        fovy = fovy * T(3.14159265358979323846 / 180.0);
        
        T cotan = T(1.0 / tan(double(fovy) / 2.0));
        
        array[0] = cotan / aspect;
        array[4] = T(0);
        array[8] = T(0);
        array[12] = T(0);
        array[1] = T(0);
        array[5] = cotan;
        array[9] = T(0);
        array[13] = T(0);
        array[2] = T(0);
        array[6] = T(0);
        array[10] = -(f + n) / (f - n);
        array[14] = T(-2) * (f * n) / (f - n);
        array[3] = T(0);
        array[7] = T(0);
        array[11] = T(-1);
        array[15] = T(0);
      }
      
      void Ortho(T left, T right, T bottom, T top, T znear, T zfar) {
        array[0] = T(2) / (right - left);
        array[4] = T(0);
        array[8] = T(0);
        array[12] = -(right + left) / (right - left);
        array[1] = T(0);
        array[5] = T(2) / (top - bottom);
        array[9] = T(0);
        array[13] = -(top + bottom) / (top - bottom);
        array[2] = T(0);
        array[6] = T(0);
        array[10] = -T(2) / (zfar - znear);
        array[14] = -(zfar + znear) / (zfar - znear);
        array[3] = T(0);
        array[7] = T(0);
        array[11] = T(0);
        array[15] = T(1);
      }
      
      void MatrixPerspectiveOffCenter(T left, T right, T bottom, T top, T n, T f) {
        array[0] = T(2) * n / (right - left);
        array[4] = T(0);
        array[8] = (right + left) / (right - left);
        array[12] = T(0);
        array[1] = T(0);
        array[5] = T(2) * n / (top - bottom);
        array[9] = (top + bottom) / (top - bottom);
        array[13] = T(0);
        array[2] = T(0);
        array[6] = T(0);
        array[10] = -(f + n) / (f - n);
        array[14] = T(-2) * (f * n) / (f - n);
        array[3] = T(0);
        array[7] = T(0);
        array[11] = T(-1);
        array[15] = T(0);
      }
      
      std::unique_ptr<trinity::ISerializable> clone() const override { return std::unique_ptr<ISerializable>(new MATRIX4(*this)); }
      void serialize(trinity::ISerialWriter& writer) const override { serializeImpl(writer); }
      void deserialize(const trinity::ISerialReader& reader) override { deserializeImpl(reader); }
      
    private:
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, double>::value>::type* = nullptr) const {
        writer.appendDouble("m11", m11);
        writer.appendDouble("m12", m12);
        writer.appendDouble("m13", m13);
        writer.appendDouble("m14", m14);
        
        writer.appendDouble("m21", m21);
        writer.appendDouble("m22", m22);
        writer.appendDouble("m23", m23);
        writer.appendDouble("m24", m24);
        
        writer.appendDouble("m31", m31);
        writer.appendDouble("m32", m32);
        writer.appendDouble("m33", m33);
        writer.appendDouble("m34", m34);
        
        writer.appendDouble("m41", m41);
        writer.appendDouble("m42", m42);
        writer.appendDouble("m43", m43);
        writer.appendDouble("m44", m44);
      }
      
      template <typename U = T>
      void serializeImpl(trinity::ISerialWriter& writer, typename std::enable_if<std::is_same<U, float>::value>::type* = nullptr) const {
        writer.appendFloat("m11", m11);
        writer.appendFloat("m12", m12);
        writer.appendFloat("m13", m13);
        writer.appendFloat("m14", m14);
        
        writer.appendFloat("m21", m21);
        writer.appendFloat("m22", m22);
        writer.appendFloat("m23", m23);
        writer.appendFloat("m24", m24);
        
        writer.appendFloat("m31", m31);
        writer.appendFloat("m32", m32);
        writer.appendFloat("m33", m33);
        writer.appendFloat("m34", m34);
        
        writer.appendFloat("m41", m41);
        writer.appendFloat("m42", m42);
        writer.appendFloat("m43", m43);
        writer.appendFloat("m44", m44);
      }
      
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, double>::value>::type* = nullptr) {
        m11 = reader.getDouble("m11");
        m12 = reader.getDouble("m12");
        m13 = reader.getDouble("m13");
        m14 = reader.getDouble("m14");
        
        m21 = reader.getDouble("m21");
        m22 = reader.getDouble("m22");
        m23 = reader.getDouble("m23");
        m24 = reader.getDouble("m24");
        
        m31 = reader.getDouble("m31");
        m32 = reader.getDouble("m32");
        m33 = reader.getDouble("m33");
        m34 = reader.getDouble("m34");
        
        m41 = reader.getDouble("m41");
        m42 = reader.getDouble("m42");
        m43 = reader.getDouble("m43");
        m44 = reader.getDouble("m44");
      }
      
      template <typename U = T>
      void deserializeImpl(const trinity::ISerialReader& reader, typename std::enable_if<std::is_same<U, float>::value>::type* = nullptr) {
        m11 = reader.getFloat("m11");
        m12 = reader.getFloat("m12");
        m13 = reader.getFloat("m13");
        m14 = reader.getFloat("m14");
        
        m21 = reader.getFloat("m21");
        m22 = reader.getFloat("m22");
        m23 = reader.getFloat("m23");
        m24 = reader.getFloat("m24");
        
        m31 = reader.getFloat("m31");
        m32 = reader.getFloat("m32");
        m33 = reader.getFloat("m33");
        m34 = reader.getFloat("m34");
        
        m41 = reader.getFloat("m41");
        m42 = reader.getFloat("m42");
        m43 = reader.getFloat("m43");
        m44 = reader.getFloat("m44");
      }
    };
    
    typedef MATRIX2<int32_t> Mat2i;
    typedef MATRIX3<int32_t> Mat3i;
    typedef MATRIX4<int32_t> Mat4i;
    
    typedef MATRIX2<float> Mat2f;
    typedef MATRIX3<float> Mat3f;
    typedef MATRIX4<float> Mat4f;
    
    typedef MATRIX2<double> Mat2d;
    typedef MATRIX3<double> Mat3d;
    typedef MATRIX4<double> Mat4d;
    
    template <class T> class QUATERNION4 {
    public:
      float x, y, z, w;
      
      QUATERNION4()
      : x(0)
      , y(0)
      , z(0)
      , w(0) {}
      QUATERNION4(T _x, T _y, T _z, T _w)
      : x(_x)
      , y(_y)
      , z(_z)
      , w(_w) {}
      QUATERNION4(const T& other)
      : x(other.x)
      , y(other.y)
      , z(other.z)
      , w(other.w) {}
      
      MATRIX4<T> ComputeRotation() {
        float n, s;
        float xs, ys, zs;
        float wx, wy, wz;
        float xx, xy, xz;
        float yy, yz, zz;
        
        n = (x * x) + (y * y) + (z * z) + (w * w);
        s = (n > 0.0f) ? (2.0f / n) : 0.0f;
        
        xs = x * s;
        ys = y * s;
        zs = z * s;
        wx = w * xs;
        wy = w * ys;
        wz = w * zs;
        xx = x * xs;
        xy = x * ys;
        xz = x * zs;
        yy = y * ys;
        yz = y * zs;
        zz = z * zs;
        
        return MATRIX4<T>(T(1.0f - (yy + zz)), T(xy - wz), T(xz + wy), T(0), T(xy + wz), T(1.0f - (xx + zz)), T(yz - wx), T(0), T(xz - wy),
                          T(yz + wx), T(1.0f - (xx + yy)), T(0), T(0), T(0), T(0), T(1));
      }
      
      bool operator==(const QUATERNION4<T>& other) const { return (other.x == x && other.y == y && other.z == z && other.w == w); }
      bool operator!=(const QUATERNION4<T>& other) const { return (other.x != x || other.y != y || other.z != z || other.w != w); }
      
      // binary operators with other quaternions
      QUATERNION4<T> operator+(const QUATERNION4<T>& other) const {
        return QUATERNION4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
      }
      QUATERNION4<T> operator-(const QUATERNION4<T>& other) const {
        return QUATERNION4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
      }
      QUATERNION4<T> operator*(const QUATERNION4<T>& other) const {
        VECTOR3<T> v1(x, y, z);
        VECTOR3<T> v2(other.x, other.y, other.z);
        
        T _w = w * other.w - (v1 ^ v2);
        VECTOR3<T> _v = (v2 * w) + (v1 * other.w) + (v1 % v2);
        
        return QUATERNION4<T>(_v.x, _v.y, _v.z, _w);
      }
      
      
      // binary operator with scalars
      QUATERNION4<T> operator*(const T other) const { return QUATERNION4<T>(x * other, y * other, z * other, w * other); }
      
      QUATERNION4<T>& operator=(const QUATERNION4<T>& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
      }
      QUATERNION4<T>& operator+=(const T& other) {
        x += other;
        y += other;
        z += other;
        w += other;
        return *this;
      }
      QUATERNION4<T>& operator-=(const T& other) {
        x -= other;
        y -= other;
        z -= other;
        w -= other;
        return *this;
      }
    };
    
    typedef QUATERNION4<float> FLOATQUATERNION4;
    typedef QUATERNION4<double> DOUBLEQUATERNION4;
    
    
    /// Tests to see if the two values are basically the same.
    template <class T> static bool EpsilonEqual(T a, T b) {
      /// @todo FIXME provide specializations for types; currently this would only
      ///       be correct if instantiated as a float!
      return fabs(a - b) <= std::numeric_limits<T>::epsilon();
    }
    
    /// a PLANE is a VECTOR4 which is always normalized.
    template <class T> class PLANE : public VECTOR4<T> {
    public:
      PLANE<T>()
      : VECTOR4<T>(0, 0, 0, 0) {}
      // plane from paramters (usually all 4 are given)
      template <class S> explicit PLANE<T>(const std::vector<S>& v) {
        this->x = T(v.size() > 0 ? v[0] : 0);
        this->y = T(v.size() > 1 ? v[1] : 0);
        this->z = T(v.size() > 2 ? v[2] : 0);
        this->w = T(v.size() > 3 ? v[3] : 0);
      }
      // plane from points
      template <class S> explicit PLANE<T>(const VECTOR3<S>& v0, const VECTOR3<S>& v1, const VECTOR3<S>& v2) {
        this->x = T(v0.y) * (T(v1.z) - T(v2.z)) + T(v1.y) * (T(v2.z) - T(v0.z)) + T(v2.y) * (T(v0.z) - T(v1.z));
        this->y = T(v0.z) * (T(v1.x) - T(v2.x)) + T(v1.z) * (T(v2.x) - T(v0.x)) + T(v2.z) * (T(v0.x) - T(v1.x));
        this->z = T(v0.x) * (T(v1.y) - T(v2.y)) + T(v1.x) * (T(v2.y) - T(v0.y)) + T(v2.x) * (T(v0.y) - T(v1.y));
        this->w = -(T(v0.x) * (T(v1.y) * T(v2.z) - T(v2.y) * T(v1.z)) + T(v1.x) * (T(v2.y) * T(v0.z) - T(v0.y) * T(v2.z)) +
                    T(v2.x) * (T(v0.y) * T(v1.z) - T(v1.y) * T(v0.z)));
      }
      PLANE<T>(const VECTOR2<T>& other, const T _z, const T _w)
      : VECTOR4<T>(other, _z, _w) {}
      PLANE<T>(const VECTOR3<T>& other, const T _w = 1)
      : VECTOR4<T>(other, _w) {}
      PLANE<T>(const VECTOR4<T>& other)
      : VECTOR4<T>(other) {}
      
      template <class S>
      explicit PLANE<T>(const PLANE<S>& other)
      : VECTOR4<T>(other) {}
      
      PLANE<T>(const T _x, const T _y, const T _z, const T _w)
      : VECTOR4<T>(_x, _y, _z, _w) {}
      PLANE<T>(const T* vec)
      : VECTOR4<T>(vec) {}
      
      /// @return true if the given point is clipped by this plane.
      bool clip(VECTOR3<T> point) const { return ((Vec4f::xyz() ^ point) + this->w >= 0); }
      
      /// Transform the plane by the given matrix.
      void transform(const MATRIX4<T>& m) {
        Mat4f mIT(m.inverse());
        mIT = mIT.Transpose();
        transformIT(mIT);
      }
      
      /// Transform the plane by the inverse transpose of the given matrix.
      void transformIT(const MATRIX4<T>& M) {
        (*this) = Vec4f::operator*(M);
        normalize();
      }
      
      void normalize() {
        const T x = this->x;
        const T y = this->y;
        const T z = this->z;
        T length = sqrt(x * x + y * y + z * z);
        (*this) /= length;
      }
      T d() const { return this->w; }
      VECTOR3<T> normal() { return this->xyz(); }
      
      // binary operators with a matrix
      PLANE<T> operator*(const MATRIX4<T>& matrix) const {
        PLANE<T> tmp(*this);
        tmp.transform(matrix);
        return tmp;
      }
      
      /// Determine the intersection point of the plane and a line `ab'.
      /// @return whether or not the two intersect.  If false, `hit' will not be
      ///         valid.
      bool intersect(const VECTOR3<T>& a, const VECTOR3<T>& b, VECTOR3<T>& hit) const {
        T t;
        bool bIntersect = intersect(a, b, t);
        if (!bIntersect)
          return false;
        hit = a + (t * (b - a));
        return true;
      }
      
      /// Determine the parametric intersection point of the plane and a line `ab'.
      /// @return whether or not the two intersect.  If false, `hit' will not be
      ///         valid.
      bool intersect(const VECTOR3<T>& a, const VECTOR3<T>& b, T& hit) const {
        const T denom = (*this) ^ (a - b);
        if (EpsilonEqual(denom, 0.0f)) {
          return false;
        }
        hit = (((*this) ^ a) + this->d()) / denom;
        return true;
      }
    };
    
    typedef PLANE<float> FLOATPLANE;
    typedef PLANE<double> DOUBLEPLANE;
  }
}