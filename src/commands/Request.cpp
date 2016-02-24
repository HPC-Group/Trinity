#include "commands/Request.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ISerialObjectFactory.h"
#include "commands/ProcessingCommands.h"

#include "mocca/base/Memory.h"

using namespace trinity::commands;

std::unique_ptr<Request> Request::createFromSerialObject(const ISerialObject& serial) {
    VclType type = Vcl::instance().toType(serial.getString("type"));

    // processing commands
    if (type == InitProcessingSessionRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<InitProcessingSessionRequest>();
        serial.getSerializable("req", *request);
        return request;
    } else if (type == SetIsoValueRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<SetIsoValueRequest>();
        serial.getSerializable("req", *request);
        return request;
    }

    // IO commands
    else if (type == ListFilesRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<ListFilesRequest>();
        serial.getSerializable("req", *request);
        return request;
    } else if (type == InitIOSessionRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<InitIOSessionRequest>();
        serial.getSerializable("req", *request);
        return request;
    } else if (type == GetLODLevelCountRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<GetLODLevelCountRequest>();
        serial.getSerializable("req", *request);
        return request;
    }

    throw mocca::Error("Invalid request type", __FILE__, __LINE__);
}

std::unique_ptr<ISerialObject> Request::createSerialObject(const Request& request) {
    auto serial = ISerialObjectFactory::create();
    serial->append("type", Vcl::instance().toString(request.getType()));
    serial->append("req", request);
    return serial;
}