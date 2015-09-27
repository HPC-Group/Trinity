#include "CommandReciever.h"

#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

#include <renderer/NetworkRenderer/NetCommands.h>

#include <core/splittools.h>
#include <core/Math/Vectors.h>

#include <io/JPEGTool.h>

using namespace ghoul::logging;
using namespace Core::Math;

CommandReciever::CommandReciever(uint32_t port, std::shared_ptr<IRenderer> renderer):
_listener(nullptr),
_connection(nullptr),
_renderer(renderer),
_keepAlive(true),
_port(port),
_thread(nullptr)
{
    TCPNetworkService netService;
    _listener  = netService.bind(std::to_string(port)); // listen for connections
	
}


CommandReciever::~CommandReciever(){

}

void CommandReciever::setRenderer(std::shared_ptr<IRenderer> renderer){
    _renderer = renderer;
}


void CommandReciever::run(){
    //wait for client to connect
    while(_connection == nullptr && _keepAlive){
        _connection = _listener->getConnection();
        std::cout <<" waiting" << std::endl;
    }
	_timer.start();
    while(_keepAlive){
        try{
            BytePacket data = _connection->receive();
            if(data.byteArray().size() > 0){
                std::string s(data.byteArray().data(),0,data.byteArray().size());
                LDEBUGC("Renderprocess", s);

                handleMsg(s);
            }

        }catch(const ConnectionClosedError& err){
            LERRORC("Renderprocess", "lost connection");
            _keepAlive = false;
            _renderer->stopRenderer();
        }catch(const NetworkError& err){
            LERRORC("Renderprocess", "network error");
            _keepAlive = false;
            _renderer->stopRenderer();
        }
    }
}

unsigned char* compressedImage = new unsigned char[640 * 480 * 3];
uint32_t quality = 20;
void CommandReciever::handleMsg(std::string& msg){
    int32_t ticketID = 0;

    //split the msg into a vector!
    std::vector<std::string> args = splitString(msg,':');

    if(args.size() <= 0) return;

    if(_renderer != nullptr){
        if(args.size() >= 3)
            ticketID = std::atoi(args[1].c_str());

        //requested framebuffer?
        if(args[0] == Commands::FRAME && args.size() == 4){ //request frame needs 1 parameter + 2 overhead

            uint64_t clientFrame = std::atoi(args[3].c_str());

            FrameData frame  = _renderer->ReadFrameBuffer();

            //send the frameID;
            ByteArray framePacket;
            framePacket.append(&(frame._frameID),sizeof(frame._frameID));
            _connection->send(framePacket);
			if (clientFrame < frame._frameID) quality = 20; //start with lowest quality

            //does the client have an older frame?
            if(clientFrame < frame._frameID || quality <= 100){
				ByteArray qualityValue;
				qualityValue.append(&(quality), sizeof(quality));
				_connection->send(qualityValue);
				
				//compress frame to target quality
				unsigned long l = compressImage((char*)&(frame._data[0]), 640, 480, compressedImage, quality);

                ByteArray p;
				p.append(&(compressedImage[0]), l);
                _connection->send(p);

				quality += 10;
			}

        //rotate renderer
        }else if(args[0] == Commands::ROT && args.size() == 6){
            RotateCamera(Vec3f(   std::stof(args[3].c_str()) ,
                                  std::stof(args[4].c_str()) ,
                                  std::stof(args[5].c_str()) ));
        }else if(args[0] == Commands::MOV && args.size() == 6){
            MoveCamera(Vec3f(   std::stof(args[3].c_str()) ,
                                  std::stof(args[4].c_str()) ,
                                  std::stof(args[5].c_str()) ));
        }else if(args[0] == Commands::ZOM && args.size() == 4){
            ZoomCamera(std::stof(args[3].c_str()));
        }
    }
}


void CommandReciever::endCommandReciever(){
    _keepAlive= false;
}

void CommandReciever::startThread(){
    _thread = std::unique_ptr<std::thread>(new std::thread(&CommandReciever::run,this));
}

void CommandReciever::joinThread(){
	_thread->join();
}

//------------- NetworkCommands-------------------------------------------------
void CommandReciever::SetFirstPersonMode(bool mode){
    _renderer->SetFirstPersonMode(mode);
}
void CommandReciever::RotateCamera(Core::Math::Vec3f rotation){
    _renderer->RotateCamera(rotation);
}
void CommandReciever::MoveCamera(Core::Math::Vec3f direction){
    _renderer->MoveCamera(direction);
}
void CommandReciever::SetCameraZoom(float f){
    _renderer->SetCameraZoom(f);
}
void CommandReciever::ZoomCamera(float f){
std::cout << "ZOOM UM "<< f << std::endl;
    _renderer->ZoomCamera(f);
}

void CommandReciever::SetIsoValue(float fIsoValue){
    _renderer->SetIsoValue(fIsoValue);
}
void CommandReciever::SetIsoValueRelative(float fIsovalue){
    _renderer->SetIsoValueRelative(fIsovalue);
}
void CommandReciever::GetIsoValue(){
    float f = _renderer->GetIsoValue();

    ByteArray response;
    response.append(&(f),sizeof(f));
    _connection->send(response);
}
void CommandReciever::SetIsosurfaceColor(Core::Math::Vec3f vColor){
    _renderer->SetIsosurfaceColor(vColor);
}
void CommandReciever::GetIsosurfaceColor(){
    Vec3f v = _renderer->GetIsosurfaceColor();

    ByteArray response;
    response.append(&(v),sizeof(v));
    _connection->send(response);
}
void CommandReciever::SetColorDataset(bool isColor){
    _renderer->SetColorDataset(isColor);
}

void CommandReciever::SetViewParameters(float angle, float znear, float zfar) {
    _renderer->SetViewParameters(angle,znear,zfar);
}
void CommandReciever::GetProjectionMatrix(){
    Mat4f m = _renderer->GetProjectionMatrix();

    ByteArray response;
    response.append(&(m),sizeof(m));
    _connection->send(response);
}
void CommandReciever::GetViewMatrix(){
    Mat4f m = _renderer->GetViewMatrix();

    ByteArray response;
    response.append(&(m),sizeof(m));
    _connection->send(response);
}

void CommandReciever::SetBackgroundColors(Core::Math::Vec3f vTopColor,Core::Math::Vec3f vBottomColor){
    _renderer->SetBackgroundColors(vTopColor,vBottomColor);
}
void CommandReciever::GetBackgroundColor(const uint8_t index){
    Vec3f v = _renderer->GetBackgroundColor(index);

    ByteArray response;
    response.append(&(v),sizeof(v));
    _connection->send(response);
}

void CommandReciever::SetUseLighting(bool bUseLighting){
    _renderer->SetUseLighting(bUseLighting);
}
void CommandReciever::SetLightingParameters(Core::Math::Vec4f cAmbient,
			Core::Math::Vec4f cDiffuse,
			Core::Math::Vec4f cSpecular){
    _renderer->SetLightingParameters(cAmbient,cDiffuse,cSpecular);
}
void CommandReciever::GetAmbientColor(){
    Vec4f v = _renderer->GetAmbientColor();

    ByteArray response;
    response.append(&(v),sizeof(v));
    _connection->send(response);
}
void CommandReciever::GetDiffuseColor(){
    Vec4f v = _renderer->GetDiffuseColor();

    ByteArray response;
    response.append(&(v),sizeof(v));
    _connection->send(response);
}
void CommandReciever::GetSpecularColor(){
    Vec4f v = _renderer->GetSpecularColor();

    ByteArray response;
    response.append(&(v),sizeof(v));
    _connection->send(response);
}
void CommandReciever::SetSampleRateModifier(float fSampleRateModifier){
    _renderer->SetSampleRateModifier(fSampleRateModifier);
}
void CommandReciever::ResetCamera(){
    _renderer->ResetCamera();
}

void CommandReciever::GetSize(){
    Vec2ui v = _renderer->GetSize();

    ByteArray response;
    response.append(&(v),sizeof(v));
    _connection->send(response);
}
void CommandReciever::Resize(Core::Math::Vec2ui vWinSize){
    _renderer->Resize(vWinSize);
}

void CommandReciever::SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res){
    _renderer->SetViewPort(lower_left, upper_right,decrease_screen_res);
}
