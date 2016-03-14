#pragma once

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"
#include "commands/SerializerFactory.h"

#include "mocca/base/Memory.h"

#include <vector>

namespace trinity {

using DefaultSerializerFactory = JsonSerializerFactory;

template <typename ObjType> mocca::ByteArray toByteArray(const ObjType& obj, const ISerializerFactory& factory = ISerializerFactory::defaultFactory()) {
    auto writer = factory.createWriter();
    serializeRoot(obj, *writer);
    return writer->write();
}

template <typename ObjType> void serializeRoot(const ObjType& obj, ISerialWriter& writer) {
    serializeObjectImpl(obj, writer);
}

template <typename ObjType> void serializeObject(const std::string& key, const ObjType& obj, ISerialWriter& writer) {
    writer.push(key);
    serializeObjectImpl(obj, writer);
    writer.pop();
}

void serializeInt(const std::string& key, int32_t obj, ISerialWriter& writer);
void serializeFloat(const std::string& key, float obj, ISerialWriter& writer);
void serializeString(const std::string& key, const std::string& obj, ISerialWriter& writer);

//template <typename ObjType> mocca::ByteArray serialize(const std::vector<ObjType>& vec, const ISerializerFactory& factory = ISerializerFactory::defaultFactory()) {
//    auto writer = factory.createWriter();
//    writer->append("size", static_cast<int32_t>(vec.size()));
//    for (const auto& obj : vec) {
//        serializeImpl(obj, *writer);
//    }
//    return writer->write();
//}

template <typename ObjType> ObjType fromByteArray(mocca::ByteArray& byteArray, const ISerializerFactory& factory = ISerializerFactory::defaultFactory()) {
    auto reader = factory.createReader(byteArray);
    ObjType obj;
    deserializeRoot(obj, *reader);
    return obj;
}

template <typename ObjType> void deserializeRoot(ObjType& obj, ISerialReader& reader) {
    deserializeObjectImpl(obj, reader);
}

template <typename ObjType> void deserializeObject(const std::string & key,  ObjType& obj, ISerialReader& reader) {
    reader.push(key);
    deserializeObjectImpl(obj, reader);
    reader.pop();
}

void deserializeInt(const std::string& key, int32_t& obj, ISerialReader& reader);
void deserializeFloat(const std::string& key, float& obj, ISerialReader& reader);
void deserializeString(const std::string& key, std::string& obj, ISerialReader& reader);

//template <typename ObjType> std::vector<ObjType> deserializeVec(mocca::ByteArray& byteArray, const ISerializerFactory& factory = ISerializerFactory::defaultFactory()) {
//    auto reader = factory.createReader(byteArray);
//    std::vector<ObjType> vec;
//    int32_t size = reader->getInt("size");
//    for (int32_t i = 0; i < size; ++i) {
//        ObjType obj;
//        deserializeImpl(obj, *reader);
//        vec.push_back(obj);
//    }
//    return vec;
//}


class ISerializable {
public:
    virtual ~ISerializable() {}

    virtual std::unique_ptr<ISerializable> clone() const = 0;
    virtual void serialize(ISerialWriter& writer) const = 0;
    virtual void deserialize(const ISerialReader& reader) = 0;
};

// CRTP for clone operation
template <typename Derived>
class SerializableTemplate : public ISerializable {
public:
    virtual std::unique_ptr<ISerializable> clone() const override {
        return mocca::make_unique<Derived>(static_cast<const Derived&>(*this));
    }
};
}