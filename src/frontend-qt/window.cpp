#include "window.h"

#include "ui_window.h"
#include <QTimer>

#include <iostream>

#include "mocca/log/LogManager.h"

#include "commands/Vcl.h"
#include "common/TrinityError.h"

using namespace std;
using namespace mocca::net;

static int reconnectInSec = 5;

Window::Window(QWidget* parent)
: QMainWindow(parent)
, _initDone(false)
, ui(new Ui::Window) {
  ui->setupUi(this);
  LINFO("Window created");
  QTimer* timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(10);
}

Window::~Window() {
  delete ui;
}

/* OLD
 void Window::initRenderer() {
 trinity::StreamingParams params(_renderWidth, _renderHeight);
 
 Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");
 
 // the file id will be available after implementing the listdata command
 int fileId = 0;
 _renderer = _processingNode->initRenderer(trinity::VclType::SimpleRenderer, fileId, endpointIO, params);
 
 // sending commands
 _renderer->initContext();
 std::this_thread::sleep_for(std::chrono::seconds(1));
 _initDone = true;
 }
 
 void Window::on_IOconnectIP_clicked() {
 Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(), "localhost", "6678");
 
 _ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
 }
 
 void Window::on_PRconnectIP_clicked() {
 Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(), "localhost", "8678");
 
 _processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
 LINFO("connected to processing node");
 
 initRenderer();
 }
 
 static float rot = 0.0f;
 void Window::update() {
 if (_initDone) {
 _renderer->setIsoValue(rot);
 rot += 0.01f;
 
 auto frameNullable = _renderer->getVisStream()->get();
 if (!frameNullable.isNull()) {
 auto frame = frameNullable.release();
 ui->openGLWidget->setData(_renderWidth, _renderHeight, frame.data());
 ui->openGLWidget->repaint();
 }
 }
 }
 */


void Window::printDataTree(const std::string& dataId,
                           const std::string& indent) const {
  try {
    if (indent.empty()) {
      LINFO("Listing " + dataId);
    }
    
    std::vector<trinity::IOData> list = _ioNode->listFiles(dataId);
    for (auto e : list) {
      if (e.getDataType() == trinity::IOData::DataType::Directory) {
        LINFO(indent + "Directory:" + e.getName()+"\t ("+e.getFileId()+")");
        printDataTree(e.getFileId(), indent+"  ");
      } else {
        LINFO(indent+e.getName()+"\t ("+e.getFileId()+")");
      }
    }
  } catch (const trinity::TrinityError&) {
    
  }
}

void Window::initRenderer() {
  _renderWidth = (ui->resx->text().toInt());
  _renderHeight =(ui->resy->text().toInt());
  trinity::StreamingParams params(_renderWidth, _renderHeight);
  
  Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(),
                      ui->IOaddressIPedit->text().toStdString(),
                      ui->IOaddressPortedit->text().toStdString());
  
  // the file id will be available after implementing the listdata command
  std::string fileId = "FractalData@3a"; // fixme: should be selectable from the gui
  //std::string fileId = "UVFData@c60.uvf"; // fixme: should be selectable from the gui
  
  printDataTree("FractalData");
  printDataTree("UVFData");
  
  
  
  try {
    _renderer = _processingNode->initRenderer(trinity::VclType::SimpleRenderer,
                                              fileId, endpointIO, params);
   // _renderer->initContext();
  } catch (const trinity::TrinityError&) {
    LERROR("(qt) no connection to a renderer");
  }
  
  std::this_thread::sleep_for(std::chrono::seconds(1));
  _initDone = true;
}

void Window::on_IOconnectIP_clicked() {
  Endpoint endpointIO(ConnectionFactorySelector::tcpPrefixed(),
                      ui->IOaddressIPedit->text().toStdString(),
                      ui->IOaddressPortedit->text().toStdString());
  _ioNode = std::unique_ptr<trinity::IONodeProxy>(new trinity::IONodeProxy(endpointIO));
}

void Window::on_PRconnectIP_clicked() {
  Endpoint endpoint(ConnectionFactorySelector::tcpPrefixed(),
                    ui->PRaddressIPedit->text().toStdString(),
                    ui->PRaddressPortedit->text().toStdString());
  
  _processingNode = std::unique_ptr<trinity::ProcessingNodeProxy>(new trinity::ProcessingNodeProxy(endpoint));
  LINFO("connected to processing node");
  initRenderer();
}


void Window::update() {
  static float rot = 0.0f;
  
  if (_initDone && _renderer) {
    
    const bool somethingToDo = true;  // in this demo there is always something to do
    
    if (somethingToDo) {
      _renderer->setIsoValue(0, rot);
      rot += 0.01f;
    } else {
      _renderer->proceedRendering();
    }
    
    auto frameNullable = _renderer->getVisStream()->get();
    if (!frameNullable.isNull()) {
      auto frame = frameNullable.release();
      ui->openGLWidget->setData(_renderWidth, _renderHeight, frame.data());
      ui->openGLWidget->repaint();
      
    }
  }
}

void Window::repaint() {}
