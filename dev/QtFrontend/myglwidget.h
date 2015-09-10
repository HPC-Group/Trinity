#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QMouseEvent>

#include <iostream>

#include <core/Math/Vectors.h>
#include <renderer/IRenderer.h>
#include <memory>

using namespace Core::Math;

class myGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT
public:
    myGLWidget(QWidget *parent = 0);
        ~myGLWidget();

    void cleanup();
    void initializeGL();
    void paintGL();

    void paintFrameBuffer(void* pixels);

	void setRenderer(std::shared_ptr<Tuvok::Renderer::IRenderer> renderer);
	void setSize(int w, int h);

private:
    std::shared_ptr<Tuvok::Renderer::IRenderer> _renderer;

    Core::Math::Vec2d  _lastPosition;
    float               _moveScale;

	QVector2D _windowSize;

signals:
	void Rotate(Core::Math::Vec3f);
	void Move(Core::Math::Vec3f);

public slots:
	void mouseMoveEvent(QMouseEvent * event);
};

#endif // MYGLWIDGET_H
