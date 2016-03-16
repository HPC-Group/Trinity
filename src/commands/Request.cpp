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
#define PYTHON_MAGIC_PROC


	else if (type == ZoomCameraRequest::Ifc::Type) {
		return reader->getSerializablePtr<ZoomCameraRequest>("req");
	}

#undef PYTHON_MAGIC_PROC

    // IO commands
    else if (type == ListFilesRequest::Ifc::Type) {
        return reader->getSerializablePtr<ListFilesRequest>("req");
    } else if (type == InitIOSessionRequest::Ifc::Type) {
        return reader->getSerializablePtr<InitIOSessionRequest>("req");
    } else if (type == GetLODLevelCountRequest::Ifc::Type) {
        return reader->getSerializablePtr<GetLODLevelCountRequest>("req");
    }
#define PYTHON_MAGIC_IO


	else if (type == GetMaxBrickSizeRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetMaxBrickSizeRequest>("req");
	}
	else if (type == GetMaxUsedBrickSizesRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetMaxUsedBrickSizesRequest>("req");
	}
	else if (type == MaxMinForKeyRequest::Ifc::Type) {
		return reader->getSerializablePtr<MaxMinForKeyRequest>("req");
	}
	else if (type == GetNumberOfTimestepsRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetNumberOfTimestepsRequest>("req");
	}
	else if (type == GetDomainSizeRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetDomainSizeRequest>("req");
	}
	else if (type == GetTransformationRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetTransformationRequest>("req");
	}
	else if (type == GetBrickOverlapSizeRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetBrickOverlapSizeRequest>("req");
	}
	else if (type == GetLargestSingleBrickLODRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetLargestSingleBrickLODRequest>("req");
	}
	else if (type == GetBrickVoxelCountsRequest::Ifc::Type) {
		return reader->getSerializablePtr<GetBrickVoxelCountsRequest>("req");
	}

#undef PYTHON_MAGIC_IO

    throw TrinityError("Invalid request type", __FILE__, __LINE__);
}

mocca::ByteArray Request::createByteArray(const Request& request) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->appendString("type", Vcl::instance().toString(request.getType()));
    writer->appendObject("req", request);
    return writer->write();
}

namespace trinity {
std::ostream& operator<<(std::ostream& os, const Request& obj) {
    return os << obj.toString();
}
}
