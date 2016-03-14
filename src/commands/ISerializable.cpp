#include "commands/ISerializable.h"

namespace trinity {

void serializeInt(const std::string& key, int32_t obj, ISerialWriter& writer) {
    writer.append(key, obj);
}

void serializeFloat(const std::string& key, float obj, ISerialWriter& writer) {
    writer.append(key, obj);
}

void serializeString(const std::string& key, const std::string& obj, ISerialWriter& writer) {
    writer.append(key, obj);
}


void deserializeInt(const std::string& key, int32_t& obj, ISerialReader& reader) {
    obj = reader.getInt(key);
}

void deserializeFloat(const std::string& key, float& obj, ISerialReader& reader) {
    obj = reader.getFloat(key);
}

void deserializeString(const std::string& key, std::string& obj, ISerialReader& reader) {
    obj = reader.getString(key);
}
}