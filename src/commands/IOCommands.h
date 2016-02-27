#pragma once

#include "commands/ISerializable.h"
#include "commands/Request.h"
#include "commands/Reply.h"

namespace trinity {
namespace commands {

struct ListFilesCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;
    };
};

struct InitIOSessionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const std::string& protocol, int fileId);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getProtocol() const;
        int getFileId() const;

        bool equals(const InitIOSessionCmd::RequestParams& other) const;

    private:
        std::string m_protocol;
        int m_fileId;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(int controlPort);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        int getControlPort() const;

        bool equals(const InitIOSessionCmd::ReplyParams& other) const;

    private:
        int m_controlPort;
    };
};

bool operator==(const InitIOSessionCmd::RequestParams& lhs, const InitIOSessionCmd::RequestParams& rhs);
bool operator==(const InitIOSessionCmd::ReplyParams& lhs, const InitIOSessionCmd::ReplyParams& rhs);

struct GetLODLevelCountCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(int lodCount);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        int getLODLevelCount() const;

        bool equals(const GetLODLevelCountCmd::ReplyParams& other) const;

    private:
        int m_lodCount;
    };
};

bool operator==(const GetLODLevelCountCmd::ReplyParams& lhs, const GetLODLevelCountCmd::ReplyParams& rhs);

class IOData : public SerializableTemplate<IOData> {
public:
    IOData() = default;
    IOData(const std::string& name, int fileId, const VclType& dataType);

    void serialize(ISerialWriter& writer) const override;
    void deserialize(const ISerialReader& reader) override;

    std::string getName() const;
    int getFileId() const;
    VclType getDataType() const;

    bool equals(const IOData& other) const;

private:
    std::string m_name;
    int m_fileId;
    VclType m_dataType;
};

bool operator==(const IOData& lhs, const IOData& rhs);


using ListFilesRequest = RequestTemplate<ListFilesCmd>;

using InitIOSessionRequest = RequestTemplate<InitIOSessionCmd>;
using InitIOSessionReply = ReplyTemplate<InitIOSessionCmd>;

using GetLODLevelCountRequest = RequestTemplate<GetLODLevelCountCmd>;
using GetLODLevelCountReply = ReplyTemplate<GetLODLevelCountCmd>;

}
}