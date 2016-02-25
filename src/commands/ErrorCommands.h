#pragma once

#include "commands/ISerializable.h"
#include "commands/Reply.h"

namespace trinity {
namespace commands {

struct ErrorCmd {
    static VclType Type;

    class ReplyParams : public ISerializable {
    public:
        ReplyParams() = default;
        ReplyParams(int errorCode);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string printError() const;
        int getErrorCode() const;

        bool equals(const ReplyParams& other) const;

    private:
        int m_errorCode;
    };
};

bool operator==(const ErrorCmd::ReplyParams& lhs, const ErrorCmd::ReplyParams& rhs);

using ErrorReply = ReplyTemplate<ErrorCmd>;

}
}