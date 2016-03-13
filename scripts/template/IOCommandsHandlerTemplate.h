class {CommandName}Hdl : public ICommandHandler {
public:
    {CommandName}Hdl(const {CommandName}Request& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    {CommandName}Request m_request;
};
