#pragma once
#include "ICommand.h"


namespace trinity {
namespace common {

class ErrorCmd : public ICommand {
    
public:
    ErrorCmd();
    ErrorCmd(int sid, int rid, int errorCode);
    virtual ~ErrorCmd();
    
    virtual VclType getType() const;

    const std::string printError();
    int getErrorCode() const;
    
    virtual void serialize(ISerialObject& serial);
    virtual void deserialize(ISerialObject& serial);
    
private:
    int m_errorCode;
};
}
}