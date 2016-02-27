#pragma once

#include "commands/ISerializable.h"
#include "commands/Request.h"
#include "commands/Reply.h"

#include "mocca/net/Endpoint.h"

namespace trinity {
namespace commands {

class StreamingParams : public SerializableTemplate<StreamingParams> {
public:
    StreamingParams();
    StreamingParams(int resX, int resY);

    void serialize(ISerialWriter& writer) const override;
    void deserialize(const ISerialReader& reader) override;

    int getResX() const;
    int getResY() const;

    bool equals(const StreamingParams& other) const;

private:
    int m_resX, m_resY;
};

bool operator==(const StreamingParams& lhs, const StreamingParams& rhs);

struct InitProcessingSessionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const std::string& protocol, const VclType& renderType, int fileId, const std::string& stringifiedIoEndpoint,
                      const StreamingParams& p);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getProtocol() const;
        std::string getStringifiedEndpoint() const;
        int getFileId() const;
        VclType getRenderType() const;
        StreamingParams getStreamingParams() const;

        bool equals(const RequestParams& other) const;

    private:
        std::string m_protocol;
        VclType m_renderType;
        int m_fileId;
        std::string m_stringifiedEndpoint;
        StreamingParams m_streamingParams;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(int controlPort, int visPort);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        int getControlPort() const;
        int getVisPort() const;

        bool equals(const ReplyParams& other) const;

    private:
        int m_controlPort;
        int m_visPort;
    };
};

bool operator==(const InitProcessingSessionCmd::RequestParams& lhs, const InitProcessingSessionCmd::RequestParams& rhs);
bool operator==(const InitProcessingSessionCmd::ReplyParams& lhs, const InitProcessingSessionCmd::ReplyParams& rhs);


struct SetIsoValueCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(float value);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        float getIsoValue() const;

        bool equals(const RequestParams& other) const;

    private:
        float m_isoValue;
    };
};

bool operator==(const SetIsoValueCmd::RequestParams& lhs, const SetIsoValueCmd::RequestParams& rhs);


using InitProcessingSessionRequest = RequestTemplate<InitProcessingSessionCmd>;
using InitProcessingSessionReply = ReplyTemplate<InitProcessingSessionCmd>;

using SetIsoValueRequest = RequestTemplate<SetIsoValueCmd>;
}
}