#include "openglwidget.h"
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include <iostream>

#include "mocca/log/LogManager.h"

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

OpenGLWidget::OpenGLWidget(QWidget* parent) :
QOpenGLWidget(parent),
texture(nullptr),
program(nullptr)
{
}

void OpenGLWidget::setData(int width, int height, unsigned char* data) {
  if (!data) return;
  
  if (texture == 0 || width != texture->width() || height != texture->height()) {
    texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
    texture->create();
    texture->setSize(width, height);
    texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
    texture->setMipLevels(1);
    texture->allocateStorage();
    texture->setWrapMode(QOpenGLTexture::ClampToEdge);
    texture->setMinificationFilter(QOpenGLTexture::Linear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
  }
  
  texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data);
}

OpenGLWidget::~OpenGLWidget() {
  delete texture;
  delete program;
}

void OpenGLWidget::createShaderProgram() {
  QOpenGLShader *vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
  const char *vsrc =
  "attribute highp vec4 vertex;\n"
  "attribute highp vec2 texCoord;\n"
  "varying highp vec2 texc;\n"
  "void main(void)\n"
  "{\n"
  "    gl_Position = vertex;\n"
  "    texc = texCoord;\n"
  "}\n";
  vshader->compileSourceCode(vsrc);
  
  QOpenGLShader *fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
  const char *fsrc =
  "uniform sampler2D texture;\n"
  "varying highp vec2 texc;\n"
  "void main(void)\n"
  "{\n"
  "    vec3 texColor = texture2D(texture, texc).rgb;\n"
  "    gl_FragColor = vec4(texColor, 1.0); \n"
  "}\n";
  fshader->compileSourceCode(fsrc);
  
  program = new QOpenGLShaderProgram;
  program->addShader(vshader);
  program->addShader(fshader);
  program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
  program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
  program->link();
  
  program->bind();
  program->setUniformValue("texture", 0);
  program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
  program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
  program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT,
                              0, 3, 5 * sizeof(GLfloat));
  program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT,
                              3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
}

void OpenGLWidget::createVBO() {
  static const int coords[4][3] = {
    { -1, -1, 0}, { -1, 3, 0 }, { 3, -1, 0 }
  };
  static const int texCoord[4][2] = {
    { 0, 0}, { 0, 2 }, { 2,  0 },
  };
  
  QVector<GLfloat> vertData;
  for (uint32_t i = 0;i<3;++i) {
    // vertex position
    vertData.append(coords[i][0]);
    vertData.append(coords[i][1]);
    vertData.append(coords[i][2]);
    
    // texture coordinate
    vertData.append(texCoord[i][0]);
    vertData.append(texCoord[i][1]);
  }
  
  vbo.create();
  vbo.bind();
  vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}

void OpenGLWidget::initializeGL() {
  makeCurrent();
  initializeOpenGLFunctions();
  
  createVBO();
  createShaderProgram();
}

static bool toggle = true;

void OpenGLWidget::paintGL() {
  // devicePixelRatio is needed for retina displays
  glViewport(0, 0, this->width()*devicePixelRatio(),
                   this->height()*devicePixelRatio());
  
  
  if (texture) {
    texture->bind(0);
    glDrawArrays(GL_TRIANGLES, 0, 3);
  } else {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT);
  }
  
  // make sure OpenGL has finished rendering before we return control back
  // to Qt
  glFinish();
}
