#include "client.h"
#include "ui_client.h"

#include <iostream>

#include <thread>

using namespace Core::Math;

client::client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::client)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));

    timer->start(10);
}

client::~client()
{
    delete ui;
}

void client::on_clearview_clicked()
{
    IRenderManager& manager = RenderManager::getInstance();
    renderer = manager.createRenderer("localhost",1234,Visibility::Windowed,Vec2ui(640,480),"WholeBody-SCANLINE-68-lz4.uvf","WholeBody.1dt");

    //TCPNetworkService netService; // instantiate the network service
    //clientConnection = netService.connect("localhost:1234");
}

void client::on_slicing_clicked()
{

    renderer->RotateCamera(Vec3f(0,1,0));
   //BytePacket msg;
    //msg << "INIT";
    //clientConnection->send(msg.byteArray());

    //connect(timer,SIGNAL(timeout()), this, SLOT(on_rotation_clicked()));
}

void client::on_rotation_clicked()
{
   connect(timer, SIGNAL(timeout()), this, SLOT(requestInUpdate()));
   connect(timer, SIGNAL(timeout()), this, SLOT(on_slicing_clicked()));

   //BytePacket data = clientConnection->receive(ReceiveMode::NON_BLOCKING);
   //if(data.byteArray().size() > 0){
   //     ui->openGLWidget->paintFrameBuffer((void*) data.byteArray().data());
  //}
}

void client::requestInUpdate(){
    std::vector<uint8_t> pix = renderer->ReadFrameBuffer()._data;
    ui->openGLWidget->paintFrameBuffer((void*) &pix[0]);
}
