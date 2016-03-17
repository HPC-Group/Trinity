#include "commands/IOData.h"

using namespace trinity;

const mocca::BidirectionalMap<IOData::DataType, std::string>& IOData::dataTypeMapper() {
    static mocca::BidirectionalMap<DataType, std::string> mapper;
    if (mapper.empty()) {
        mapper.insert(DataType::Dataset, "dataset");
        mapper.insert(DataType::Directory, "directory");
    }
    return mapper;
}

IOData::IOData(const std::string& name, int fileId, DataType dataType)
    : m_name(name)
    , m_fileId(fileId)
    , m_dataType(dataType) {}

void IOData::serialize(ISerialWriter& writer) const {
    writer.appendString("name", m_name);
    writer.appendInt("fileid", m_fileId);
    writer.appendString("datatype", dataTypeMapper().getByFirst(m_dataType));
}

void IOData::deserialize(const ISerialReader& reader) {
    m_name = reader.getString("name");
    m_fileId = reader.getInt32("fileid");
    m_dataType = dataTypeMapper().getBySecond(reader.getString("datatype"));
}

std::string IOData::getName() const {
    return m_name;
}

int IOData::getFileId() const {
    return m_fileId;
}

IOData::DataType IOData::getDataType() const {
    return m_dataType;
}

bool IOData::equals(const IOData& other) const {
    return m_name == other.m_name && m_fileId == other.m_fileId && m_dataType == other.m_dataType;
}

std::string IOData::toString() const {
    std::stringstream stream;
    stream << "name: " << m_name << "; fileId: " << m_fileId << "; dataType: " << dataTypeMapper().getByFirst(m_dataType);
    return stream.str();
}