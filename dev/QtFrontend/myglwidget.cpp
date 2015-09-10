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
    _moveScale(1)
{
setFocusPolicy(Qt::StrongFocus);
setMouseTracking(true);
}

myGLWidget::~myGLWidget()
{
}

void myGLWidget::cleanup(){

}

void myGLWidget::setRenderer(std::shared_ptr<Tuvok::Renderer::IRenderer> renderer)
{
	_renderer = renderer;
}

void myGLWidget::setSize(int w, int h)
{
	_windowSize.setX(w);
	_windowSize.setY(h);
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

	if (_renderer != NULL){
		
		Tuvok::Renderer::FrameData data = _renderer->ReadFrameBuffer();

		if (data._data.size() > 0)
			glDrawPixels(_windowSize.x(), _windowSize.y(), GL_RGB, GL_UNSIGNED_BYTE, &data._data[0]); // DISPLAY FRAMEBUFER!
      }
}

void myGLWidget::mouseMoveEvent(QMouseEvent * event)
{
	if (_lastPosition.x != -1 && _lastPosition.y != -1 && event->buttons() & Qt::LeftButton){
		Vec2d pix;
		pix.x = event->x();
		pix.y = event->y();

		Vec2d dir = _lastPosition - pix;
		_lastPosition = pix;

		emit Rotate(Vec3f(dir.y, dir.x, 0));
	}
	
	if (_lastPosition.x != -1 && _lastPosition.y != -1 && event->buttons() & Qt::RightButton) {
		Vec2d pix;
		pix.x = event->x();
		pix.y = event->y();

		Vec2d dir = _lastPosition - pix;
		_lastPosition = pix;

		emit Move(Vec3f(-dir.x / (float)_windowSize.x(), -dir.y / (float)_windowSize.y(), 0.0f));
	}
	
	_lastPosition.x = event->x();
	_lastPosition.y = event->y();
}
