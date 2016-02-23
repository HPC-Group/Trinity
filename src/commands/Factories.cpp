#include "commands/Factories.h"

#include "commands/IOCommands.h"
#include "commands/ProcessingCommands.h"
#include "commands/ErrorCommands.h"

namespace trinity {
namespace commands {
std::unique_ptr<Request> createRequest(const ISerialObject& obj) {
    VclType type = Vcl::instance().toType(obj.getString("type"));
    switch (type) {
        // todo dmc: create request depending on type
    }

    return nullptr;
}

std::unique_ptr<Reply> createreply(const ISerialObject& obj) {
    VclType type = Vcl::instance().toType(obj.getString("type"));
    switch (type) {
        // todo dmc: create reply depending on type
    }

    return nullptr;
}
}
}