#include <renderer/NetworkRenderer/NetRendererServer.h>
#include <net/Error.h>
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
_connectionCounterID(0),
_ticketCounter(0),
_disconnectedConnections(),
_endoflifeTickets()
{
}
NetRendererServer::~NetRendererServer()
{
}

void NetRendererServer::openServer(int port){
    TCPNetworkService netService; // instantiate the network service
    connectionListener = netService.bind(str(port)); // listen for connections

    _acceptionThread = std::make_shared<std::thread>(&NetRendererServer::acceptionThreadMth, this);
}

void NetRendererServer::acceptConnection(){
    // accept the next pending connection, returns nullptr if there is no pending connection
    std::unique_ptr<AbstractConnection> con = connectionListener->getConnection();
    if(con != nullptr){
        _connections.insert(std::pair<  uint32_t,
                                        std::unique_ptr<AbstractConnection>>
                                        (_connectionCounterID,std::move(con)));
        _connectionCounterID++;
    }
}

void NetRendererServer::acceptionThreadMth(){
    while(true){
        acceptConnection();
    }
}

void NetRendererServer::waitForMsg(){
    _disconnectedConnections.clear();
    for (auto connection = _connections.begin(); connection!=_connections.end(); ++connection){
        //only the msg thread will sleep and wait
        //renderer runs in seperate threads!
        try{
            BytePacket data = connection->second->receive();
            // dmc: I changed the interface of AbstractConnection::receive. I
            // removed the enum ReceiveMode and replaced it with a timeout. This
            // call was NON_BLOCKING before, corresponding to a timeout of 0.
            // However, a timeout of 0 leads to busy-waiting, i.e. unneccessarily
            // high CPU-load. Since action is only performed when data is received,
            // it should be ok to go with the default timeout (or a higher timeout,
            // in fact). If it isn't, setting the timeout to 0 should restore the
            // old behavior.
            if(data.byteArray().size() > 0){
                // for first we see incoming bytarray as simple strings where each byte
                // represents a single 8 bit char
                std::string s(data.byteArray().data(),0,data.byteArray().size());
                //give msg to our handle method
                handleMsg(s,connection->first);
            }
        }catch(ConnectionClosedError& err){
            //add connection id to be closed afterwards
            _disconnectedConnections.push_back(connection->first);
        }
    }
    cleanOldConnections();
    cleanOldTickets();
}

void NetRendererServer::cleanOldConnections(){
    for(uint32_t i : _disconnectedConnections){
        _connections.erase(i);
    }
}

void NetRendererServer::cleanOldTickets(){
    _endoflifeTickets.clear();
    for (auto ticket = _tickets.begin(); ticket!=_tickets.end(); ++ticket){
        if(ticket->second->endOfLife()){
            std::cout << "something is TTL" << std::endl;
            _endoflifeTickets.push_back(ticket->first);
        }
    }
    for(int32_t i : _endoflifeTickets){
        std::cout << "should clean : "<< i << std::endl;
        if(_tickets.find(i)->second->getRenderer() != nullptr)
            _tickets.erase(i);
    }
}

void NetRendererServer::handleMsg(std::string msg, uint32_t connectionID){

    //split the msg into a vector!
    std::vector<std::string> args = split(msg,':');

    if(args.size() <= 0){
        //something wrong!
        return;
    }

    int32_t ticketID = 0;
    RenderPtr renderer = nullptr;
    if(args.size() >= 3)
        ticketID = std::atoi(args[1].c_str());

    if(ticketID != 0){
        renderer = getRenderer(ticketID);
        updateTTL(ticketID);
    }

    //requested framebuffer?
    if(args[0] == Commands::FRAME && args.size() == 4){ //request frame needs 1 parameter + 2 overhead
        if(renderer == nullptr) return;

        FrameData frame  = renderer->ReadFrameBuffer();

        //send the frameID;
        ByteArray framePacket;
        framePacket.append(&(frame._frameID),sizeof(frame._frameID));
        _connections.find(connectionID)->second->send(framePacket);

        uint64_t clientFrame = std::atoi(args[3].c_str());

        //does the client have an older frame?
        if(clientFrame < frame._frameID){

            _compressedData.resize(640*480*3);
            int compressedSize = LZ4_compress(  (char*)&(frame._data[0]),
                                                (char*) &(_compressedData[0]),
                                                frame._data.size());
            // maybe move compress to renderer less work for serverthread(?)

            ByteArray p;
            p.append(&(_compressedData[0]),compressedSize);
            _connections.find(connectionID)->second->send(p);
        }
        return;

    //rotate renderer
    }else if(args[0] == Commands::ROT && args.size() == 6){
        if(renderer == nullptr) return;

        renderer->RotateCamera(Vec3f(   std::atof(args[3].c_str()) ,
                                        std::atof(args[4].c_str()) ,
                                        std::atof(args[5].c_str()) ));

    }else if(args[0] == Commands::MOV && args.size() == 6){
        if(renderer == nullptr) return;

        renderer->MoveCamera(Vec3f(     std::atof(args[3].c_str()) ,
                                        std::atof(args[4].c_str()) ,
                                        std::atof(args[5].c_str()) ));

    }else if(args[0] == Commands::INIT && args.size() == 8){
    std::cout << "should do some init" << std::endl;
        if(_tickets.find(ticketID) != _tickets.end()){

            //check for params
            Visibility v = (Visibility)std::atoi(args[3].c_str());
            Vec2ui resolution = Vec2ui( std::atoi(args[4].c_str()) ,
                                        std::atoi(args[5].c_str()) );
            std::string ds = args[6];
            std::string tf = args[7];

            //init renderer
            IRenderManager& manager = RenderManager::getInstance();
            renderer = manager.createRenderer(v,resolution,ds,tf);

            _tickets.find(ticketID)->second->setRenderer(renderer);
        }
        return;
    }else if(args[0] == Commands::OPENTICKET && args.size() == 2){
        ++_ticketCounter;

        auto ticket = std::unique_ptr<Ticket>(new Ticket(_ticketCounter));
        _tickets.insert(std::pair<int32_t,std::unique_ptr<Ticket>>(_ticketCounter,std::move(ticket)));

        ByteArray ticketID;
        ticketID.append(&(_ticketCounter),sizeof(_ticketCounter));
        _connections.find(connectionID)->second->send(ticketID);
    }
}

RenderPtr NetRendererServer::getRenderer(uint32_t ticketID) const {
    return _tickets.find(ticketID)->second->getRenderer();
}
void NetRendererServer::updateTTL(uint32_t ticketID){
    _tickets.find(ticketID)->second->updatePing();
}
