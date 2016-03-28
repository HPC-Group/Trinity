#pragma once

#include "common/IRenderer.h"
#include "commands/ISerializable.h"
#include "commands/Reply.h"
#include "commands/Request.h"

#include "mocca/net/Endpoint.h"

namespace trinity {

class StreamingParams : public SerializableTemplate<StreamingParams> {
public:
    StreamingParams();
    explicit StreamingParams(int resX, int resY);

    void serialize(ISerialWriter& writer) const override;
    void deserialize(const ISerialReader& reader) override;

    int getResX() const;
    int getResY() const;

    std::string toString() const;
    bool equals(const StreamingParams& other) const;

private:
    int m_resX, m_resY;
};

bool operator==(const StreamingParams& lhs, const StreamingParams& rhs);
std::ostream& operator<<(std::ostream& os, const StreamingParams& obj);

struct InitProcessingSessionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        explicit RequestParams(const std::string& protocol, const VclType& renderType, const std::string& fileId,
                               const mocca::net::Endpoint& ioEndpoint, const StreamingParams& p);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getProtocol() const;
        mocca::net::Endpoint getIoEndpoint() const;
        std::string getFileId() const;
        VclType getRenderType() const;
        StreamingParams getStreamingParams() const;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

    private:
        std::string m_protocol;
        VclType m_renderType;
        std::string m_fileId;
        mocca::net::Endpoint m_ioEndpoint;
        StreamingParams m_streamingParams;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        explicit ReplyParams(const std::string& controlPort, const std::string& visPort);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getControlPort() const;
        std::string getVisPort() const;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

    private:
        std::string m_controlPort;
        std::string m_visPort;
    };
};

bool operator==(const InitProcessingSessionCmd::RequestParams& lhs, const InitProcessingSessionCmd::RequestParams& rhs);
bool operator==(const InitProcessingSessionCmd::ReplyParams& lhs, const InitProcessingSessionCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const InitProcessingSessionCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const InitProcessingSessionCmd::ReplyParams& obj);

using InitProcessingSessionRequest = RequestTemplate<InitProcessingSessionCmd>;
using InitProcessingSessionReply = ReplyTemplate<InitProcessingSessionCmd>;

struct SetIsoValueCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        explicit RequestParams(uint8_t surfaceIndex, float value);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        uint8_t getSurfaceIndex() const;
        float getIsoValue() const;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

    private:
        uint8_t m_surfaceIndex;
        float m_isoValue;
    };
};

bool operator==(const SetIsoValueCmd::RequestParams& lhs, const SetIsoValueCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetIsoValueCmd::RequestParams& obj);

using SetIsoValueRequest = RequestTemplate<SetIsoValueCmd>;

struct InitContextCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        explicit RequestParams(int32_t value);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

    private:
        int32_t m_dummy;
    };
};

bool operator==(const InitContextCmd::RequestParams& lhs, const InitContextCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const InitContextCmd::RequestParams& obj);

using InitContextRequest = RequestTemplate<InitContextCmd>;

struct ZoomCameraCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        explicit RequestParams(float value);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;
        float getZoom() const;

    private:
        float m_zoom;
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

bool operator==(const ZoomCameraCmd::RequestParams& lhs, const ZoomCameraCmd::RequestParams& rhs);
bool operator==(const ZoomCameraCmd::ReplyParams& lhs, const ZoomCameraCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const ZoomCameraCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const ZoomCameraCmd::ReplyParams& obj);

using ZoomCameraRequest = RequestTemplate<ZoomCameraCmd>;
using ZoomCameraReply = ReplyTemplate<ZoomCameraCmd>;

struct SetRenderModeCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(IRenderer::ERenderMode renderMode);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        IRenderer::ERenderMode getRenderMode() const;

    private:
        IRenderer::ERenderMode m_renderMode;
    };
};

bool operator==(const SetRenderModeCmd::RequestParams& lhs, const SetRenderModeCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetRenderModeCmd::RequestParams& obj);

using SetRenderModeRequest = RequestTemplate<SetRenderModeCmd>;

struct SupportsRenderModeCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(IRenderer::ERenderMode renderMode);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        IRenderer::ERenderMode getRenderMode() const;

    private:
        IRenderer::ERenderMode m_renderMode;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(bool result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        bool getResult() const;

    private:
        bool m_result;
    };
};

bool operator==(const SupportsRenderModeCmd::RequestParams& lhs, const SupportsRenderModeCmd::RequestParams& rhs);
bool operator==(const SupportsRenderModeCmd::ReplyParams& lhs, const SupportsRenderModeCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const SupportsRenderModeCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const SupportsRenderModeCmd::ReplyParams& obj);

using SupportsRenderModeRequest = RequestTemplate<SupportsRenderModeCmd>;
using SupportsRenderModeReply = ReplyTemplate<SupportsRenderModeCmd>;

struct SetActiveModalityCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint64_t modality);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint64_t getModality() const;

    private:
        uint64_t m_modality;
    };
};

bool operator==(const SetActiveModalityCmd::RequestParams& lhs, const SetActiveModalityCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetActiveModalityCmd::RequestParams& obj);

using SetActiveModalityRequest = RequestTemplate<SetActiveModalityCmd>;

struct GetActiveModalityCmd {
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
        ReplyParams(uint64_t modality);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getModality() const;

    private:
        uint64_t m_modality;
    };
};

bool operator==(const GetActiveModalityCmd::RequestParams& lhs, const GetActiveModalityCmd::RequestParams& rhs);
bool operator==(const GetActiveModalityCmd::ReplyParams& lhs, const GetActiveModalityCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetActiveModalityCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetActiveModalityCmd::ReplyParams& obj);

using GetActiveModalityRequest = RequestTemplate<GetActiveModalityCmd>;
using GetActiveModalityReply = ReplyTemplate<GetActiveModalityCmd>;

struct SetActiveTimestepCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint64_t timestep);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint64_t getTimestep() const;

    private:
        uint64_t m_timestep;
    };
};

bool operator==(const SetActiveTimestepCmd::RequestParams& lhs, const SetActiveTimestepCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetActiveTimestepCmd::RequestParams& obj);

using SetActiveTimestepRequest = RequestTemplate<SetActiveTimestepCmd>;

struct GetActiveTimestepCmd {
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
        ReplyParams(uint64_t timestep);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getTimestep() const;

    private:
        uint64_t m_timestep;
    };
};

bool operator==(const GetActiveTimestepCmd::RequestParams& lhs, const GetActiveTimestepCmd::RequestParams& rhs);
bool operator==(const GetActiveTimestepCmd::ReplyParams& lhs, const GetActiveTimestepCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetActiveTimestepCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetActiveTimestepCmd::ReplyParams& obj);

using GetActiveTimestepRequest = RequestTemplate<GetActiveTimestepCmd>;
using GetActiveTimestepReply = ReplyTemplate<GetActiveTimestepCmd>;

struct GetModalityCountProcCmd {
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
        ReplyParams(uint64_t result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getResult() const;

    private:
        uint64_t m_result;
    };
};

bool operator==(const GetModalityCountProcCmd::RequestParams& lhs, const GetModalityCountProcCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetModalityCountProcCmd::RequestParams& obj);
using GetModalityCountProcRequest = RequestTemplate<GetModalityCountProcCmd>;

bool operator==(const GetModalityCountProcCmd::ReplyParams& lhs, const GetModalityCountProcCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetModalityCountProcCmd::ReplyParams& obj);
using GetModalityCountProcReply = ReplyTemplate<GetModalityCountProcCmd>;

struct GetTimestepCountProcCmd {
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
        ReplyParams(uint64_t result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getResult() const;

    private:
        uint64_t m_result;
    };
};

bool operator==(const GetTimestepCountProcCmd::RequestParams& lhs, const GetTimestepCountProcCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetTimestepCountProcCmd::RequestParams& obj);
using GetTimestepCountProcRequest = RequestTemplate<GetTimestepCountProcCmd>;

bool operator==(const GetTimestepCountProcCmd::ReplyParams& lhs, const GetTimestepCountProcCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetTimestepCountProcCmd::ReplyParams& obj);
using GetTimestepCountProcReply = ReplyTemplate<GetTimestepCountProcCmd>;

struct Set1DTransferFunctionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const TransferFunction1D& tf);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        TransferFunction1D getTf() const;

    private:
        TransferFunction1D m_tf;
    };
};

bool operator==(const Set1DTransferFunctionCmd::RequestParams& lhs, const Set1DTransferFunctionCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const Set1DTransferFunctionCmd::RequestParams& obj);
using Set1DTransferFunctionRequest = RequestTemplate<Set1DTransferFunctionCmd>;

struct GetDefault1DTransferFunctionProcCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint64_t index);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint64_t getIndex() const;

    private:
        uint64_t m_index;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const TransferFunction1D& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        TransferFunction1D getResult() const;

    private:
        TransferFunction1D m_result;
    };
};

bool operator==(const GetDefault1DTransferFunctionProcCmd::RequestParams& lhs,
                const GetDefault1DTransferFunctionProcCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionProcCmd::RequestParams& obj);
using GetDefault1DTransferFunctionProcRequest = RequestTemplate<GetDefault1DTransferFunctionProcCmd>;

bool operator==(const GetDefault1DTransferFunctionProcCmd::ReplyParams& lhs, const GetDefault1DTransferFunctionProcCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionProcCmd::ReplyParams& obj);
using GetDefault1DTransferFunctionProcReply = ReplyTemplate<GetDefault1DTransferFunctionProcCmd>;

struct GetDefault1DTransferFunctionCountProcCmd {
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
        ReplyParams(uint64_t result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getResult() const;

    private:
        uint64_t m_result;
    };
};

bool operator==(const GetDefault1DTransferFunctionCountProcCmd::RequestParams& lhs,
                const GetDefault1DTransferFunctionCountProcCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCountProcCmd::RequestParams& obj);
using GetDefault1DTransferFunctionCountProcRequest = RequestTemplate<GetDefault1DTransferFunctionCountProcCmd>;

bool operator==(const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& lhs,
                const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetDefault1DTransferFunctionCountProcCmd::ReplyParams& obj);
using GetDefault1DTransferFunctionCountProcReply = ReplyTemplate<GetDefault1DTransferFunctionCountProcCmd>;

struct Get1DHistogramProcCmd {
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
        ReplyParams(const std::vector<uint64_t>& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        std::vector<uint64_t> getResult() const;

    private:
        std::vector<uint64_t> m_result;
    };
};

bool operator==(const Get1DHistogramProcCmd::RequestParams& lhs, const Get1DHistogramProcCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const Get1DHistogramProcCmd::RequestParams& obj);
using Get1DHistogramProcRequest = RequestTemplate<Get1DHistogramProcCmd>;

bool operator==(const Get1DHistogramProcCmd::ReplyParams& lhs, const Get1DHistogramProcCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const Get1DHistogramProcCmd::ReplyParams& obj);
using Get1DHistogramProcReply = ReplyTemplate<Get1DHistogramProcCmd>;

struct GetIsoValueCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint8_t surfaceIndex);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint8_t getSurfaceIndex() const;

    private:
        uint8_t m_surfaceIndex;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(float result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        float getResult() const;

    private:
        float m_result;
    };
};

bool operator==(const GetIsoValueCmd::RequestParams& lhs, const GetIsoValueCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetIsoValueCmd::RequestParams& obj);
using GetIsoValueRequest = RequestTemplate<GetIsoValueCmd>;

bool operator==(const GetIsoValueCmd::ReplyParams& lhs, const GetIsoValueCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetIsoValueCmd::ReplyParams& obj);
using GetIsoValueReply = ReplyTemplate<GetIsoValueCmd>;

struct SetIsosurfaceColorCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint8_t surfaceIndex, const Core::Math::Vec3ui8& vColor);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint8_t getSurfaceIndex() const;
        Core::Math::Vec3ui8 getVColor() const;

    private:
        uint8_t m_surfaceIndex;
        Core::Math::Vec3ui8 m_vColor;
    };
};

bool operator==(const SetIsosurfaceColorCmd::RequestParams& lhs, const SetIsosurfaceColorCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetIsosurfaceColorCmd::RequestParams& obj);
using SetIsosurfaceColorRequest = RequestTemplate<SetIsosurfaceColorCmd>;

struct GetIsosurfaceColorCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint8_t surfaceIndex);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint8_t getSurfaceIndex() const;

    private:
        uint8_t m_surfaceIndex;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const Core::Math::Vec3ui8& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui8 getResult() const;

    private:
        Core::Math::Vec3ui8 m_result;
    };
};

bool operator==(const GetIsosurfaceColorCmd::RequestParams& lhs, const GetIsosurfaceColorCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetIsosurfaceColorCmd::RequestParams& obj);
using GetIsosurfaceColorRequest = RequestTemplate<GetIsosurfaceColorCmd>;

bool operator==(const GetIsosurfaceColorCmd::ReplyParams& lhs, const GetIsosurfaceColorCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetIsosurfaceColorCmd::ReplyParams& obj);
using GetIsosurfaceColorReply = ReplyTemplate<GetIsosurfaceColorCmd>;

struct GetRangeProcCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint64_t modality);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint64_t getModality() const;

    private:
        uint64_t m_modality;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const Core::Math::Vec2f& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec2f getResult() const;

    private:
        Core::Math::Vec2f m_result;
    };
};

bool operator==(const GetRangeProcCmd::RequestParams& lhs, const GetRangeProcCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetRangeProcCmd::RequestParams& obj);
using GetRangeProcRequest = RequestTemplate<GetRangeProcCmd>;

bool operator==(const GetRangeProcCmd::ReplyParams& lhs, const GetRangeProcCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetRangeProcCmd::ReplyParams& obj);
using GetRangeProcReply = ReplyTemplate<GetRangeProcCmd>;

struct SetClearViewPositionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const Core::Math::Vec2f& vNormalizedWindowPos);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        Core::Math::Vec2f getVNormalizedWindowPos() const;

    private:
        Core::Math::Vec2f m_vNormalizedWindowPos;
    };
};

bool operator==(const SetClearViewPositionCmd::RequestParams& lhs, const SetClearViewPositionCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetClearViewPositionCmd::RequestParams& obj);
using SetClearViewPositionRequest = RequestTemplate<SetClearViewPositionCmd>;

struct SetClearViewRadiusCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(float radius);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        float getRadius() const;

    private:
        float m_radius;
    };
};

bool operator==(const SetClearViewRadiusCmd::RequestParams& lhs, const SetClearViewRadiusCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetClearViewRadiusCmd::RequestParams& obj);
using SetClearViewRadiusRequest = RequestTemplate<SetClearViewRadiusCmd>;

struct SetClearBorderSizeCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(float borderSize);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        float getBorderSize() const;

    private:
        float m_borderSize;
    };
};

bool operator==(const SetClearBorderSizeCmd::RequestParams& lhs, const SetClearBorderSizeCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetClearBorderSizeCmd::RequestParams& obj);
using SetClearBorderSizeRequest = RequestTemplate<SetClearBorderSizeCmd>;

struct EnableLightingCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(bool enable);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        bool getEnable() const;

    private:
        bool m_enable;
    };
};

bool operator==(const EnableLightingCmd::RequestParams& lhs, const EnableLightingCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const EnableLightingCmd::RequestParams& obj);
using EnableLightingRequest = RequestTemplate<EnableLightingCmd>;

struct SetLightingColorsCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const IRenderer::PhongColorTriple& colors);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        IRenderer::PhongColorTriple getColors() const;

    private:
        IRenderer::PhongColorTriple m_colors;
    };
};

bool operator==(const SetLightingColorsCmd::RequestParams& lhs, const SetLightingColorsCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetLightingColorsCmd::RequestParams& obj);
using SetLightingColorsRequest = RequestTemplate<SetLightingColorsCmd>;

struct GetLightingColorsCmd {
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
        ReplyParams(const IRenderer::PhongColorTriple& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        IRenderer::PhongColorTriple getResult() const;

    private:
        IRenderer::PhongColorTriple m_result;
    };
};

bool operator==(const GetLightingColorsCmd::RequestParams& lhs, const GetLightingColorsCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetLightingColorsCmd::RequestParams& obj);
using GetLightingColorsRequest = RequestTemplate<GetLightingColorsCmd>;

bool operator==(const GetLightingColorsCmd::ReplyParams& lhs, const GetLightingColorsCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetLightingColorsCmd::ReplyParams& obj);
using GetLightingColorsReply = ReplyTemplate<GetLightingColorsCmd>;

struct SetLightDirectionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const Core::Math::Vec3f& direction);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        Core::Math::Vec3f getDirection() const;

    private:
        Core::Math::Vec3f m_direction;
    };
};

bool operator==(const SetLightDirectionCmd::RequestParams& lhs, const SetLightDirectionCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetLightDirectionCmd::RequestParams& obj);
using SetLightDirectionRequest = RequestTemplate<SetLightDirectionCmd>;

struct GetLightDirectionCmd {
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
        ReplyParams(const Core::Math::Vec3f& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3f getResult() const;

    private:
        Core::Math::Vec3f m_result;
    };
};

bool operator==(const GetLightDirectionCmd::RequestParams& lhs, const GetLightDirectionCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetLightDirectionCmd::RequestParams& obj);
using GetLightDirectionRequest = RequestTemplate<GetLightDirectionCmd>;

bool operator==(const GetLightDirectionCmd::ReplyParams& lhs, const GetLightDirectionCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetLightDirectionCmd::ReplyParams& obj);
using GetLightDirectionReply = ReplyTemplate<GetLightDirectionCmd>;

struct SetSampleRateModifierCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(float fSampleRateModifier);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        float getFSampleRateModifier() const;

    private:
        float m_fSampleRateModifier;
    };
};

bool operator==(const SetSampleRateModifierCmd::RequestParams& lhs, const SetSampleRateModifierCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetSampleRateModifierCmd::RequestParams& obj);
using SetSampleRateModifierRequest = RequestTemplate<SetSampleRateModifierCmd>;

struct GetSampleRateModifierCmd {
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
        ReplyParams(float result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        float getResult() const;

    private:
        float m_result;
    };
};

bool operator==(const GetSampleRateModifierCmd::RequestParams& lhs, const GetSampleRateModifierCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetSampleRateModifierCmd::RequestParams& obj);
using GetSampleRateModifierRequest = RequestTemplate<GetSampleRateModifierCmd>;

bool operator==(const GetSampleRateModifierCmd::ReplyParams& lhs, const GetSampleRateModifierCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetSampleRateModifierCmd::ReplyParams& obj);
using GetSampleRateModifierReply = ReplyTemplate<GetSampleRateModifierCmd>;

struct SetBoundingBoxModeCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(IRenderer::BBoxMode mode);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        IRenderer::BBoxMode getMode() const;

    private:
        IRenderer::BBoxMode m_mode;
    };
};

bool operator==(const SetBoundingBoxModeCmd::RequestParams& lhs, const SetBoundingBoxModeCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetBoundingBoxModeCmd::RequestParams& obj);
using SetBoundingBoxModeRequest = RequestTemplate<SetBoundingBoxModeCmd>;

struct GetBoundingBoxModeCmd {
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
        ReplyParams(const IRenderer::BBoxMode& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        IRenderer::BBoxMode getResult() const;

    private:
        IRenderer::BBoxMode m_result;
    };
};

bool operator==(const GetBoundingBoxModeCmd::RequestParams& lhs, const GetBoundingBoxModeCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBoundingBoxModeCmd::RequestParams& obj);
using GetBoundingBoxModeRequest = RequestTemplate<GetBoundingBoxModeCmd>;

bool operator==(const GetBoundingBoxModeCmd::ReplyParams& lhs, const GetBoundingBoxModeCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBoundingBoxModeCmd::ReplyParams& obj);
using GetBoundingBoxModeReply = ReplyTemplate<GetBoundingBoxModeCmd>;

struct SetRendererSpecialsCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const std::vector<uint64_t>& params);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        std::vector<uint64_t> getParams() const;

    private:
        std::vector<uint64_t> m_params;
    };
};

bool operator==(const SetRendererSpecialsCmd::RequestParams& lhs, const SetRendererSpecialsCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const SetRendererSpecialsCmd::RequestParams& obj);
using SetRendererSpecialsRequest = RequestTemplate<SetRendererSpecialsCmd>;

struct GetRendererSpecialsCmd {
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
        ReplyParams(const std::vector<uint64_t>& result);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        std::vector<uint64_t> getResult() const;

    private:
        std::vector<uint64_t> m_result;
    };
};

bool operator==(const GetRendererSpecialsCmd::RequestParams& lhs, const GetRendererSpecialsCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetRendererSpecialsCmd::RequestParams& obj);
using GetRendererSpecialsRequest = RequestTemplate<GetRendererSpecialsCmd>;

bool operator==(const GetRendererSpecialsCmd::ReplyParams& lhs, const GetRendererSpecialsCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetRendererSpecialsCmd::ReplyParams& obj);
using GetRendererSpecialsReply = ReplyTemplate<GetRendererSpecialsCmd>;

/* AUTOGEN CommandHeader */
}
