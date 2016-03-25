class {{CommandNameHdl}} : public ICommandHandler {
public:
    {{CommandNameHdl}}(const {{CommandNameRequest}}& request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {{CommandNameRequest}} m_request;
	IOSession* m_session;
};
