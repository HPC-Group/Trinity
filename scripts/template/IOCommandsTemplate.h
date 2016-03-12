struct {CommandName}Cmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;
    };
};

bool operator==(const {CommandName}Cmd::RequestParams& lhs, const {CommandName}Cmd::RequestParams& rhs);
bool operator==(const {CommandName}Cmd::ReplyParams& lhs, const {CommandName}Cmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const {CommandName}Cmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const {CommandName}Cmd::ReplyParams& obj);

using {CommandName}Request = RequestTemplate<{CommandName}Cmd>;
using {CommandName}Reply = ReplyTemplate<{CommandName}Cmd>;
