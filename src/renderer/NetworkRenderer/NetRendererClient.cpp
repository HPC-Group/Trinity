#include "NetRendererClient.h"

using namespace Tuvok;
using namespace Tuvok::Renderer;

NetRendererClient::NetRendererClient(std::string ip,
                          int port,
                          Visibility visibility,
                          Core::Math::Vec2ui resolution,
                          std::string dataset,
                          std::string tf):
                          _currentFrame(){
connectToServer(ip,port);
initializeRenderer(visibility,resolution,dataset,tf);
}

NetRendererClient::~NetRendererClient(){

}

void NetRendererClient::connectToServer(std::string ip, int port){
    TCPNetworkService netService; // instantiate the network service
    std::string s = ip + ":"+ std::to_string(port);
    connection = netService.connect(s);
}


void NetRendererClient::initializeRenderer( Visibility visibility,
                                            Core::Math::Vec2ui resolution,
                                            std::string dataset,
                                            std::string tf){
    BytePacket msg;
    std::string m = "INIT:" + std::to_string(visibility) + ":" + std::to_string(resolution.x) +":" + std::to_string(resolution.y) + ":" + dataset + ":"+tf;
    msg << m;
    connection->send(msg.byteArray());

    _framebuffersize = resolution.x*resolution.y*3;
}

FrameData NetRendererClient::ReadFrameBuffer(){
    BytePacket msg;
    msg << "READFB:";
    connection->send(msg.byteArray());

    //wait for frameid
    BytePacket frameIDPacket = connection->receive();
    uint64_t frameID = frameIDPacket.get<uint64_t>();

    //if there is a new frame we wait till we get it!
    if(_currentFrame._frameID < frameID){
        //std::cout << "Waiting for new frame with id "<< frameID << std::endl;

        BytePacket data = connection->receive();

        _currentFrame._data.resize(_framebuffersize);
        int uncompressedSize =  LZ4_decompress_safe ((char*)data.byteArray().data(), (char*)&(_currentFrame._data)[0], data.byteArray().size(), _framebuffersize);

        _currentFrame._frameID = frameID;
    }
    return _currentFrame;
}

void NetRendererClient::RotateCamera(Core::Math::Vec3f rotation){
    std::string msgString = "ROTATE:"+std::to_string(rotation.x)+":"+std::to_string(rotation.y)+":"+std::to_string(rotation.z);
    BytePacket msg;
    msg << msgString;
    connection->send(msg.byteArray());
}
