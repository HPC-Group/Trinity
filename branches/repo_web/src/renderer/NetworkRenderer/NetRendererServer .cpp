#include <renderer/NetworkRenderer/NetRendererServer.h>

using namespace Core::Math;

#include <string>
#include <sstream>
#include <vector>

//as allways... http://stackoverflow.com/questions/236129/split-a-string-in-c
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}
std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}



NetRendererServer::NetRendererServer():connectionCounter(0),_lastFrameID(0){
}
NetRendererServer::~NetRendererServer(){}

void NetRendererServer::openServer(int port){
    TCPNetworkService netService; // instantiate the network service
    connectionListener = netService.bind(std::to_string(port)); // listen for connections
}

void NetRendererServer::acceptConnection(){
    if(connectionListener->numConnections() > 0){
           serverConnection = connectionListener->getConnection(); // accept the next pending connection
           std::cout << "new connection!"<< std::endl;
           connectionCounter = connectionListener->numConnections();
    }
}

void NetRendererServer::waitForMsg(){
    if(serverConnection != nullptr){
        BytePacket data = serverConnection->receive(ReceiveMode::NON_BLOCKING);
        if(data.byteArray().size() > 0){
            std::string message = data.get<std::string>();
            //std::cout << message << std::endl;
            handleMsg(message);
        }
    }
}

void NetRendererServer::handleMsg(std::string msg){
    std::vector<std::string> args = split(msg,':');

    if(args.size() <= 0){
        return;
    }

    //requested framebuffer?
    if(args[0] == "READFB"){
        if(renderer == nullptr) return;

        FrameData frame  = renderer->ReadFrameBuffer();
        //send the frameID;
        ByteArray framePacket;
        framePacket.append(&(frame._frameID),sizeof(frame._frameID));
        serverConnection->send(framePacket);


        if(_lastFrameID < frame._frameID){
            _lastFrameID = frame._frameID;

            _compressedData.resize(640*480*3);
            int compressedSize = LZ4_compress((char*)&(frame._data[0]),(char*) &(_compressedData[0]), frame._data.size());

            ByteArray p;
            //p.append(&(frame._data[0]),frame._data.size());
            p.append(&(_compressedData[0]),compressedSize);
            serverConnection->send(p);
        }
        return;

    //init renderer command?
    }else if(args[0] == "ROTATE" & args.size() == 4){
        renderer->RotateCamera(Vec3f( std::atof(args[1].c_str()) , std::atof(args[2].c_str()) , std::atof(args[3].c_str()) ));

    }else if(args[0] == "INIT"){
        if(args.size() != 6){
            std::cout << "not enough args to init"<<std::endl;
            return;
        }

        //check for params
        Visibility v = (Visibility)std::atoi(args[1].c_str());
        Vec2ui resolution = Vec2ui( std::atoi(args[2].c_str()) , std::atoi(args[3].c_str()) );
        std::string ds = args[4];
        std::string tf = args[5];

        //init renderer
        IRenderManager& manager = RenderManager::getInstance();
        renderer = manager.createRenderer(v,resolution,ds,tf);
        return;
    }
}
