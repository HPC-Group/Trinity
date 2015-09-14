#include "myglwidget.h"

#include <iostream>
#include <QString>
#include <string>
#include <cstdio>
#include <cstring>

#include <QRect>
#include <QPoint>
#include <QCursor>
#include <QMouseEvent>

myGLWidget::myGLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
    _lastPosition(-1,-1),
    _deltaPosition(0,0),
    _moveScale(0.9),
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

float clip(float n, float lower, float upper) {
  return std::max(lower, std::min(n, upper));
}

void myGLWidget::paintGL(){
      if(pixelptr != NULL){
        glDrawPixels(640,480, GL_RGB, GL_UNSIGNED_BYTE,pixelptr); // DISPLAY FRAMEBUFER!
      }

    QRect widgetRect = this->geometry();
    QPoint mousePos = this->mapFromGlobal(QCursor::pos());
    if(widgetRect.contains(mousePos))
    {
        if(_lastPosition.x != -1 && _lastPosition.y != -1 && (_leftClick || _rightClick)){

            _deltaPosition.x = (mousePos.x()-_lastPosition.x)*_moveScale;
            _deltaPosition.y = (mousePos.y()-_lastPosition.y)*_moveScale;


            if(_leftClick){
            std::cout << _deltaPosition << std::endl;
                _renderer->RotateCamera(Core::Math::Vec3f(-_deltaPosition.y,-_deltaPosition.x,0.0f));
            }
            if(_rightClick){
            _deltaPosition.x = _deltaPosition.x*0.01f;
            _deltaPosition.y = _deltaPosition.y*0.01f;
            std::cout << "MOVE: " <<_deltaPosition.x << " " << _deltaPosition.y << std::endl;
                _renderer->MoveCamera(Core::Math::Vec3f(-_deltaPosition.x,_deltaPosition.y,0.0f));
            }
        }

        _lastPosition.x = mousePos.x();
        _lastPosition.y = mousePos.y();

    }
}

void myGLWidget::paintFrameBuffer(void* pixels){
    memcpy(pixelptr, (char*)pixels, 640*480*3);
}

void myGLWidget::mousePressEvent(QMouseEvent  *event){
    if(event->buttons() == Qt::LeftButton)
    {
        std::cout << "left"<< std::endl;
        _leftClick = !_leftClick;
    }
    if(event->buttons() == Qt::RightButton)
    {
        std::cout << "right"<< std::endl;
        _rightClick = !_rightClick;
    }
}

void myGLWidget::mouseReleaseEvent(QMouseEvent  *event){
    if(event->buttons() == Qt::LeftButton)
    {
        std::cout << "left"<< std::endl;
        _leftClick = false;
    }
    if(event->buttons() == Qt::RightButton)
    {
        std::cout << "right"<< std::endl;
        _rightClick = false;
    }
}

void myGLWidget::wheelEvent(QWheelEvent* event){
    std::cout << (float)event->delta()/1000.0f << std::endl;
   _renderer->ZoomCamera((float)event->delta()/1000.0f);
}
