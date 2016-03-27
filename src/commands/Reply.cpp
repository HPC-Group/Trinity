#include "commands/Reply.h"

#include "commands/ErrorCommands.h"
#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/SerializerFactory.h"
#include "common/TrinityError.h"

#include "mocca/base/Memory.h"

using namespace trinity;


std::unique_ptr<Reply> Reply::createFromByteArray(mocca::ByteArray& byteArray) {
    auto reader = ISerializerFactory::defaultFactory().createReader(byteArray);
    VclType type = Vcl::instance().toType(reader->getString("type"));

    // processing commands
    if (type == InitProcessingSessionReply::Ifc::Type) {
        return reader->getSerializablePtr<InitProcessingSessionReply>("rep");
    } else if (type == ZoomCameraReply::Ifc::Type) {
        return reader->getSerializablePtr<ZoomCameraReply>("rep");
    } else if (type == SupportsRenderModeReply::Ifc::Type) {
        return reader->getSerializablePtr<SupportsRenderModeReply>("rep");
    } else if (type == GetActiveModalityReply::Ifc::Type) {
        return reader->getSerializablePtr<GetActiveModalityReply>("rep");
    } else if (type == GetActiveTimestepReply::Ifc::Type) {
        return reader->getSerializablePtr<GetActiveTimestepReply>("rep");
    }
	/* AUTOGEN ProcReplyFactoryEntry */

    // IO commands
    if (type == ListFilesRequest::Ifc::Type) {
        return reader->getSerializablePtr<ListFilesReply>("rep");
    } else if (type == InitIOSessionReply::Ifc::Type) {
        return reader->getSerializablePtr<InitIOSessionReply>("rep");
    } else if (type == GetLODLevelCountReply::Ifc::Type) {
        return reader->getSerializablePtr<GetLODLevelCountReply>("rep");
    } else if (type == GetMaxBrickSizeReply::Ifc::Type) {
        return reader->getSerializablePtr<GetMaxBrickSizeReply>("rep");
    } else if (type == GetMaxUsedBrickSizesReply::Ifc::Type) {
        return reader->getSerializablePtr<GetMaxUsedBrickSizesReply>("rep");
    } else if (type == MaxMinForKeyReply::Ifc::Type) {
        return reader->getSerializablePtr<MaxMinForKeyReply>("rep");
    } else if (type == GetNumberOfTimestepsReply::Ifc::Type) {
        return reader->getSerializablePtr<GetNumberOfTimestepsReply>("rep");
    } else if (type == GetDomainSizeReply::Ifc::Type) {
        return reader->getSerializablePtr<GetDomainSizeReply>("rep");
    } else if (type == GetTransformationReply::Ifc::Type) {
        return reader->getSerializablePtr<GetTransformationReply>("rep");
    } else if (type == GetBrickOverlapSizeReply::Ifc::Type) {
        return reader->getSerializablePtr<GetBrickOverlapSizeReply>("rep");
    } else if (type == GetLargestSingleBrickLODReply::Ifc::Type) {
        return reader->getSerializablePtr<GetLargestSingleBrickLODReply>("rep");
    } else if (type == GetBrickVoxelCountsReply::Ifc::Type) {
        return reader->getSerializablePtr<GetBrickVoxelCountsReply>("rep");
    } else if (type == GetBrickExtentsReply::Ifc::Type) {
        return reader->getSerializablePtr<GetBrickExtentsReply>("rep");
    } else if (type == GetBrickLayoutReply::Ifc::Type) {
        return reader->getSerializablePtr<GetBrickLayoutReply>("rep");
    } else if (type == GetModalityCountReply::Ifc::Type) {
        return reader->getSerializablePtr<GetModalityCountReply>("rep");
    } else if (type == GetComponentCountReply::Ifc::Type) {
        return reader->getSerializablePtr<GetComponentCountReply>("rep");
    } else if (type == GetRangeReply::Ifc::Type) {
        return reader->getSerializablePtr<GetRangeReply>("rep");
    } else if (type == GetTotalBrickCountReply::Ifc::Type) {
        return reader->getSerializablePtr<GetTotalBrickCountReply>("rep");
    } else if (type == GetBrickReply::Ifc::Type) {
        return reader->getSerializablePtr<GetBrickReply>("rep");
    } else if (type == GetTypeReply::Ifc::Type) {
        return reader->getSerializablePtr<GetTypeReply>("rep");
    } else if (type == GetSemanticReply::Ifc::Type) {
        return reader->getSerializablePtr<GetSemanticReply>("rep");
    } else if (type == GetDefault1DTransferFunctionCountReply::Ifc::Type) {
        return reader->getSerializablePtr<GetDefault1DTransferFunctionCountReply>("rep");
    } else if (type == Get1DHistogramReply::Ifc::Type) {
        return reader->getSerializablePtr<Get1DHistogramReply>("rep");
    } else if (type == Get2DHistogramReply::Ifc::Type) {
        return reader->getSerializablePtr<Get2DHistogramReply>("rep");
    } else if (type == GetUserDefinedSemanticReply::Ifc::Type) {
        return reader->getSerializablePtr<GetUserDefinedSemanticReply>("rep");
    } else if (type == GetDefault1DTransferFunctionReply::Ifc::Type) {
        return reader->getSerializablePtr<GetDefault1DTransferFunctionReply>("rep");
    }
	/* AUTOGEN IOReplyFactoryEntry */

    // error commands
    else if (type == ErrorReply::Ifc::Type) {
        return reader->getSerializablePtr<ErrorReply>("rep");
    }

    throw TrinityError("Invalid reply type", __FILE__, __LINE__);
}

mocca::ByteArray Reply::createByteArray(const Reply& reply) {
    auto writer = ISerializerFactory::defaultFactory().createWriter();
    writer->appendString("type", Vcl::instance().toString(reply.getType()));
    writer->appendObject("rep", reply);
    return writer->write();
}

namespace trinity {
std::ostream& operator<<(std::ostream& os, const Reply& obj) {
    return os << obj.toString();
}
}
