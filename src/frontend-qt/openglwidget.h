#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLBuffer>

#include <vector>
#include <cstdint>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();

    void setData(int width,int height, unsigned char* data);

private:
  QOpenGLTexture *texture;
  QOpenGLShaderProgram *program;
  QOpenGLBuffer vbo;

};

#endif // OPENGLWIDGET_H
