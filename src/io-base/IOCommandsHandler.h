#pragma once

#include "commands/ICommandHandler.h"
#include "commands/IOCommands.h"

namespace trinity {
// command-pattern like execution of trinity commands
class InitIOSessionHdl : public ICommandHandler {
public:
    InitIOSessionHdl(const InitIOSessionRequest& request);

    std::unique_ptr<Reply> execute() override;

private:
    InitIOSessionRequest m_request;
};


class GetLODLevelCountHdl : public ICommandHandler {
public:
    GetLODLevelCountHdl(const GetLODLevelCountRequest& request);

    std::unique_ptr<Reply> execute() override;

private:
    GetLODLevelCountRequest m_request;
};


class ListFilesHdl : public ICommandHandler {
public:
    ListFilesHdl(const ListFilesRequest& request);

    std::unique_ptr<Reply> execute() override;

private:
    ListFilesRequest m_request;
};
}