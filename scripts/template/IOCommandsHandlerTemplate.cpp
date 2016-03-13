{CommandName}Hdl::{CommandName}Hdl(const {CommandName}Request& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> {CommandName}Hdl::execute() {
}
