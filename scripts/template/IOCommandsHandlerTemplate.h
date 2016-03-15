class {CommandName}Hdl : public ICommandHandler {
public:
    {CommandName}Hdl(const {CommandName}Request&request, IOSession* session);

    std::unique_ptr<Reply> execute() override;

private:
    {CommandName}Request m_request;
	IOSession* m_session;
};
