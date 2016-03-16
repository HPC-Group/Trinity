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

#define PYTHON_MAGIC


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
    GetMaxUsedBrickSizesHdl(const GetMaxUsedBrickSizesRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetMaxUsedBrickSizesRequest m_request;
	IOSession* m_session;
};

class MaxMinForKeyHdl : public ICommandHandler {
public:
    MaxMinForKeyHdl(const MaxMinForKeyRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    MaxMinForKeyRequest m_request;
	IOSession* m_session;
};

class GetNumberOfTimestepsHdl : public ICommandHandler {
public:
    GetNumberOfTimestepsHdl(const GetNumberOfTimestepsRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetNumberOfTimestepsRequest m_request;
	IOSession* m_session;
};

class GetDomainSizeHdl : public ICommandHandler {
public:
    GetDomainSizeHdl(const GetDomainSizeRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetDomainSizeRequest m_request;
	IOSession* m_session;
};

class GetTransformationHdl : public ICommandHandler {
public:
    GetTransformationHdl(const GetTransformationRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetTransformationRequest m_request;
	IOSession* m_session;
};

class GetBrickOverlapSizeHdl : public ICommandHandler {
public:
    GetBrickOverlapSizeHdl(const GetBrickOverlapSizeRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickOverlapSizeRequest m_request;
	IOSession* m_session;
};

class GetLargestSingleBrickLODHdl : public ICommandHandler {
public:
    GetLargestSingleBrickLODHdl(const GetLargestSingleBrickLODRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetLargestSingleBrickLODRequest m_request;
	IOSession* m_session;
};

class GetBrickVoxelCountsHdl : public ICommandHandler {
public:
    GetBrickVoxelCountsHdl(const GetBrickVoxelCountsRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickVoxelCountsRequest m_request;
	IOSession* m_session;
};

class GetBrickExtentsHdl : public ICommandHandler {
public:
    GetBrickExtentsHdl(const GetBrickExtentsRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickExtentsRequest m_request;
	IOSession* m_session;
};

class GetBrickLayoutHdl : public ICommandHandler {
public:
    GetBrickLayoutHdl(const GetBrickLayoutRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetBrickLayoutRequest m_request;
	IOSession* m_session;
};

class GetModalityCountHdl : public ICommandHandler {
public:
    GetModalityCountHdl(const GetModalityCountRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetModalityCountRequest m_request;
	IOSession* m_session;
};

class GetComponentCountHdl : public ICommandHandler {
public:
    GetComponentCountHdl(const GetComponentCountRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetComponentCountRequest m_request;
	IOSession* m_session;
};

class GetRangeHdl : public ICommandHandler {
public:
    GetRangeHdl(const GetRangeRequest&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    GetRangeRequest m_request;
	IOSession* m_session;
};

#undef PYTHON_MAGIC
}