#pragma once

#include "commands/IOData.h"
#include "commands/ISerializable.h"
#include "commands/Reply.h"
#include "commands/Request.h"

#include "silverbullet/dataio/base/Brick.h"
#include "silverbullet/math/MinMaxBlock.h"
#include "silverbullet/math/Vectors.h"


namespace trinity {

struct ListFilesCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        explicit RequestParams(int32_t dirID);

        bool equals(const ListFilesCmd::RequestParams& other) const;
        std::string toString() const;

        int32_t getDirID() const;

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

    private:
        int32_t m_dirID;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        explicit ReplyParams(const std::vector<IOData>& ioData);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::vector<IOData> getIOData() const;

        std::string toString() const;
        bool equals(const ListFilesCmd::ReplyParams& other) const;

    private:
        std::vector<IOData> m_ioData;
    };
};

bool operator==(const ListFilesCmd::RequestParams& lhs, const ListFilesCmd::RequestParams& rhs);
std::ostream& operator<<(std::ostream& os, const ListFilesCmd::RequestParams& obj);
bool operator==(const ListFilesCmd::ReplyParams& lhs, const ListFilesCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const ListFilesCmd::ReplyParams& obj);

struct InitIOSessionCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        explicit RequestParams(const std::string& protocol, int fileId);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getProtocol() const;
        int getFileId() const;

        std::string toString() const;
        bool equals(const InitIOSessionCmd::RequestParams& other) const;

    private:
        std::string m_protocol;
        int m_fileId;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        explicit ReplyParams(const std::string& controlPort);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getControlPort() const;

        std::string toString() const;
        bool equals(const InitIOSessionCmd::ReplyParams& other) const;

    private:
        std::string m_controlPort;
    };
};

bool operator==(const InitIOSessionCmd::RequestParams& lhs, const InitIOSessionCmd::RequestParams& rhs);
bool operator==(const InitIOSessionCmd::ReplyParams& lhs, const InitIOSessionCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const InitIOSessionCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const InitIOSessionCmd::ReplyParams& obj);

struct GetLODLevelCountCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;

        std::string toString() const;
        bool equals(const GetLODLevelCountCmd::RequestParams& other) const;

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        explicit ReplyParams(int lodCount);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        int getLODLevelCount() const;

        std::string toString() const;
        bool equals(const GetLODLevelCountCmd::ReplyParams& other) const;

    private:
        int m_lodCount;
    };
};

bool operator==(const GetLODLevelCountCmd::RequestParams& lhs, const GetLODLevelCountCmd::RequestParams& rhs);
bool operator==(const GetLODLevelCountCmd::ReplyParams& lhs, const GetLODLevelCountCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetLODLevelCountCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetLODLevelCountCmd::ReplyParams& obj);

#define PYTHON_MAGIC


struct GetMaxBrickSizeCmd {
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
        ReplyParams(const Core::Math::Vec3ui64& maxBrickSize);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui64 getMaxBrickSize() const;

    private:
        Core::Math::Vec3ui64 m_maxBrickSize;
    };
};

bool operator==(const GetMaxBrickSizeCmd::RequestParams& lhs, const GetMaxBrickSizeCmd::RequestParams& rhs);
bool operator==(const GetMaxBrickSizeCmd::ReplyParams& lhs, const GetMaxBrickSizeCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetMaxBrickSizeCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetMaxBrickSizeCmd::ReplyParams& obj);

using GetMaxBrickSizeRequest = RequestTemplate<GetMaxBrickSizeCmd>;
using GetMaxBrickSizeReply = ReplyTemplate<GetMaxBrickSizeCmd>;

struct GetMaxUsedBrickSizesCmd {
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
        ReplyParams(const Core::Math::Vec3ui64& maxusedBrickSizes);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui64 getMaxUsedBrickSizes() const;

    private:
        Core::Math::Vec3ui64 m_maxUsedBrickSizes;
    };
};

bool operator==(const GetMaxUsedBrickSizesCmd::RequestParams& lhs, const GetMaxUsedBrickSizesCmd::RequestParams& rhs);
bool operator==(const GetMaxUsedBrickSizesCmd::ReplyParams& lhs, const GetMaxUsedBrickSizesCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetMaxUsedBrickSizesCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetMaxUsedBrickSizesCmd::ReplyParams& obj);

using GetMaxUsedBrickSizesRequest = RequestTemplate<GetMaxUsedBrickSizesCmd>;
using GetMaxUsedBrickSizesReply = ReplyTemplate<GetMaxUsedBrickSizesCmd>;

struct MaxMinForKeyCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const BrickKey& brickKey);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        BrickKey getBrickKey() const;

    private:
        BrickKey m_brickKey;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const MinMaxBlock& minMaxBlock);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        MinMaxBlock getMinMaxBlock() const;

    private:
        MinMaxBlock m_minMaxBock;
    };
};

bool operator==(const MaxMinForKeyCmd::RequestParams& lhs, const MaxMinForKeyCmd::RequestParams& rhs);
bool operator==(const MaxMinForKeyCmd::ReplyParams& lhs, const MaxMinForKeyCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const MaxMinForKeyCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const MaxMinForKeyCmd::ReplyParams& obj);

using MaxMinForKeyRequest = RequestTemplate<MaxMinForKeyCmd>;
using MaxMinForKeyReply = ReplyTemplate<MaxMinForKeyCmd>;

struct GetNumberOfTimestepsCmd {
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
        ReplyParams(uint64_t numberOfTimesteps);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getNumberOfTimesteps() const;

    private:
        uint64_t m_numberOfTimesteps;
    };
};

bool operator==(const GetNumberOfTimestepsCmd::RequestParams& lhs, const GetNumberOfTimestepsCmd::RequestParams& rhs);
bool operator==(const GetNumberOfTimestepsCmd::ReplyParams& lhs, const GetNumberOfTimestepsCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetNumberOfTimestepsCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetNumberOfTimestepsCmd::ReplyParams& obj);

using GetNumberOfTimestepsRequest = RequestTemplate<GetNumberOfTimestepsCmd>;
using GetNumberOfTimestepsReply = ReplyTemplate<GetNumberOfTimestepsCmd>;

struct GetDomainSizeCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams(uint64_t lod = 0, uint64_t modality = 0);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint64_t getLod() const;
        uint64_t getModality() const;

    private:
        uint64_t m_lod;
        uint64_t m_modality;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const Core::Math::Vec3ui64& domainSize);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui64 getDomainSize() const;

    private:
        Core::Math::Vec3ui64 m_domainSize;
    };
};

bool operator==(const GetDomainSizeCmd::RequestParams& lhs, const GetDomainSizeCmd::RequestParams& rhs);
bool operator==(const GetDomainSizeCmd::ReplyParams& lhs, const GetDomainSizeCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetDomainSizeCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetDomainSizeCmd::ReplyParams& obj);

using GetDomainSizeRequest = RequestTemplate<GetDomainSizeCmd>;
using GetDomainSizeReply = ReplyTemplate<GetDomainSizeCmd>;

struct GetTransformationCmd {
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
        ReplyParams(const Core::Math::Mat4d& transformation);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Mat4d getTransformation() const;

    private:
        Core::Math::Mat4d m_transformation;
    };
};

bool operator==(const GetTransformationCmd::RequestParams& lhs, const GetTransformationCmd::RequestParams& rhs);
bool operator==(const GetTransformationCmd::ReplyParams& lhs, const GetTransformationCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetTransformationCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetTransformationCmd::ReplyParams& obj);

using GetTransformationRequest = RequestTemplate<GetTransformationCmd>;
using GetTransformationReply = ReplyTemplate<GetTransformationCmd>;

struct GetBrickOverlapSizeCmd {
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
        ReplyParams(const Core::Math::Vec3ui& overlapSize);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui getOverlapSize() const;

    private:
        Core::Math::Vec3ui m_overlapSize;
    };
};

bool operator==(const GetBrickOverlapSizeCmd::RequestParams& lhs, const GetBrickOverlapSizeCmd::RequestParams& rhs);
bool operator==(const GetBrickOverlapSizeCmd::ReplyParams& lhs, const GetBrickOverlapSizeCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBrickOverlapSizeCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetBrickOverlapSizeCmd::ReplyParams& obj);

using GetBrickOverlapSizeRequest = RequestTemplate<GetBrickOverlapSizeCmd>;
using GetBrickOverlapSizeReply = ReplyTemplate<GetBrickOverlapSizeCmd>;

struct GetLargestSingleBrickLODCmd {
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
        ReplyParams(uint64_t largestSingleBrickLOD);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getLargestSingleBrickLOD() const;

    private:
        uint64_t m_largestSingleBrickLOD;
    };
};

bool operator==(const GetLargestSingleBrickLODCmd::RequestParams& lhs, const GetLargestSingleBrickLODCmd::RequestParams& rhs);
bool operator==(const GetLargestSingleBrickLODCmd::ReplyParams& lhs, const GetLargestSingleBrickLODCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetLargestSingleBrickLODCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetLargestSingleBrickLODCmd::ReplyParams& obj);

using GetLargestSingleBrickLODRequest = RequestTemplate<GetLargestSingleBrickLODCmd>;
using GetLargestSingleBrickLODReply = ReplyTemplate<GetLargestSingleBrickLODCmd>;

struct GetBrickVoxelCountsCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const BrickKey& brickKey);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        BrickKey getBrickKey() const;

    private:
        BrickKey m_brickKey;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const Core::Math::Vec3ui& brickVoxelCounts);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui getBrickVoxelCounts() const;

    private:
        Core::Math::Vec3ui m_brickVoxelCounts;
    };
};

bool operator==(const GetBrickVoxelCountsCmd::RequestParams& lhs, const GetBrickVoxelCountsCmd::RequestParams& rhs);
bool operator==(const GetBrickVoxelCountsCmd::ReplyParams& lhs, const GetBrickVoxelCountsCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBrickVoxelCountsCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetBrickVoxelCountsCmd::ReplyParams& obj);

using GetBrickVoxelCountsRequest = RequestTemplate<GetBrickVoxelCountsCmd>;
using GetBrickVoxelCountsReply = ReplyTemplate<GetBrickVoxelCountsCmd>;

struct GetBrickExtentsCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const BrickKey& brickKey);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        BrickKey getBrickKey() const;

    private:
        BrickKey m_brickKey;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const Core::Math::Vec3f& brickExtents);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3f getBrickExtents() const;

    private:
        Core::Math::Vec3f m_brickExtents;
    };
};

bool operator==(const GetBrickExtentsCmd::RequestParams& lhs, const GetBrickExtentsCmd::RequestParams& rhs);
bool operator==(const GetBrickExtentsCmd::ReplyParams& lhs, const GetBrickExtentsCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBrickExtentsCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetBrickExtentsCmd::ReplyParams& obj);

using GetBrickExtentsRequest = RequestTemplate<GetBrickExtentsCmd>;
using GetBrickExtentsReply = ReplyTemplate<GetBrickExtentsCmd>;

struct GetBrickLayoutCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(uint64_t lod, uint64_t modality);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        uint64_t getLod() const;
        uint64_t getModality() const;

    private:
        uint64_t m_lod;
        uint64_t m_modality;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(const Core::Math::Vec3ui& brickLayout);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec3ui getBrickLayout() const;

    private:
        Core::Math::Vec3ui m_brickLayout;
    };
};

bool operator==(const GetBrickLayoutCmd::RequestParams& lhs, const GetBrickLayoutCmd::RequestParams& rhs);
bool operator==(const GetBrickLayoutCmd::ReplyParams& lhs, const GetBrickLayoutCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBrickLayoutCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetBrickLayoutCmd::ReplyParams& obj);

using GetBrickLayoutRequest = RequestTemplate<GetBrickLayoutCmd>;
using GetBrickLayoutReply = ReplyTemplate<GetBrickLayoutCmd>;

struct GetModalityCountCmd {
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
        ReplyParams(uint64_t modalityCount);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getModalityCount() const;

    private:
        uint64_t m_modalityCount;
    };
};

bool operator==(const GetModalityCountCmd::RequestParams& lhs, const GetModalityCountCmd::RequestParams& rhs);
bool operator==(const GetModalityCountCmd::ReplyParams& lhs, const GetModalityCountCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetModalityCountCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetModalityCountCmd::ReplyParams& obj);

using GetModalityCountRequest = RequestTemplate<GetModalityCountCmd>;
using GetModalityCountReply = ReplyTemplate<GetModalityCountCmd>;

struct GetComponentCountCmd {
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
        ReplyParams(uint64_t componentCount);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getComponentCount() const;

    private:
        uint64_t m_componentCount;
    };
};

bool operator==(const GetComponentCountCmd::RequestParams& lhs, const GetComponentCountCmd::RequestParams& rhs);
bool operator==(const GetComponentCountCmd::ReplyParams& lhs, const GetComponentCountCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetComponentCountCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetComponentCountCmd::ReplyParams& obj);

using GetComponentCountRequest = RequestTemplate<GetComponentCountCmd>;
using GetComponentCountReply = ReplyTemplate<GetComponentCountCmd>;

struct GetRangeCmd {
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
        ReplyParams(const Core::Math::Vec2f& range);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        Core::Math::Vec2f getRange() const;

    private:
        Core::Math::Vec2f m_range;
    };
};

bool operator==(const GetRangeCmd::RequestParams& lhs, const GetRangeCmd::RequestParams& rhs);
bool operator==(const GetRangeCmd::ReplyParams& lhs, const GetRangeCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetRangeCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetRangeCmd::ReplyParams& obj);

using GetRangeRequest = RequestTemplate<GetRangeCmd>;
using GetRangeReply = ReplyTemplate<GetRangeCmd>;

struct GetTotalBrickCountCmd {
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
        ReplyParams(uint64_t totalBrickCount);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        uint64_t getTotalBrickCount() const;

    private:
        uint64_t m_totalBrickCount;
    };
};

bool operator==(const GetTotalBrickCountCmd::RequestParams& lhs, const GetTotalBrickCountCmd::RequestParams& rhs);
bool operator==(const GetTotalBrickCountCmd::ReplyParams& lhs, const GetTotalBrickCountCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetTotalBrickCountCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetTotalBrickCountCmd::ReplyParams& obj);

using GetTotalBrickCountRequest = RequestTemplate<GetTotalBrickCountCmd>;
using GetTotalBrickCountReply = ReplyTemplate<GetTotalBrickCountCmd>;

struct GetBrickCmd {
    static VclType Type;

    class RequestParams : public SerializableTemplate<RequestParams> {
    public:
        RequestParams() = default;
        RequestParams(const BrickKey& brickKey);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const RequestParams& other) const;

        BrickKey getBrickKey() const;

    private:
        BrickKey m_brickKey;
    };

    class ReplyParams : public SerializableTemplate<ReplyParams> {
    public:
        ReplyParams() = default;
        ReplyParams(std::vector<uint8_t> brick, bool success);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string toString() const;
        bool equals(const ReplyParams& other) const;

        bool getSuccess() const;
        const std::vector<uint8_t>& getBrick() const; // returning reference for performance reasons, be careful!

    private:
        bool m_success;
        std::vector<uint8_t> m_brick;
    };
};

bool operator==(const GetBrickCmd::RequestParams& lhs, const GetBrickCmd::RequestParams& rhs);
bool operator==(const GetBrickCmd::ReplyParams& lhs, const GetBrickCmd::ReplyParams& rhs);
std::ostream& operator<<(std::ostream& os, const GetBrickCmd::RequestParams& obj);
std::ostream& operator<<(std::ostream& os, const GetBrickCmd::ReplyParams& obj);

using GetBrickRequest = RequestTemplate<GetBrickCmd>;
using GetBrickReply = ReplyTemplate<GetBrickCmd>;

#undef PYTHON_MAGIC

using ListFilesRequest = RequestTemplate<ListFilesCmd>;
using ListFilesReply = ReplyTemplate<ListFilesCmd>;

using InitIOSessionRequest = RequestTemplate<InitIOSessionCmd>;
using InitIOSessionReply = ReplyTemplate<InitIOSessionCmd>;

using GetLODLevelCountRequest = RequestTemplate<GetLODLevelCountCmd>;
using GetLODLevelCountReply = ReplyTemplate<GetLODLevelCountCmd>;
}