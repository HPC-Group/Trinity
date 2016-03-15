#pragma once

#include "commands/ISerializable.h"
#include "commands/Request.h"
#include "commands/Reply.h"

#include "silverbullet/math/Vectors.h"

#include "mocca/base/BidirectionalMap.h"


namespace trinity {

class IOData : public SerializableTemplate<IOData> {
public:
    enum class DataType { Dataset = 0, Directory = 1 };
    static const mocca::BidirectionalMap<DataType, std::string>& dataTypeMapper();
    
    IOData() = default;
    IOData(const std::string& name, int fileId, DataType dataType);

    void serialize(ISerialWriter& writer) const override;
    void deserialize(const ISerialReader& reader) override;

    std::string getName() const;
    int getFileId() const;
    DataType getDataType() const;

    std::string toString() const;
    bool equals(const IOData& other) const;

private:
    std::string m_name;
    int m_fileId;
    DataType m_dataType;
};

bool operator==(const IOData& lhs, const IOData& rhs);
std::ostream& operator<<(std::ostream& os, const IOData& obj);

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

#undef PYTHON_MAGIC

using ListFilesRequest = RequestTemplate<ListFilesCmd>;
using ListFilesReply = ReplyTemplate<ListFilesCmd>;

using InitIOSessionRequest = RequestTemplate<InitIOSessionCmd>;
using InitIOSessionReply = ReplyTemplate<InitIOSessionCmd>;

using GetLODLevelCountRequest = RequestTemplate<GetLODLevelCountCmd>;
using GetLODLevelCountReply = ReplyTemplate<GetLODLevelCountCmd>;
}