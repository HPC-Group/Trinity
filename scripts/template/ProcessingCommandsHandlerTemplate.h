class {{CommandNameHdl}} : public ICommandHandler {
public:
    {{CommandNameHdl}}(const {{CommandNameRequest}}& request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {{CommandNameRequest}} m_request;
	RenderSession* m_session;
};
