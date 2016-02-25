#include "commands/Reply.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/SerializerFactory.h"

#include "mocca/base/Memory.h"

using namespace trinity::commands;

std::unique_ptr<Reply> Reply::createFromByteArray(mocca::ByteArray& byteArray) {
    auto reader = ISerializerFactory::defaultFactory().createReader(byteArray);
    VclType type = Vcl::instance().toType(reader->getString("type"));

    // processing commands
    if (type == InitProcessingSessionReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<InitProcessingSessionReply>();
        reader->getSerializable("rep", *reply);
        return reply;
    }

    // IO commands
    else if (type == InitIOSessionReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<InitIOSessionReply>();
        reader->getSerializable("rep", *reply);
        return reply;
    } else if (type == GetLODLevelCountReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<GetLODLevelCountReply>();
        reader->getSerializable("rep", *reply);
        return reply;
    }

    // error commands
    else if (type == ErrorReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<ErrorReply>();
        reader->getSerializable("rep", *reply);
        return reply;
    }

    throw mocca::Error("Invalid reply type", __FILE__, __LINE__);
}

mocca::ByteArray Reply::createByteArray(const Reply& reply) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->append("type", Vcl::instance().toString(reply.getType()));
    writer->append("rep", reply);
    return writer->write();
}