#pragma once

#include "commands/ICommandHandler.h"
#include "commands/IOCommands.h"
#include "io-base/IONode.h"
#include "io-base/IOSession.h"

namespace trinity {
// command-pattern like execution of trinity commands
class InitIOSessionHdl : public ICommandHandler {
public:
    InitIOSessionHdl(const InitIOSessionRequest& request, IONode* node);

    std::unique_ptr<Reply> execute() override;

private:
    IONode* m_node;
    InitIOSessionRequest m_request;
};

class GetLODLevelCountHdl : public ICommandHandler {
public:
    GetLODLevelCountHdl(const GetLODLevelCountRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    IOSession* m_session;
    GetLODLevelCountRequest m_request;
};

class ListFilesHdl : public ICommandHandler {
public:
    ListFilesHdl(const ListFilesRequest& request, IONode* node);

    std::unique_ptr<Reply> execute() override;

private:
    IONode* m_node;
    ListFilesRequest m_request;
};

class GetMaxBrickSizeHdl : public ICommandHandler {
public:
    GetMaxBrickSizeHdl(const GetMaxBrickSizeRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetMaxBrickSizeRequest m_request;
    IOSession* m_session;
};

class GetMaxUsedBrickSizesHdl : public ICommandHandler {
public:
    GetMaxUsedBrickSizesHdl(const GetMaxUsedBrickSizesRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetMaxUsedBrickSizesRequest m_request;
    IOSession* m_session;
};

class MaxMinForKeyHdl : public ICommandHandler {
public:
    MaxMinForKeyHdl(const MaxMinForKeyRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    MaxMinForKeyRequest m_request;
    IOSession* m_session;
};

class GetNumberOfTimestepsHdl : public ICommandHandler {
public:
    GetNumberOfTimestepsHdl(const GetNumberOfTimestepsRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetNumberOfTimestepsRequest m_request;
    IOSession* m_session;
};

class GetDomainSizeHdl : public ICommandHandler {
public:
    GetDomainSizeHdl(const GetDomainSizeRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDomainSizeRequest m_request;
    IOSession* m_session;
};

class GetTransformationHdl : public ICommandHandler {
public:
    GetTransformationHdl(const GetTransformationRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetTransformationRequest m_request;
    IOSession* m_session;
};

class GetBrickOverlapSizeHdl : public ICommandHandler {
public:
    GetBrickOverlapSizeHdl(const GetBrickOverlapSizeRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickOverlapSizeRequest m_request;
    IOSession* m_session;
};

class GetLargestSingleBrickLODHdl : public ICommandHandler {
public:
    GetLargestSingleBrickLODHdl(const GetLargestSingleBrickLODRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetLargestSingleBrickLODRequest m_request;
    IOSession* m_session;
};

class GetBrickVoxelCountsHdl : public ICommandHandler {
public:
    GetBrickVoxelCountsHdl(const GetBrickVoxelCountsRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickVoxelCountsRequest m_request;
    IOSession* m_session;
};

class GetBrickExtentsHdl : public ICommandHandler {
public:
    GetBrickExtentsHdl(const GetBrickExtentsRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickExtentsRequest m_request;
    IOSession* m_session;
};

class GetBrickLayoutHdl : public ICommandHandler {
public:
    GetBrickLayoutHdl(const GetBrickLayoutRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickLayoutRequest m_request;
    IOSession* m_session;
};

class GetModalityCountHdl : public ICommandHandler {
public:
    GetModalityCountHdl(const GetModalityCountRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetModalityCountRequest m_request;
    IOSession* m_session;
};

class GetComponentCountHdl : public ICommandHandler {
public:
    GetComponentCountHdl(const GetComponentCountRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetComponentCountRequest m_request;
    IOSession* m_session;
};

class GetRangeHdl : public ICommandHandler {
public:
    GetRangeHdl(const GetRangeRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetRangeRequest m_request;
    IOSession* m_session;
};

class GetTotalBrickCountHdl : public ICommandHandler {
public:
    GetTotalBrickCountHdl(const GetTotalBrickCountRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetTotalBrickCountRequest m_request;
    IOSession* m_session;
};

class GetBrickHdl : public ICommandHandler {
public:
    GetBrickHdl(const GetBrickRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickRequest m_request;
    IOSession* m_session;
};

class GetTypeHdl : public ICommandHandler {
public:
    GetTypeHdl(const GetTypeRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetTypeRequest m_request;
    IOSession* m_session;
};

class GetSemanticHdl : public ICommandHandler {
public:
    GetSemanticHdl(const GetSemanticRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetSemanticRequest m_request;
    IOSession* m_session;
};

class GetDefault1DTransferFunctionCountHdl : public ICommandHandler {
public:
    GetDefault1DTransferFunctionCountHdl(const GetDefault1DTransferFunctionCountRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDefault1DTransferFunctionCountRequest m_request;
    IOSession* m_session;
};

class GetDefault2DTransferFunctionCountHdl : public ICommandHandler {
public:
    GetDefault2DTransferFunctionCountHdl(const GetDefault2DTransferFunctionCountRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDefault2DTransferFunctionCountRequest m_request;
    IOSession* m_session;
};

class Get1DHistogramHdl : public ICommandHandler {
public:
    Get1DHistogramHdl(const Get1DHistogramRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    Get1DHistogramRequest m_request;
    IOSession* m_session;
};

class Get2DHistogramHdl : public ICommandHandler {
public:
    Get2DHistogramHdl(const Get2DHistogramRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    Get2DHistogramRequest m_request;
    IOSession* m_session;
};

class GetUserDefinedSemanticHdl : public ICommandHandler {
public:
    GetUserDefinedSemanticHdl(const GetUserDefinedSemanticRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetUserDefinedSemanticRequest m_request;
    IOSession* m_session;
};

class GetDefault1DTransferFunctionHdl : public ICommandHandler {
public:
    GetDefault1DTransferFunctionHdl(const GetDefault1DTransferFunctionRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDefault1DTransferFunctionRequest m_request;
    IOSession* m_session;
};

class GetDomainScaleHdl : public ICommandHandler {
public:
    GetDomainScaleHdl(const GetDomainScaleRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDomainScaleRequest m_request;
    IOSession* m_session;
};

class GetFloatBrickLayoutHdl : public ICommandHandler {
public:
    GetFloatBrickLayoutHdl(const GetFloatBrickLayoutRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetFloatBrickLayoutRequest m_request;
    IOSession* m_session;
};

class GetBrickMaxMinHdl : public ICommandHandler {
public:
    GetBrickMaxMinHdl(const GetBrickMaxMinRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickMaxMinRequest m_request;
    IOSession* m_session;
};

class GetBricksHdl : public ICommandHandler {
public:
    GetBricksHdl(const GetBricksRequest& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBricksRequest m_request;
    IOSession* m_session;
};

class GetRootsHdl : public ICommandHandler {
public:
    GetRootsHdl(const GetRootsRequest& request, IONode* node);

    std::unique_ptr<Reply> execute() override;

private:
    GetRootsRequest m_request;
    IONode* m_node;
};

/* AUTOGEN IOCommandHandlerHeader */
}