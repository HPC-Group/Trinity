#include "openglwidget.h"

#include <iostream>

#include "mocca/log/LogManager.h"


OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , _width(255)
    , _height(255) {

    for (int x = 0; x < _width; ++x) {
        for (int y = 0; y < _height; ++y) {
            _imgdata.push_back(x);
            _imgdata.push_back(x);
            _imgdata.push_back(x);
            _imgdata.push_back(255);
        }
    }
}


OpenGLWidget::~OpenGLWidget() {}

void OpenGLWidget::initializeGL() {
    makeCurrent();
    initializeOpenGLFunctions();
    std::string versionString = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    LINFO("OpenGL Version : " << versionString);
}

void OpenGLWidget::paintGL() {
    glClearColor(0.0, 0.0, 0.0, 0.0);

    /*
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
glOrtho(0, this->width(), 0, this->height(), 0, 1);
glMatrixMode(GL_MODELVIEW);

glShadeModel(GL_FLAT);
glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

glViewport(0, 0, this->width(), this->height());
glPixelZoom(this->width() / (double) _width, this->height() / (double) _height);

glRasterPos2i(0,0);
glDrawPixels(_width,_height,GL_RGBA,GL_UNSIGNED_BYTE,&_imgdata[0]);
    */
}
