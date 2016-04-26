#include "commands/BrickMetaData.h"

#include "commands/ISerialReader.h"
#include "commands/ISerialWriter.h"

using namespace trinity;

std::vector<std::shared_ptr<std::vector<uint8_t>>> BrickMetaData::createBinary(const std::vector<BrickMetaData>& metaDataVec) {
    static size_t metaDataSize = 4 * sizeof(double) + 3 * sizeof(uint32_t);
    static size_t dataPerPart = 1024 * 1024;
    static size_t binaryPartSize = dataPerPart * metaDataSize;
    size_t numBinaryParts = metaDataVec.size() / dataPerPart + 1;
    
    std::vector<std::shared_ptr<std::vector<uint8_t>>> result;
    for (size_t i = 0; i < numBinaryParts - 1; ++i) {
        auto part = std::make_shared<std::vector<uint8_t>>();
        part->reserve(binaryPartSize);
        result.push_back(part);
    }
    auto part = std::make_shared<std::vector<uint8_t>>();
    part->reserve((metaDataVec.size() % dataPerPart) * metaDataSize);
    result.push_back(part);

    for (size_t i = 0; i < metaDataVec.size(); ++i) {
        auto& messagePart = *result[i * metaDataSize / binaryPartSize];
        const auto& metaData = metaDataVec[i];
        mocca::net::appendToMessagePart(messagePart, metaData.minScalar);
        mocca::net::appendToMessagePart(messagePart, metaData.maxScalar);
        mocca::net::appendToMessagePart(messagePart, metaData.minGradient);
        mocca::net::appendToMessagePart(messagePart, metaData.maxGradient);
        mocca::net::appendToMessagePart(messagePart, metaData.voxelSize.x);
        mocca::net::appendToMessagePart(messagePart, metaData.voxelSize.y);
        mocca::net::appendToMessagePart(messagePart, metaData.voxelSize.z);
    }

    return result;
}

std::vector<BrickMetaData> BrickMetaData::createFromBinary(const std::vector<std::shared_ptr<std::vector<uint8_t>>>& binary) {
    static size_t metaDataSize = 4 * sizeof(double) + 3 * sizeof(uint32_t);
    static size_t dataPerPart = 1024 * 1024;
    static size_t binaryPartSize = dataPerPart * metaDataSize;
    size_t numElements = (binary.size() - 1) * dataPerPart + binary.back()->size() / metaDataSize;

    std::vector<BrickMetaData> result;
    for (size_t i = 0; i < numElements; ++i) {
        const uint8_t* ptr = binary[i / dataPerPart]->data() + (i % dataPerPart) * metaDataSize;
        result.push_back(BrickMetaData());
        ptr = mocca::net::readFromMessagePart(ptr, result.back().minScalar);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().maxScalar);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().minGradient);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().maxGradient);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().voxelSize.x);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().voxelSize.y);
        ptr = mocca::net::readFromMessagePart(ptr, result.back().voxelSize.z);
    }
    return result;
}
