	else if (type == PROC_ELEFANTRequest::Ifc::Type) {
		return reader->getSerializablePtr<PROC_ELEFANTRequest>("req");
	}

#include "commands/Request.h"

#include "common/TrinityError.h"
#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/SerializerFactory.h"

#include "mocca/base/Memory.h"

using namespace trinity;

std::unique_ptr<Request> Request::createFromByteArray(mocca::ByteArray& byteArray) {
    auto reader = ISerializerFactory::defaultFactory().createReader(byteArray);
    VclType type = Vcl::instance().toType(reader->getString("type"));

    // processing commands
    if (type == InitProcessingSessionRequest::Ifc::Type) {
        return reader->getSerializablePtr<InitProcessingSessionRequest>("req");
    } else if (type == SetIsoValueRequest::Ifc::Type) {
        return reader->getSerializablePtr<SetIsoValueRequest>("req");
    } else if (type == InitContextRequest::Ifc::Type) {
        return reader->getSerializablePtr<InitContextRequest>("req");
    }

    // IO commands
    else if (type == ListFilesRequest::Ifc::Type) {
        return reader->getSerializablePtr<ListFilesRequest>("req");
    } else if (type == InitIOSessionRequest::Ifc::Type) {
        return reader->getSerializablePtr<InitIOSessionRequest>("req");
    } else if (type == GetLODLevelCountRequest::Ifc::Type) {
        return reader->getSerializablePtr<GetLODLevelCountRequest>("req");
    }

    throw TrinityError("Invalid request type", __FILE__, __LINE__);
}

mocca::ByteArray Request::createByteArray(const Request& request) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->append("type", Vcl::instance().toString(request.getType()));
    writer->append("req", request);
    return writer->write();
}

namespace trinity {
std::ostream& operator<<(std::ostream& os, const Request& obj) {
    return os << obj.toString();
}
}