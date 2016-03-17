#include "io-base/IONode.h"

#include "io-base/FractalListData.h"

#include "mocca/base/Memory.h"

using namespace trinity;

IONode::IONode()
    : AbstractNode(nullptr) {
    m_listData.push_back(mocca::make_unique<FractalListData>());
}

IONode::IONode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
    : AbstractNode(std::move(aggregator)) {
    m_listData.push_back(mocca::make_unique<FractalListData>());
}

IONode::~IONode() {
    join();
}

std::unique_ptr<ICommandHandler> IONode::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}

void IONode::addSession(std::unique_ptr<IOSession> session) {
    m_sessions.push_back(std::move(session));
}

IListData& IONode::getListDataForID(const std::string& fileID) const {
    for (auto& lister : m_listData) {
        if (lister->canList(fileID)) {
            return *lister;
        }
    }
    throw TrinityError("No matching data lister for file ID " + fileID, __FILE__, __LINE__);
}