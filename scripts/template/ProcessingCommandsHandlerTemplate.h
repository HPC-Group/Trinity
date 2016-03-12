class {CommandName}Hdl : public commands::ICommandHandler {
public:
    {CommandName}Hdl(const commands::{CommandName}Request& request);

    std::unique_ptr<commands::Reply> execute() override;

private:
    commands::{CommandName}Request m_request;
};
