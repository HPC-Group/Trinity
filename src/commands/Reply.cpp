#include "commands/Reply.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ISerialObjectFactory.h"
#include "commands/ProcessingCommands.h"

#include "mocca/base/Memory.h"

using namespace trinity::commands;

std::unique_ptr<Reply> Reply::createFromSerialObject(const ISerialObject& serial) {
    VclType type = Vcl::instance().toType(serial.getString("type"));

    // processing commands
    if (type == InitProcessingSessionReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<InitProcessingSessionReply>();
        serial.getSerializable("rep", *reply);
        return reply;
    }

    // IO commands
    else if (type == InitIOSessionReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<InitIOSessionReply>();
        serial.getSerializable("rep", *reply);
        return reply;
    } else if (type == GetLODLevelCountReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<GetLODLevelCountReply>();
        serial.getSerializable("rep", *reply);
        return reply;
    }

    // error commands
    else if (type == ErrorReply::Ifc::Type) {
        std::unique_ptr<Reply> reply = mocca::make_unique<ErrorReply>();
        serial.getSerializable("rep", *reply);
        return reply;
    }

    throw mocca::Error("Invalid reply type", __FILE__, __LINE__);
}

std::unique_ptr<ISerialObject> Reply::createSerialObject(const Reply& reply) {
    auto serial = ISerialObjectFactory::create();
    serial->append("type", Vcl::instance().toString(reply.getType()));
    serial->append("rep", reply);
    return serial;
}