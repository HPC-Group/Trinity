class {CommandName}Hdl : public ICommandHandler {
public:
    {CommandName}Hdl(const {CommandName}Request&request, RenderSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {CommandName}Request m_request;
	RenderSession* m_session;
};
