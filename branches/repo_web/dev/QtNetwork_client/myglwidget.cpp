#include "myglwidget.h"

#include <iostream>
#include <QString>
#include <string>
#include <cstdio>
#include <cstring>

#include <QRect>
#include <QPoint>
#include <QCursor>

myGLWidget::myGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
std::cout << "here"<<std::endl;
std::cout << isValid() <<std::endl;

col = 0.0f;
pixelptr = new char[640*480*3];
}

myGLWidget::~myGLWidget()
{
}

void myGLWidget::cleanup(){

}


void myGLWidget::initializeGL()
{
    initializeOpenGLFunctions();
    std::cout << isValid() <<std::endl;


    const char* versionString = reinterpret_cast<const char*>(glGetString(GL_VERSION));
     std::string t(versionString, 0, 17);
     std::printf("version %s \n", t.c_str());
}

void myGLWidget::paintGL(){
      if(pixelptr != NULL){
        glDrawPixels(640,480, GL_RGB, GL_UNSIGNED_BYTE,pixelptr); // DISPLAY FRAMEBUFER!
      }

    QRect widgetRect = this->geometry();
    QPoint mousePos = this->mapFromGlobal(QCursor::pos());
    if(widgetRect.contains(mousePos))
    {
       // std::cout << "MOUSEPOS: "<< mousePos.x() << " " << mousePos.y() <<std::endl;
       // std::cout << "MOUSEPOS(2): "<< mousePos.rx()<< " " << mousePos.ry() <<std::endl;
       // std::cout << "Geometry: "<<  widgetRect.left() << " " << widgetRect.right() <<std::endl;
    }
}

void myGLWidget::paintFrameBuffer(void* pixels){
    memcpy(pixelptr, (char*)pixels, 640*480*3);
}

void myGLWidget::mousePressEvent(QMouseEvent  *event){

}
