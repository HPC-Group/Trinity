#include "commands/Request.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/SerializerFactory.h"

#include "mocca/base/Memory.h"

using namespace trinity::commands;

std::unique_ptr<Request> Request::createFromByteArray(mocca::ByteArray& byteArray) {
    auto reader = ISerializerFactory::defaultFactory().createReader(byteArray);
    VclType type = Vcl::instance().toType(reader->getString("type"));

    // processing commands
    if (type == InitProcessingSessionRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<InitProcessingSessionRequest>();
        reader->getSerializable("req", *request);
        return request;
    } else if (type == SetIsoValueRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<SetIsoValueRequest>();
        reader->getSerializable("req", *request);
        return request;
    }

    // IO commands
    else if (type == ListFilesRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<ListFilesRequest>();
        reader->getSerializable("req", *request);
        return request;
    } else if (type == InitIOSessionRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<InitIOSessionRequest>();
        reader->getSerializable("req", *request);
        return request;
    } else if (type == GetLODLevelCountRequest::Ifc::Type) {
        std::unique_ptr<Request> request = mocca::make_unique<GetLODLevelCountRequest>();
        reader->getSerializable("req", *request);
        return request;
    }

    throw mocca::Error("Invalid request type", __FILE__, __LINE__);
}

mocca::ByteArray Request::createByteArray(const Request& request) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->append("type", Vcl::instance().toString(request.getType()));
    writer->append("req", request);
    return writer->write();
}