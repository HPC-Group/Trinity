#pragma once

#include "commands/ICommandHandler.h"
#include "commands/IOCommands.h"

namespace trinity {
namespace io {

// command-pattern like execution of trinity commands
class InitIOSessionHdl : public commands::ICommandHandler {
public:
    InitIOSessionHdl(const commands::InitIOSessionRequest& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    commands::InitIOSessionRequest m_request;
};


class GetLODLevelCountHdl : public commands::ICommandHandler {
public:
    GetLODLevelCountHdl(const commands::GetLODLevelCountRequest& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    commands::GetLODLevelCountRequest m_request;
};


class ListFilesHdl : public commands::ICommandHandler {
public:
    ListFilesHdl(const commands::ListFilesRequest& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    commands::ListFilesRequest m_request;
};
}
}