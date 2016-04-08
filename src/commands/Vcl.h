

#pragma once

#include <map>
#include <memory>

#include "mocca/base/BidirectionalMap.h"
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
    GridLeapingRenderer,
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
    GetDefault1DTransferFunctionCount,
    GetDefault2DTransferFunctionCount,
    Get1DHistogram,
    Get2DHistogram,
    GetUserDefinedSemantic,
    GetDefault1DTransferFunction,
    SetRenderMode,
    SupportsRenderMode,
    SetActiveModality,
    GetActiveModality,
    SetActiveTimestep,
    GetActiveTimestep,
    GetMaxBrickSize,
    GetMaxUsedBrickSizes,
    GetMaxMinForKey,
    GetNumberOfTimesteps,
    GetDomainSize,
    GetModalityCountProc,
    GetTimestepCountProc,
    Set1DTransferFunction,
    GetDefault1DTransferFunctionProc,
    GetDefault1DTransferFunctionCountProc,
    Get1DHistogramProc,
    GetIsoValue,
    SetIsosurfaceColor,
    GetIsosurfaceColor,
    GetRangeProc,
    SetClearViewPosition,
    SetClearViewRadius,
    SetClearBorderSize,
    EnableLighting,
    SetLightingColors,
    GetLightingColors,
    SetLightDirection,
    GetLightDirection,
    SetSampleRateModifier,
    GetSampleRateModifier,
    SetBoundingBoxMode,
    GetBoundingBoxMode,
    SetRendererSpecials,
    GetRendererSpecials,
    SetBackgroundColors,
    GetBackgroundColors,
    EnableClipping,
    SetClipVolume,
    SetViewParameters,
    RotateCamera,
    MoveCamera,
    RotateScene,
    MoveScene,
    RescaleScene,
    ResetCamera,
    ResetScene,
    StartRendering,
    StopRendering,
    IsIdle,
    ProceedRendering,
    GetDomainScale,
    GetFloatBrickLayout,
    SetUserViewMatrix,
    SetUserWorldMatrix,
    GetBrickMaxMin,
    GetBricks,
    GetRoots,
    /* AUTOGEN VclEnumEntry */
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
        m_cmdMap.insert("GridLeapingRenderer", VclType::GridLeapingRenderer);
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
        m_cmdMap.insert("GetMaxMinForKey", VclType::GetMaxMinForKey);
        m_cmdMap.insert("GetNumberOfTimesteps", VclType::GetNumberOfTimesteps);
        m_cmdMap.insert("GetDomainSize", VclType::GetDomainSize);
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
        m_cmdMap.insert("GetDefault1DTransferFunctionCount", VclType::GetDefault1DTransferFunctionCount);
        m_cmdMap.insert("GetDefault2DTransferFunctionCount", VclType::GetDefault2DTransferFunctionCount);
        m_cmdMap.insert("Get1DHistogram", VclType::Get1DHistogram);
        m_cmdMap.insert("Get2DHistogram", VclType::Get2DHistogram);
        m_cmdMap.insert("GetUserDefinedSemantic", VclType::GetUserDefinedSemantic);
        m_cmdMap.insert("GetDefault1DTransferFunction", VclType::GetDefault1DTransferFunction);
        m_cmdMap.insert("SetRenderMode", VclType::SetRenderMode);
        m_cmdMap.insert("SupportsRenderMode", VclType::SupportsRenderMode);
        m_cmdMap.insert("SetActiveModality", VclType::SetActiveModality);
        m_cmdMap.insert("GetActiveModality", VclType::GetActiveModality);
        m_cmdMap.insert("SetActiveTimestep", VclType::SetActiveTimestep);
        m_cmdMap.insert("GetActiveTimestep", VclType::GetActiveTimestep);
        m_cmdMap.insert("GetModalityCountProc", VclType::GetModalityCountProc);
        m_cmdMap.insert("GetTimestepCountProc", VclType::GetTimestepCountProc);
        m_cmdMap.insert("Set1DTransferFunction", VclType::Set1DTransferFunction);
        m_cmdMap.insert("GetDefault1DTransferFunctionProc", VclType::GetDefault1DTransferFunctionProc);
        m_cmdMap.insert("GetDefault1DTransferFunctionCountProc", VclType::GetDefault1DTransferFunctionCountProc);
        m_cmdMap.insert("Get1DHistogramProc", VclType::Get1DHistogramProc);
        m_cmdMap.insert("GetIsoValue", VclType::GetIsoValue);
        m_cmdMap.insert("SetIsosurfaceColor", VclType::SetIsosurfaceColor);
        m_cmdMap.insert("GetIsosurfaceColor", VclType::GetIsosurfaceColor);
        m_cmdMap.insert("GetRangeProc", VclType::GetRangeProc);
        m_cmdMap.insert("SetClearViewPosition", VclType::SetClearViewPosition);
        m_cmdMap.insert("SetClearViewRadius", VclType::SetClearViewRadius);
        m_cmdMap.insert("SetClearBorderSize", VclType::SetClearBorderSize);
        m_cmdMap.insert("EnableLighting", VclType::EnableLighting);
        m_cmdMap.insert("SetLightingColors", VclType::SetLightingColors);
        m_cmdMap.insert("GetLightingColors", VclType::GetLightingColors);
        m_cmdMap.insert("SetLightDirection", VclType::SetLightDirection);
        m_cmdMap.insert("GetLightDirection", VclType::GetLightDirection);
        m_cmdMap.insert("SetSampleRateModifier", VclType::SetSampleRateModifier);
        m_cmdMap.insert("GetSampleRateModifier", VclType::GetSampleRateModifier);
        m_cmdMap.insert("SetBoundingBoxMode", VclType::SetBoundingBoxMode);
        m_cmdMap.insert("GetBoundingBoxMode", VclType::GetBoundingBoxMode);
        m_cmdMap.insert("SetRendererSpecials", VclType::SetRendererSpecials);
        m_cmdMap.insert("GetRendererSpecials", VclType::GetRendererSpecials);
        m_cmdMap.insert("SetBackgroundColors", VclType::SetBackgroundColors);
        m_cmdMap.insert("GetBackgroundColors", VclType::GetBackgroundColors);
        m_cmdMap.insert("EnableClipping", VclType::EnableClipping);
        m_cmdMap.insert("SetClipVolume", VclType::SetClipVolume);
        m_cmdMap.insert("SetViewParameters", VclType::SetViewParameters);
        m_cmdMap.insert("RotateCamera", VclType::RotateCamera);
        m_cmdMap.insert("MoveCamera", VclType::MoveCamera);
        m_cmdMap.insert("RotateScene", VclType::RotateScene);
        m_cmdMap.insert("MoveScene", VclType::MoveScene);
        m_cmdMap.insert("RescaleScene", VclType::RescaleScene);
        m_cmdMap.insert("ResetCamera", VclType::ResetCamera);
        m_cmdMap.insert("ResetScene", VclType::ResetScene);
        m_cmdMap.insert("StartRendering", VclType::StartRendering);
        m_cmdMap.insert("StopRendering", VclType::StopRendering);
        m_cmdMap.insert("IsIdle", VclType::IsIdle);
        m_cmdMap.insert("ProceedRendering", VclType::ProceedRendering);
        m_cmdMap.insert("GetDomainScale", VclType::GetDomainScale);
        m_cmdMap.insert("GetFloatBrickLayout", VclType::GetFloatBrickLayout);
        m_cmdMap.insert("SetUserViewMatrix", VclType::SetUserViewMatrix);
        m_cmdMap.insert("SetUserWorldMatrix", VclType::SetUserWorldMatrix);
        m_cmdMap.insert("GetBrickMaxMin", VclType::GetBrickMaxMin);
        m_cmdMap.insert("GetBricks", VclType::GetBricks);
        m_cmdMap.insert("GetRoots", VclType::GetRoots);
        /* AUTOGEN VclMapEntry */

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
