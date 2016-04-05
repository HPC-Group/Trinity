#pragma once

#include "commands/ISerializable.h"
#include "commands/Reply.h"

namespace trinity {
struct ErrorCmd {
    static VclType Type;

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const std::string& error);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getError() const;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

    private:
        std::string m_error;
    };
};

bool operator==(const ErrorCmd::ReplyParams& lhs, const ErrorCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const ErrorCmd::ReplyParams& obj);

using ErrorReply = ReplyTemplate<ErrorCmd>;
}