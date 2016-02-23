#pragma once
#include "ICommand.h"


namespace trinity {
namespace commands {

class ErrorCmd : public ICommand {

public:
    ErrorCmd(ISerialObject&);
    ErrorCmd(int sid, int rid, int errorCode);

    VclType getType() const override;
    void serialize(ISerialObject& serial) const override;
    void deserialize(const ISerialObject& serial) override;

    const std::string printError();
    int getErrorCode() const;

private:
    int m_errorCode;
};
}
}