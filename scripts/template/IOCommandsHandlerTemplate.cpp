{{CommandNameHdl}}::{{CommandNameHdl}}(const {{CommandNameRequest}}& request, IOSession* session)
    : m_request(request), m_session(session) {}

std::unique_ptr<Reply> {{CommandNameHdl}}::execute() {
}
