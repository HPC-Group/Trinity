#include "NetRendererClient.h"


#include "logging/logmanager.h"
#include "logging/consolelog.h"
#include "logging/htmllog.h"
#include "logging/textlog.h"

#include <net/Error.h>
#include <algorithm>

#include <IO/JPEGTool.h>

#define tostr std::to_string

using namespace Tuvok;
using namespace Tuvok::Renderer;
using namespace ghoul::logging;
using namespace std;
/*
    BIG THING TO CHANGE ! ADD THE CORRECT PROTOKOL!! CAN'T FIND IT RIGHT NOW!
*/

//init uses ip/port to register to a server
NetRendererClient::NetRendererClient(std::string ip,
                          int port,
                          Visibility visibility,
                          Core::Math::Vec2ui resolution,
                          std::string dataset,
                          std::string tf):
                          _currentFrame(),
                          _iTicketID(0),
                          _iCallID(0),
                          _framebuffersize(resolution.x*resolution.y*3),
                          _contextMutex(),
						  _ip(ip)
{
//connect to the server
connectToServer(ip,port);

//open ticket
openTicket(visibility,resolution,dataset,tf);

//auto init the renderer !
//initializeRenderer(visibility,resolution,dataset,tf);
}

NetRendererClient::~NetRendererClient(){

}

void NetRendererClient::connectToServer(std::string ip, int port)
{
    TCPNetworkService netService; // instantiate the network service
    std::string s = ip + ":"+ std::to_string(port);
    std::cout << "WILL CONNECT TO : "<<s <<std::endl;
    connection = netService.connect(s);

    //add some getRendererID or so, will be used later on
}

void NetRendererClient::openTicket(Visibility visibility,
                                Core::Math::Vec2ui resolution,
                                std::string dataset,
                                std::string tf){
    //std::string s = Commands::OPENTICKET+":"+str(_iCallID);

    //OPENTICKET:FILENAME:TRANSFERFUNKTION:AUFLÖSUNGX:AUFLÖSUNGY ---return---> port
    std::string s = Commands::OPENTICKET+":"+dataset+":"+tf+":"+std::to_string(resolution.x)+":"+std::to_string(resolution.y);

    sendString(s);

    //wait for ticketId;
    BytePacket frameIDPacket = connection->receive();
    int32_t rendererPort = frameIDPacket.get<int32_t>();

    std::cout << rendererPort << std::endl;



	std::string adr = _ip +":"+ std::to_string(rendererPort);

    std::cout << adr << std::endl;

    std::this_thread::sleep_for(std::chrono::milliseconds(1000)); // wait 1 seconds, renderer could take some time

    TCPNetworkService netService;
    connection = netService.connect(adr);

    //_iTicketID = frameIDPacket.get<int32_t>();
}

void NetRendererClient::sendString(std::string msg)
{
    std::replace( msg.begin(), msg.end(), ',', '.');
    ByteArray stringArray;
    stringArray.append(msg.c_str(),msg.size());

    connection->send(stringArray);
    ++_iCallID;
}


void NetRendererClient::initializeRenderer( Visibility visibility,
                                            Core::Math::Vec2ui resolution,
                                            std::string dataset,
                                            std::string tf)
{
    //init string : INIT:VISIBILITYSTATE:RESOLUTIONX:RESOLUTIONY:DATASET:TRANSFERFUNCTION
	std::string m = Commands::INIT + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(visibility) + ":" + tostr(resolution.x) + ":" + tostr(resolution.y) + ":" + dataset + ":" + tf;
    std::cout << m << std::endl;
    sendString(m);
}

unsigned char* inputFrame = new unsigned char[640*480*3];
uint32_t qualityRecv = 0;
FrameData& NetRendererClient::ReadFrameBuffer()
{
    _contextMutex.lock();
    //String : READFB:ID:LATESTFRAME;
	sendString(Commands::FRAME + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(_currentFrame._frameID));

    //wait for frameid
	uint64_t frameID = _currentFrame._frameID;
	try{
      BytePacket frameIDPacket = connection->receive();
      if (frameIDPacket.byteArray().size() == 8){
        frameID = frameIDPacket.get<uint64_t>();
      }

      if (_currentFrame._frameID < frameID) qualityRecv = 0; //new frame we don't know the coming quality

      //if there is a new frame we wait till we get it!
      if (_currentFrame._frameID < frameID || qualityRecv < 100){
        //wait for quality

        BytePacket QualityPacket = connection->receive(std::chrono::milliseconds(1000));
        if (QualityPacket.byteArray().size() == 4){
          qualityRecv = QualityPacket.get<int>();

          BytePacket data = connection->receive(std::chrono::milliseconds(1000));

          if (data.byteArray().size() > 1000){
            _currentFrame._data.resize(_framebuffersize);

            //decompress
            memcpy(inputFrame, data.byteArray().data(), data.byteArray().size());
            decompressImage(inputFrame, data.byteArray().size(), _currentFrame._data);

            _currentFrame._frameID = frameID;
          }
        }
    }

  }
  catch (const ConnectionClosedError& err){
    LERRORC("NetworkRendererClient", "lost connection");
  }
  catch (const NetworkError& err){
    LERRORC("NetworkRendererClient", "network error");
  }

    _contextMutex.unlock();
    return _currentFrame;
}

void NetRendererClient::RotateCamera(Core::Math::Vec3f rotation){
    _contextMutex.lock();
    //String : ROTATE:X:Y:Z
	std::string msg = Commands::ROT + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(rotation.x) + ":" + tostr(rotation.y) + ":" + tostr(rotation.z);
    sendString(msg);
    _contextMutex.unlock();
}

void NetRendererClient::MoveCamera(Core::Math::Vec3f direction){
    //String : MOVE:X:Y:Z
    _contextMutex.lock();
	std::string msg = Commands::MOV + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(direction.x) + ":" + tostr(direction.y) + ":" + tostr(direction.z);
    sendString(msg);
    _contextMutex.unlock();
}

void NetRendererClient::SetCameraZoom(float f){
    //String : ZOOM:X
    _contextMutex.lock();
	std::string msg = Commands::ZOM + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(f);
    sendString(msg);
    _contextMutex.unlock();
}

void NetRendererClient::ZoomCamera(float f){
    _contextMutex.lock();
	std::string msg = Commands::ZOM + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(f);
    sendString(msg);
    _contextMutex.unlock();
}

void NetRendererClient::SetIsoValue(float fIsoValue){
    _contextMutex.lock();
	sendString(Commands::SISO + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID) + ":" + tostr(fIsoValue));
    _contextMutex.unlock();
}

float NetRendererClient::GetIsoValue(){
	sendString(Commands::GISO + ":" + tostr(_iTicketID) + ":" + tostr(_iCallID));
    BytePacket data = connection->receive();
    return data.get<float>();
}
