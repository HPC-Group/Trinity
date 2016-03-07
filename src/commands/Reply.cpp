#include "commands/Reply.h"

#include "common/TrinityError.h"
#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/SerializerFactory.h"

#include "mocca/base/Memory.h"

using namespace trinity::commands;
using namespace trinity::common;


std::unique_ptr<Reply> Reply::createFromByteArray(mocca::ByteArray& byteArray) {
    auto reader = ISerializerFactory::defaultFactory().createReader(byteArray);
    VclType type = Vcl::instance().toType(reader->getString("type"));

    // processing commands
    if (type == InitProcessingSessionReply::Ifc::Type) {
        return reader->getSerializablePtr<InitProcessingSessionReply>("rep");
    }

    // IO commands
    else if (type == InitIOSessionReply::Ifc::Type) {
        return reader->getSerializablePtr<InitIOSessionReply>("rep");
    } else if (type == GetLODLevelCountReply::Ifc::Type) {
        return reader->getSerializablePtr<GetLODLevelCountReply>("rep");
    }

    // error commands
    else if (type == ErrorReply::Ifc::Type) {
        return reader->getSerializablePtr<ErrorReply>("rep");
    }

    throw TrinityError("Invalid reply type", __FILE__, __LINE__);
}

mocca::ByteArray Reply::createByteArray(const Reply& reply) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->append("type", Vcl::instance().toString(reply.getType()));
    writer->append("rep", reply);
    return writer->write();
}

namespace trinity {
namespace commands {
std::ostream& operator<<(std::ostream& os, const Reply& obj) {
    return os << obj.toString();
}
}
}