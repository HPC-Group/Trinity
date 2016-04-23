#pragma once

#include "common/Enums.h"
#include "commands/ICommandHandler.h"
#include "commands/Request.h"

#include "mocca/base/Thread.h"
#include "mocca/net/ConnectionAggregator.h"

#include <memory>
#include <vector>

namespace trinity {
class AbstractNode : public mocca::Runnable {

public:
    // Indicates if IO node and processing node are executed combined in the same process or in separate processes
    enum class ExecutionMode { Separate, Combined };

    AbstractNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator, ExecutionMode executionMode, CompressionMode compressionMode);
    ~AbstractNode();

    ExecutionMode executionMode() const;

    bool isLocalMachine(const std::string& machine) const;

private:
    void run();
    virtual std::unique_ptr<ICommandHandler> createHandler(const Request& request) = 0;
    virtual void handleSessionErrors() = 0;

private:
    std::unique_ptr<mocca::net::ConnectionAggregator> m_aggregator;
    ExecutionMode m_executionMode;
    CompressionMode m_compressionMode;
};
}
