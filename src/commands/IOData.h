#pragma once

#include "commands/ISerializable.h"

#include "mocca/base/BidirectionalMap.h"

namespace trinity {

    class IOData : public SerializableTemplate<IOData> {
    public:
        enum class DataType { Dataset = 0, Directory = 1 };
        static const mocca::BidirectionalMap<DataType, std::string>& dataTypeMapper();

        IOData() = default;
        IOData(const std::string& name, const std::string&  fileId, DataType dataType);

        void serialize(ISerialWriter& writer) const override;
        void deserialize(const ISerialReader& reader) override;

        std::string getName() const;
        std::string getFileId() const;
        DataType getDataType() const;

        std::string toString() const;
        bool equals(const IOData& other) const;

    private:
        std::string m_name;
        std::string m_fileId;
        DataType m_dataType;
    };

    bool operator==(const IOData& lhs, const IOData& rhs);
    std::ostream& operator<<(std::ostream& os, const IOData& obj);
}