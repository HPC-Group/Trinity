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
    : QOpenGLWidget(parent),
    _lastPosition(-1,-1),
    _deltaPosition(0,0),
    _moveScale(1),
    _leftClick(false),
    _rightClick(false)
{
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
        if(_lastPosition.x != -1 && _lastPosition.y != -1 && _leftClick){
            _deltaPosition.x = (mousePos.x()-_lastPosition.x)*_moveScale;
            _deltaPosition.y = (mousePos.y()-_lastPosition.y)*_moveScale;

            //std::cout << _deltaPosition << std::endl;
            _renderer->RotateCamera(Core::Math::Vec3f(-_deltaPosition.y,-_deltaPosition.x,0.0f));
        }

        _lastPosition.x = mousePos.x();
        _lastPosition.y = mousePos.y();

    }
}

void myGLWidget::paintFrameBuffer(void* pixels){
    memcpy(pixelptr, (char*)pixels, 640*480*3);
}

void myGLWidget::mousePressEvent(QMouseEvent  *event){
    std::cout << "pressed"<< std::endl;
    _leftClick = true;
}

void myGLWidget::mouseReleaseEvent(QMouseEvent  *event){
    std::cout << "released"<< std::endl;
    _leftClick = false;
}
