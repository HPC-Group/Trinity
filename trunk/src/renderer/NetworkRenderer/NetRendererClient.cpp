#include "NetRendererClient.h"

#define str std::to_string

using namespace Tuvok;
using namespace Tuvok::Renderer;

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
                          _framebuffersize(resolution.x*resolution.y*3)
{
//connect to the server
connectToServer(ip,port);

//open ticket
openTicket();

//auto init the renderer !
initializeRenderer(visibility,resolution,dataset,tf);
}

NetRendererClient::~NetRendererClient(){

}

void NetRendererClient::connectToServer(std::string ip, int port)
{
    TCPNetworkService netService; // instantiate the network service
    std::string s = ip + ":"+ std::to_string(port);
    connection = netService.connect(s);

    //add some getRendererID or so, will be used later on
}

void NetRendererClient::openTicket(){
    std::string s = Commands::OPENTICKET+":"+str(_iCallID);
    sendString(s);

    //wait for ticketId;
    BytePacket frameIDPacket = connection->receive();
    _iTicketID = frameIDPacket.get<int32_t>();
}

void NetRendererClient::sendString(std::string msg)
{
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
    std::string m = Commands::INIT+":"+str(_iTicketID)+":"+str(_iCallID)+":" + str(visibility) + ":" + str(resolution.x) +":" + str(resolution.y) + ":" + dataset + ":"+tf;
    std::cout << m << std::endl;
    sendString(m);
}

FrameData NetRendererClient::ReadFrameBuffer()
{
    //String : READFB:ID:LATESTFRAME;
    sendString(Commands::FRAME+":"+str(_iTicketID)+":"+str(_iCallID)+":"+str(_currentFrame._frameID));

    //wait for frameid
    BytePacket frameIDPacket = connection->receive(std::chrono::milliseconds(1000));
    uint64_t frameID = frameIDPacket.get<uint64_t>();

    //if there is a new frame we wait till we get it!
    if(_currentFrame._frameID < frameID){
        BytePacket data = connection->receive(std::chrono::milliseconds(1000));

        _currentFrame._data.resize(_framebuffersize);
        int uncompressedSize =  LZ4_decompress_safe ((char*)data.byteArray().data(), (char*)&(_currentFrame._data)[0], data.byteArray().size(), _framebuffersize);

        _currentFrame._frameID = frameID;
    }
    return _currentFrame;
}

void NetRendererClient::RotateCamera(Core::Math::Vec3f rotation){
    //String : ROTATE:X:Y:Z
    std::string msg = Commands::ROT+":"+str(_iTicketID)+":"+str(_iCallID)+":"+str(rotation.x)+":"+str(rotation.y)+":"+str(rotation.z);
    sendString(msg);
}

void NetRendererClient::MoveCamera(Core::Math::Vec3f direction){
    //String : MOVE:X:Y:Z
    std::string msg = Commands::MOV+":"+str(_iTicketID)+":"+str(_iCallID)+":"+str(direction.x)+":"+str(direction.y)+":"+str(direction.z);
    sendString(msg);
}

void NetRendererClient::SetCameraZoom(float f){
    //String : ZOOM:X
    std::string msg = Commands::ZOM+":"+str(_iTicketID)+":"+str(_iCallID)+":"+str(f);
    sendString(msg);
}

void NetRendererClient::SetIsoValue(float fIsoValue){
    sendString(Commands::SISO+":"+str(_iTicketID)+":"+str(_iCallID)+":"+str(fIsoValue));
}

float NetRendererClient::GetIsoValue(){
    sendString(Commands::GISO+":"+str(_iTicketID)+":"+str(_iCallID));
    BytePacket data = connection->receive();
    return data.get<float>();
}
