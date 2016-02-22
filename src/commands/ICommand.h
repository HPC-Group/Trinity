#pragma once

#include "ISerializable.h"
#include "Vcl.h"

namespace trinity {
namespace commands {

class ICommand : public ISerializable {
public:
    ICommand(int sid, int rid);

    void serialize(ISerialObject& serial) const override;
    void deserialize(ISerialObject& serial) override;
    std::string toString() const override;

    int getSid() const;
    int getRid() const;

protected:
    int m_sid;
    int m_rid;
};
}
}