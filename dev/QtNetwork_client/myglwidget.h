#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <iostream>

#include <core/Math/Vectors.h>
#include <renderer/IRenderer.h>
#include <memory>

class myGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
public:
    myGLWidget(QWidget *parent = 0);
        ~myGLWidget();

    void cleanup();
    void initializeGL();
    void paintGL();

    void paintFrameBuffer(void* pixels);

    void mousePressEvent(QMouseEvent  *event);
    void mouseReleaseEvent(QMouseEvent  *event);

    void setRenderer(std::shared_ptr<Tuvok::Renderer::IRenderer> renderer){
        _renderer = renderer;
    }

private:
    std::shared_ptr<Tuvok::Renderer::IRenderer> _renderer;
    char* pixelptr;
    float col;

    Core::Math::Vec2i  _lastPosition;
    Core::Math::Vec2f   _deltaPosition;
    float               _moveScale;

    bool                _leftClick;
    bool                _rightClick;

signals:

public slots:
};

#endif // MYGLWIDGET_H
