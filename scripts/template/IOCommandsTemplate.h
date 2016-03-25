struct {{CommandNameCmd}} {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
		{{RequestCtorDeclaration}}
		
        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

{{RequestGetterDeclarations}}

{{RequestMembers}}
	};

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
		{{ReplyCtorDeclaration}}

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

{{ReplyGetters}}

{{ReplyMembers}}
	};
};

bool operator==(const {{CommandNameCmd}}::RequestParams& lhs, const {{CommandNameCmd}}::RequestParams& rhs);
bool operator==(const {{CommandNameCmd}}::ReplyParams& lhs, const {{CommandNameCmd}}::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::ReplyParams& obj);

using {{CommandNameRequest}} = RequestTemplate<{{CommandNameCmd}}>;
using {{CommandNameReply}} = ReplyTemplate<{{CommandNameCmd}}>;