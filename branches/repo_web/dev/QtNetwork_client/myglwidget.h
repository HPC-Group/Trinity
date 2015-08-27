#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include <iostream>

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

private:
    char* pixelptr;
    float col;

signals:

public slots:
};

#endif // MYGLWIDGET_H
