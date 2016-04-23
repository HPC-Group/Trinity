#include "processing-base/ProcessingNode.h"

#include "mocca/log/LogManager.h"
#include "mocca/net/NetworkError.h"

using namespace trinity;

ProcessingNode::ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator, AbstractNode::ExecutionMode executionMode,
                               CompressionMode compressionMode)
    : AbstractNode(std::move(aggregator), executionMode, compressionMode) {}

ProcessingNode::~ProcessingNode() {
    join();
}

std::unique_ptr<ICommandHandler> ProcessingNode::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}

void ProcessingNode::addSession(std::unique_ptr<RenderSession> session) {
    m_sessions.push_back(std::move(session));
    LINFO("(p) session created");
}

std::vector<std::unique_ptr<RenderSession>>& ProcessingNode::getSessions() {
    return m_sessions;
}

bool ProcessingNode::maxSessionsReached() const {
    static unsigned int maxSessions = 3;
    return m_sessions.size() >= maxSessions;
}

void ProcessingNode::handleSessionErrors() {
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
