#pragma once

#include "common/AbstractNode.h"
#include "io-base/IOCommandFactory.h"
#include "io-base/IOSession.h"
#include "io-base/IListData.h"

namespace trinity {
    class IONode : public AbstractNode {

    public:
        IONode(); // use for testing purposes only!!
        IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator);
        ~IONode();

        IListData& getListDataForID(int fileID) const;
        void addSession(std::unique_ptr<IOSession> session);

    private:
        std::unique_ptr<ICommandHandler> createHandler(const Request& request) override;

    private:
        IONodeCommandFactory m_factory;
        std::vector<std::unique_ptr<IListData>> m_listData;
        std::vector<std::unique_ptr<IOSession>> m_sessions;
    };
}
