#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <vector>
#include <cstdint>

class OpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    OpenGLWidget(QWidget* parent = 0);
    ~OpenGLWidget();

    void initializeGL();
    void paintGL();

    void setData(int width,int height, unsigned char* data);

private:
    std::vector<uint8_t> _imgdata;
    unsigned char*  _data;
    uint32_t _width;
    uint32_t _height;
};

#endif // OPENGLWIDGET_H
