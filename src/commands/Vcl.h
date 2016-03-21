

#pragma once

#include <map>
#include <memory>

#include "mocca/base/BidirectionalMap.h"
#include "mocca/base/ByteArray.h"
#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"


namespace trinity {

// visualization command language types
// everything that is part of trinity protocol has to be specified here
// e.g., each command, each passed struct/class
enum class VclType {
    InitRenderer,
    TrinityError,
    DummyRenderer,
    SimpleRenderer,
    FractalIO,
    SetIsoValue,
    GetLODLevelCount,
    ListFiles,
    InitIOSession,
    StreamingParams,
    IOData,
    TrinityFile,
    TrinityDirectory,
    InitContext,
    ZoomCamera,
    Paint,
#define PYTHON_ENUM
    GetTransformation,
    GetBrickOverlapSize,
    GetLargestSingleBrickLOD,
    GetBrickVoxelCounts,
    GetBrickExtents,
    GetBrickLayout,
    GetModalityCount,
    GetComponentCount,
    GetRange,
    GetTotalBrickCount,
    GetBrick,
    GetType,
    GetSemantic,

#undef PYTHON_ENUM
    GetMaxBrickSize,
    GetMaxUsedBrickSizes,
    MaxMinForKey,
    GetNumberOfTimesteps,
    GetDomainSize,
    First = InitRenderer,
    Last = GetDomainSize,
};

VclType& operator++(VclType& x);
VclType operator*(VclType c);
VclType begin(VclType r);
VclType end(VclType r);
std::ostream& operator<<(std::ostream& os, VclType obj);

// use this class to create and parse trinity commands
class Vcl {
public:
    static const Vcl& instance();

    // throws Error if entry not found
    std::string toString(const VclType& t) const;

    // throws Error if entry not found
    VclType toType(const std::string& str) const;

    // todo test
    std::string toString(const int errorCode) const;

private:
    // keep these synchronized with the VclType enums
    Vcl() {
        m_cmdMap.insert("InitRenderer", VclType::InitRenderer);
        m_cmdMap.insert("DummyRenderer", VclType::DummyRenderer);
        m_cmdMap.insert("SimpleRenderer", VclType::SimpleRenderer);
        m_cmdMap.insert("TrinityError", VclType::TrinityError);
        m_cmdMap.insert("StreamingParams", VclType::StreamingParams);
        m_cmdMap.insert("SetIsoValue", VclType::SetIsoValue);
        m_cmdMap.insert("InitIOSession", VclType::InitIOSession);
        m_cmdMap.insert("ListFiles", VclType::ListFiles);
        m_cmdMap.insert("GetLODLevelCount", VclType::GetLODLevelCount);
        m_cmdMap.insert("FractalIO", VclType::FractalIO);
        m_cmdMap.insert("TrinityDirectory", VclType::TrinityDirectory);
        m_cmdMap.insert("TrinityFile", VclType::TrinityFile);
        m_cmdMap.insert("IOData", VclType::IOData);
        m_cmdMap.insert("InitContext", VclType::InitContext);
        m_cmdMap.insert("Paint", VclType::Paint);
        m_cmdMap.insert("ZoomCamera", VclType::ZoomCamera);
        m_cmdMap.insert("GetMaxBrickSize", VclType::GetMaxBrickSize);
        m_cmdMap.insert("GetMaxUsedBrickSizes", VclType::GetMaxUsedBrickSizes);
        m_cmdMap.insert("MaxMinForKey", VclType::MaxMinForKey);
        m_cmdMap.insert("GetNumberOfTimesteps", VclType::GetNumberOfTimesteps);
        m_cmdMap.insert("GetDomainSize", VclType::GetDomainSize);
#define PYTHON_MAGIC_STRING
        m_cmdMap.insert("GetTransformation", VclType::GetTransformation);
        m_cmdMap.insert("GetBrickOverlapSize", VclType::GetBrickOverlapSize);
        m_cmdMap.insert("GetLargestSingleBrickLOD", VclType::GetLargestSingleBrickLOD);
        m_cmdMap.insert("GetBrickVoxelCounts", VclType::GetBrickVoxelCounts);
        m_cmdMap.insert("GetBrickExtents", VclType::GetBrickExtents);
        m_cmdMap.insert("GetBrickLayout", VclType::GetBrickLayout);
        m_cmdMap.insert("GetModalityCount", VclType::GetModalityCount);
        m_cmdMap.insert("GetComponentCount", VclType::GetComponentCount);
        m_cmdMap.insert("GetRange", VclType::GetRange);
        m_cmdMap.insert("GetTotalBrickCount", VclType::GetTotalBrickCount);
        m_cmdMap.insert("GetBrick", VclType::GetBrick);
        m_cmdMap.insert("GetType", VclType::GetType);
        m_cmdMap.insert("GetSemantic", VclType::GetSemantic);

#undef PYTHON_MAGIC_STRING

        m_errorCodeMap.insert(std::make_pair(1, "no such command"));
        m_errorCodeMap.insert(std::make_pair(2, "no such renderer type"));

        assertCompleteLanguage();
    }

    mocca::BidirectionalMap<std::string, VclType> m_cmdMap;
    std::map<int, std::string> m_errorCodeMap;
    void assertCompleteLanguage() const;
};


// the trinity protocol uses request- and session id's (rid and sid)
class IDGenerator {
public:
    static uint32_t nextID();
};
}
