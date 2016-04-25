#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <cstdint>
#include <vector>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram);
QT_FORWARD_DECLARE_CLASS(QOpenGLTexture)

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();

    void setData(int width, int height, unsigned char* data);

private:
    void createVBO();
    void createShaderProgram();

private:
    QOpenGLTexture* m_texture;
    QOpenGLShaderProgram* m_program;
    QOpenGLBuffer m_vbo;
};

#endif // OPENGLWIDGET_H
