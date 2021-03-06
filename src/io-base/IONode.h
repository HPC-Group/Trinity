#pragma once

#include "common/AbstractNode.h"
#include "io-base/IListData.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOSession.h"

namespace trinity {
class IONode : public AbstractNode {

public:
    IONode(); // use for testing purposes only!!
    IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator, std::vector<std::unique_ptr<IListData>> listData,
           AbstractNode::ExecutionMode executionMode = AbstractNode::ExecutionMode::Separate);
    ~IONode();

    IListData& getListDataForID(const std::string& fileID) const;
    std::vector<std::string> getRoots() const;

    void addSession(std::unique_ptr<IOSession> session);
    bool maxSessionsReached() const;

private:
    std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;
    void handleSessionErrors() override;

private:
    IONodeCommandFactory m_factory;
    std::vector<std::unique_ptr<IListData>> m_listData;
    std::vector<std::unique_ptr<IOSession>> m_sessions;
};
}
