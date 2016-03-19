#include "io-base/IONode.h"

#include "io-base/FractalListData.h"

#include "mocca/base/Memory.h"
#include "mocca/net/NetworkError.h"
#include "mocca/log/LogManager.h"

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
        if (lister->containsIOData(fileID)) { // if it is a dir or a file of that lister
            return *lister;
        }
    }
    throw TrinityError("No matching data lister for file ID " + fileID, __FILE__, __LINE__);
}

void IONode::handleSessionErrors() {
    auto it = begin(m_sessions);
    while (it != end(m_sessions)) {
        try {
            (*it)->rethrowException();
            ++it;
        } catch (const mocca::net::NetworkError& err) {
            LWARNING("(p) session terminated because of a network error: " << err.what());
            it = m_sessions.erase(it);
        } catch (...) {
            LDEBUG("(p) error in session detected");
            setException(std::current_exception());
        }
    }
}
