#include <atomic>
#include <csignal>
#include <iostream>
#include <memory>
#include <thread>

#include "mocca/log/ConsoleLog.h"
#include "mocca/log/LogManager.h"
#include "mocca/net/ConnectionFactorySelector.h"
#include "mocca/net/Endpoint.h"

#include "commands/Vcl.h"

#include "frontend-base/ProcessingNodeProxy.h"
#include "common/IONodeProxy.h"
#include "common/ProxyUtils.h"
#include "commands/ProcessingCommands.h"
#include "commands/Request.h"
#include "mocca/log/HTMLLog.h"

using namespace trinity;
using namespace mocca::net;

void init() {
    using mocca::LogManager;
    LogManager::initialize(LogManager::LogLevel::Debug, true);
    auto web = new mocca::HTMLLog("log.html");
    LogMgr.addLog(web);
    ConnectionFactorySelector::addDefaultFactories();
}

std::unique_ptr<trinity::ProcessingNodeProxy> processingNode;
std::unique_ptr<trinity::IONodeProxy> ioNode;


std::atomic<bool> exitFlag{false};
void exitHandler(int s) {
    std::cout << "Trinity exit on signal " << std::to_string(s) << std::endl;
    exitFlag = true;
}

int main(int argc, char *argv[])
{
    init();
    
    
    // endpoints for processing and io nodes
    // hint: do not implement any cmd line parsers for that. They are already available
    Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), "localhost", "8678");
    Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");
    
    ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
    //LINFO("listing fractal dir with \"flat data\": ");
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("ListFilesCmd");
    auto datalist = ioNode->listFiles("FractalData@EMPTY");
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    for (const auto& data : datalist) {
        LINFO(data.toString());
    }
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("ListFilesCmd");
    datalist = ioNode->listFiles("FractalData@2b");
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    for (const auto& data : datalist) {
        //LINFO(data.toString());
    }
    
    processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
    
    // the file id will be available after implementing the listdata command
    std::string fileId = "FractalData@3a";
    trinity::StreamingParams params(1024, 768);
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("InitRendererCmd");
    auto renderer = processingNode->initRenderer(trinity::VclType::SimpleRenderer, fileId, endpointIO, params);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("StartRenderingCmd");
    renderer->startRendering();
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    // sending commands
   
    
    // wait for 2 seconds
    //std::this_thread::sleep_for(std::chrono::seconds(2));
    
    // receiving images
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("ZoomCameraCmd");
    ZoomCameraCmd::RequestParams requestParams0;
    ZoomCameraRequest request0(requestParams0, 0, 0);
    Request::createMessage(request0, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetIsoValueCmd");
    SetIsoValueCmd::RequestParams requestParams1;
    SetIsoValueRequest request1(requestParams1, 0, 0);
    Request::createMessage(request1, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetLODLevelCountCmd");
    GetLODLevelCountCmd::RequestParams requestParams2;
    GetLODLevelCountRequest request2(requestParams2, 0, 0);
    Request::createMessage(request2, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetRenderModeCmd");
    SetRenderModeCmd::RequestParams requestParams3;
    SetRenderModeRequest request3(requestParams3, 0, 0);
    Request::createMessage(request3, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");

    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetActiveModalityCmd");
    SetActiveModalityCmd::RequestParams requestParams4;
    SetActiveModalityRequest request4(requestParams4, 0, 0);
    Request::createMessage(request4, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetActiveModalityCmd");
    GetActiveModalityCmd::RequestParams requestParams5;
    GetActiveModalityRequest request5(requestParams5, 0, 0);
    Request::createMessage(request5, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetActiveTimestepCmd");
    SetActiveTimestepCmd::RequestParams requestParams6;
    SetActiveTimestepRequest request6(requestParams6, 0, 0);
    Request::createMessage(request6, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetActiveTimestepCmd");
    GetActiveTimestepCmd::RequestParams requestParams7;
    GetActiveTimestepRequest request7(requestParams7, 0, 0);
    Request::createMessage(request7, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetModalityCountProcCmd");
    GetModalityCountProcCmd::RequestParams requestParams9;
    GetModalityCountProcRequest request9(requestParams9, 0, 0);
    Request::createMessage(request9, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetTimestepCountProcCmd");
    GetTimestepCountProcCmd::RequestParams requestParams10;
    GetTimestepCountProcRequest request10(requestParams10, 0, 0);
    Request::createMessage(request10, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("Set1DTransferFunctionCmd");
    Set1DTransferFunctionCmd::RequestParams requestParams11;
    Set1DTransferFunctionRequest request11(requestParams11, 0, 0);
    Request::createMessage(request11, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetDefault1DTransferFunctionProcCmd");
    GetDefault1DTransferFunctionProcCmd::RequestParams requestParams12;
    GetDefault1DTransferFunctionProcRequest request12(requestParams12, 0, 0);
    Request::createMessage(request12, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetDefault1DTransferFunctionCountProcCmd");
    GetDefault1DTransferFunctionCountProcCmd::RequestParams requestParams13;
    GetDefault1DTransferFunctionCountProcRequest request13(requestParams13, 0, 0);
    Request::createMessage(request13, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("Get1DHistogramProcCmd");
    Get1DHistogramProcCmd::RequestParams requestParams14;
    Get1DHistogramProcRequest request14(requestParams14, 0, 0);
    Request::createMessage(request14, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetIsoValueCmd");
    GetIsoValueCmd::RequestParams requestParams15;
    GetIsoValueRequest request15(requestParams15, 0, 0);
    Request::createMessage(request15, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetIsosurfaceColorCmd");
    SetIsosurfaceColorCmd::RequestParams requestParams16;
    SetIsosurfaceColorRequest request16(requestParams16, 0, 0);
    Request::createMessage(request16, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetIsosurfaceColorCmd");
    GetIsosurfaceColorCmd::RequestParams requestParams17;
    GetIsosurfaceColorRequest request17(requestParams17, 0, 0);
    Request::createMessage(request17, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetRangeProcCmd");
    GetRangeProcCmd::RequestParams requestParams18;
    GetRangeProcRequest request18(requestParams18, 0, 0);
    Request::createMessage(request18, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetClearViewPositionCmd");
    SetClearViewPositionCmd::RequestParams requestParams19;
    SetClearViewPositionRequest request19(requestParams19, 0, 0);
    Request::createMessage(request19, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetClearViewRadiusCmd");
    SetClearViewRadiusCmd::RequestParams requestParams20;
    SetClearViewRadiusRequest request20(requestParams20, 0, 0);
    Request::createMessage(request20, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetClearBorderSizeCmd");
    SetClearBorderSizeCmd::RequestParams requestParams21;
    SetClearBorderSizeRequest request21(requestParams21, 0, 0);
    Request::createMessage(request21, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("EnableLightingCmd");
    EnableLightingCmd::RequestParams requestParams22;
    EnableLightingRequest request22(requestParams22, 0, 0);
    Request::createMessage(request22, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetLightingColorsCmd");
    SetLightingColorsCmd::RequestParams requestParams23;
    SetLightingColorsRequest request23(requestParams23, 0, 0);
    Request::createMessage(request23, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetLightingColorsCmd");
    GetLightingColorsCmd::RequestParams requestParams24;
    GetLightingColorsRequest request24(requestParams24, 0, 0);
    Request::createMessage(request24, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetLightDirectionCmd");
    SetLightDirectionCmd::RequestParams requestParams25;
    SetLightDirectionRequest request25(requestParams25, 0, 0);
    Request::createMessage(request25, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetLightDirectionCmd");
    GetLightDirectionCmd::RequestParams requestParams26;
    GetLightDirectionRequest request26(requestParams26, 0, 0);
    Request::createMessage(request26, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetSampleRateModifierCmd");
    SetSampleRateModifierCmd::RequestParams requestParams27;
    SetSampleRateModifierRequest request27(requestParams27, 0, 0);
    Request::createMessage(request27, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetSampleRateModifierCmd");
    GetSampleRateModifierCmd::RequestParams requestParams28;
    GetSampleRateModifierRequest request28(requestParams28, 0, 0);
    Request::createMessage(request28, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetBoundingBoxModeCmd");
    SetBoundingBoxModeCmd::RequestParams requestParams29;
    SetBoundingBoxModeRequest request29(requestParams29, 0, 0);
    Request::createMessage(request29, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetBoundingBoxModeCmd");
    GetBoundingBoxModeCmd::RequestParams requestParams30;
    GetBoundingBoxModeRequest request30(requestParams30, 0, 0);
    Request::createMessage(request30, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetRendererSpecialsCmd");
    SetRendererSpecialsCmd::RequestParams requestParams31;
    SetRendererSpecialsRequest request31(requestParams31, 0, 0);
    Request::createMessage(request31, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetRendererSpecialsCmd");
    GetRendererSpecialsCmd::RequestParams requestParams32;
    GetRendererSpecialsRequest request32(requestParams32, 0, 0);
    Request::createMessage(request32, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetBackgroundColorsCmd");
    SetBackgroundColorsCmd::RequestParams requestParams33;
    SetBackgroundColorsRequest request33(requestParams33, 0, 0);
    Request::createMessage(request33, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("GetBackgroundColorsCmd");
    GetBackgroundColorsCmd::RequestParams requestParams34;
    GetBackgroundColorsRequest request34(requestParams34, 0, 0);
    Request::createMessage(request34, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("EnableClippingCmd");
    EnableClippingCmd::RequestParams requestParams35;
    EnableClippingRequest request35(requestParams35, 0, 0);
    Request::createMessage(request35, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetClipVolumeCmd");
    SetClipVolumeCmd::RequestParams requestParams36;
    SetClipVolumeRequest request36(requestParams36, 0, 0);
    Request::createMessage(request36, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetViewParametersCmd");
    SetViewParametersCmd::RequestParams requestParams37;
    SetViewParametersRequest request37(requestParams37, 0, 0);
    Request::createMessage(request37, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("RotateCameraCmd");
    RotateCameraCmd::RequestParams requestParams38;
    RotateCameraRequest request38(requestParams38, 0, 0);
    Request::createMessage(request38, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("MoveCameraCmd");
    MoveCameraCmd::RequestParams requestParams39;
    MoveCameraRequest request39(requestParams39, 0, 0);
    Request::createMessage(request39, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("RotateSceneCmd");
    RotateSceneCmd::RequestParams requestParams40;
    RotateSceneRequest request40(requestParams40, 0, 0);
    Request::createMessage(request40, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("MoveSceneCmd");
    MoveSceneCmd::RequestParams requestParams41;
    MoveSceneRequest request41(requestParams41, 0, 0);
    Request::createMessage(request41, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("RescaleSceneCmd");
    RescaleSceneCmd::RequestParams requestParams42;
    RescaleSceneRequest request42(requestParams42, 0, 0);
    Request::createMessage(request42, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("ResetCameraCmd");
    ResetCameraCmd::RequestParams requestParams43;
    ResetCameraRequest request43(requestParams43, 0, 0);
    Request::createMessage(request43, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("ResetSceneCmd");
    ResetSceneCmd::RequestParams requestParams44;
    ResetSceneRequest request44(requestParams44, 0, 0);
    Request::createMessage(request44, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("StartRenderingCmd");
    StartRenderingCmd::RequestParams requestParams45;
    StartRenderingRequest request45(requestParams45, 0, 0);
    Request::createMessage(request45, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("StopRenderingCmd");
    StopRenderingCmd::RequestParams requestParams46;
    StopRenderingRequest request46(requestParams46, 0, 0);
    Request::createMessage(request46, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("IsIdleCmd");
    IsIdleCmd::RequestParams requestParams47;
    IsIdleRequest request47(requestParams47, 0, 0);
    Request::createMessage(request47, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("ProceedRenderingCmd");
    ProceedRenderingCmd::RequestParams requestParams48;
    ProceedRenderingRequest request48(requestParams48, 0, 0);
    Request::createMessage(request48, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetUserViewMatrixCmd");
    SetUserViewMatrixCmd::RequestParams requestParams49;
    SetUserViewMatrixRequest request49(requestParams49, 0, 0);
    Request::createMessage(request49, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    LINFO(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    LINFO("SetUserWorldMatrixCmd");
    SetUserWorldMatrixCmd::RequestParams requestParams50;
    SetUserWorldMatrixRequest request50(requestParams50, 0, 0);
    Request::createMessage(request50, CompressionMode::Uncompressed);
    LINFO("<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    
    
 
    
    
    
    
    /*
    
    while (!exitFlag) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        auto visStream = renderer->getVisStream();
        auto frame = visStream->get();
        if (frame.empty())
            LINFO("no frame arrived yet");
        else {
            LINFO("frame arrived!");
            //auto frame = frameNullable.release();
            //LINFO("content: " + frame.read(frame.size()));
        }
    };
     */
    
    
}
