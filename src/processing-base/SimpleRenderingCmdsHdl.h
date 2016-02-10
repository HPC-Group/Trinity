#pragma once
#include "common/ICommandHandler.h"
#include "common/SimpleRenderingCmds.h"

namespace trinity {
namespace processing {

// command-pattern like execution of trinity commands
class SetIsoValueHdl : public common::ICommandHandler {
    
public:
    virtual void execute();
    SetIsoValueHdl(common::SetIsoValueCmd cmd);
    ~SetIsoValueHdl();
    
    
private:
    float m_isoValue;
    int m_sid;
};
}
}