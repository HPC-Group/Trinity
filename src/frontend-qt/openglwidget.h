#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <vector>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();

private:
    std::vector<uint8_t> _imgdata;
    uint32_t _width;
    uint32_t _height;
};

#endif // OPENGLWIDGET_H
