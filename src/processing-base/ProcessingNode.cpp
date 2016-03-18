#include "processing-base/ProcessingNode.h"
#include "mocca/log/LogManager.h"

using namespace trinity;

ProcessingNode::ProcessingNode(std::unique_ptr<mocca::net::ConnectionAggregator> aggregator)
    : AbstractNode(std::move(aggregator)) {}

ProcessingNode::~ProcessingNode() {
    join();
}

std::unique_ptr<ICommandHandler> ProcessingNode::createHandler(const Request& request) {
    return m_factory.createHandler(request, this);
}

void ProcessingNode::addSession(std::unique_ptr<RenderSession> session) {
    cleanupInterruptedSessions();
    m_sessions.push_back(std::move(session));
    LINFO("(p) session created");
}

std::vector<std::unique_ptr<RenderSession>>& ProcessingNode::getSessions() {
    return m_sessions;
}

void ProcessingNode::cleanupInterruptedSessions() {
    LINFO("(p) cleaning up old sessions...");
    int counter = 0;
    std::vector<std::unique_ptr<RenderSession>> newSessions;
    for(auto& s : m_sessions) {
	LINFO("(p) iter...");
        if(!s->isInterrupted()) {
            newSessions.push_back(std::move(s));  // todo at david, ok like that?
        } else {
            counter++;
		LINFO("reset...");
            //s.reset();
		LINFO("reset done");
        }
    }
    LINFO("(p) removed " + std::to_string(counter) + " sessions");
    m_sessions = std::move(newSessions);
}
