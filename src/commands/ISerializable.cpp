#include "commands/ISerializable.h"

using namespace trinity::commands;

std::string ISerializable::toString() const {
    std::stringstream stream;
    stream << "type: " << Vcl::instance().toString(getType()) << std::endl;
    return stream.str();
}

namespace trinity {
namespace commands {
std::ostream& operator<<(std::ostream& os, const ISerializable& obj) {
    return os << obj.toString();
}
}
}
