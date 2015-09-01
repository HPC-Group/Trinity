#include <renderer/NetworkRenderer/NetRendererServer.h>

#define str std::to_string

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



NetRendererServer::NetRendererServer():
_lastFrameID(0)
{
}
NetRendererServer::~NetRendererServer()
{
}

void NetRendererServer::openServer(int port){
    TCPNetworkService netService; // instantiate the network service
    connectionListener = netService.bind(str(port)); // listen for connections
}

void NetRendererServer::acceptConnection(){
    while (serverConnection == nullptr) {
        // accept the next pending connection, returns nullptr if there is no pending connection
        serverConnection = connectionListener->getConnection();
    }
    std::cout << "new connection!" << std::endl;
}

void NetRendererServer::waitForMsg(){
    if(serverConnection != nullptr){
        //only the msg thread will sleep and wait
        //renderer runs in seperate threads!
        BytePacket data = serverConnection->receive();
        // dmc: I changed the interface of AbstractConnection::receive. I
        // removed the enum ReceiveMode and replaced it with a timeout. This
        // call was NON_BLOCKING before, corresponding to a timeout of 0.
        // However, a timeout of 0 leads to busy-waiting, i.e. unneccessarily
        // high CPU-load. Since action is only performed when data is received,
        // it should be ok to go with the default timeout (or a higher timeout,
        // in fact). If it isn't, setting the timeout to 0 should restore the
        // old behavior.
        if(data.byteArray().size() > 0){
            //std::string s(data.byteArray().data(),0,data.byteArray().size() );    // Android, until fixed the recieve
            std::string s = data.get<std::string>();                                // default mocca recieve
            //give msg to our handle method
            handleMsg(s);
        }
    }
}

void NetRendererServer::handleMsg(std::string msg){

    //split the msg into a vector!
    std::vector<std::string> args = split(msg,':');

    if(args.size() <= 0){
        //something wrong!
        return;
    }

    //requested framebuffer?
    if(args[0] == Commands::FRAME & args.size() == 3){ //request frame needs 1 parameter + 2 overhead
        if(renderer == nullptr) return;

        FrameData frame  = renderer->ReadFrameBuffer();
        //send the frameID;
        ByteArray framePacket;
        framePacket.append(&(frame._frameID),sizeof(frame._frameID));
        serverConnection->send(framePacket);

        //if last frame ov client is different then current highest frame
        //send the frame!
        if(_lastFrameID < frame._frameID){
            _lastFrameID = frame._frameID;

            _compressedData.resize(640*480*3);
            int compressedSize = LZ4_compress((char*)&(frame._data[0]),(char*) &(_compressedData[0]), frame._data.size()); // maybe move compress to renderer less work for serverthread(?)

            ByteArray p;
            p.append(&(_compressedData[0]),compressedSize);
            serverConnection->send(p);
        }
        return;

    //rotate renderer
    }else if(args[0] == Commands::ROT & args.size() == 5){
        if(renderer == nullptr) return;

        renderer->RotateCamera(Vec3f( std::atof(args[2].c_str()) , std::atof(args[3].c_str()) , std::atof(args[4].c_str()) ));

    }else if(args[0] == Commands::MOV & args.size() == 5){
        if(renderer == nullptr) return;

        renderer->MoveCamera(Vec3f( std::atof(args[2].c_str()) , std::atof(args[3].c_str()) , std::atof(args[4].c_str()) ));

    }else if(args[0] == Commands::INIT & args.size() == 6){

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
