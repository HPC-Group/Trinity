#include "openglwidget.h"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#define PROGRAM_VERTEX_ATTRIBUTE 0
#define PROGRAM_TEXCOORD_ATTRIBUTE 1

OpenGLWidget::OpenGLWidget(QWidget* parent)
    : QOpenGLWidget(parent)
    , m_texture(nullptr)
    , m_program(nullptr) {}

void OpenGLWidget::setData(int width, int height, unsigned char* data) {
    if (!data) {
        return;
    }

    if (m_texture == 0 || width != m_texture->width() || height != m_texture->height()) {
        m_texture = new QOpenGLTexture(QOpenGLTexture::Target2D);
        m_texture->create();
        m_texture->setSize(width, height);
        m_texture->setFormat(QOpenGLTexture::RGBA8_UNorm);
        m_texture->setMipLevels(1);
        m_texture->allocateStorage();
        m_texture->setWrapMode(QOpenGLTexture::ClampToEdge);
        m_texture->setMinificationFilter(QOpenGLTexture::Linear);
        m_texture->setMagnificationFilter(QOpenGLTexture::Linear);
    }

    m_texture->setData(QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, data);
}

OpenGLWidget::~OpenGLWidget() {
    delete m_texture;
    delete m_program;
}

void OpenGLWidget::createShaderProgram() {
    QOpenGLShader* vshader = new QOpenGLShader(QOpenGLShader::Vertex, this);
    const char* vsrc = "attribute highp vec4 vertex;\n"
                       "attribute highp vec2 texCoord;\n"
                       "varying highp vec2 texc;\n"
                       "void main(void)\n"
                       "{\n"
                       "    gl_Position = vertex;\n"
                       "    texc = texCoord;\n"
                       "}\n";
    vshader->compileSourceCode(vsrc);

    QOpenGLShader* fshader = new QOpenGLShader(QOpenGLShader::Fragment, this);
    const char* fsrc = "uniform sampler2D texture;\n"
                       "varying highp vec2 texc;\n"
                       "void main(void)\n"
                       "{\n"
                       "    vec3 texColor = texture2D(texture, texc).rgb;\n"
                       "    gl_FragColor = vec4(texColor, 1.0); \n"
                       "}\n";
    fshader->compileSourceCode(fsrc);

    m_program = new QOpenGLShaderProgram;
    m_program->addShader(vshader);
    m_program->addShader(fshader);
    m_program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
    m_program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->link();

    m_program->bind();
    m_program->setUniformValue("texture", 0);
    m_program->enableAttributeArray(PROGRAM_VERTEX_ATTRIBUTE);
    m_program->enableAttributeArray(PROGRAM_TEXCOORD_ATTRIBUTE);
    m_program->setAttributeBuffer(PROGRAM_VERTEX_ATTRIBUTE, GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
    m_program->setAttributeBuffer(PROGRAM_TEXCOORD_ATTRIBUTE, GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
}

void OpenGLWidget::createVBO() {
    static const int coords[4][3] = {{-1, -1, 0}, {-1, 3, 0}, {3, -1, 0}};
    static const int texCoord[4][2] = {
        {0, 0}, {0, 2}, {2, 0},
    };

    QVector<GLfloat> vertData;
    for (uint32_t i = 0; i < 3; ++i) {
        // vertex position
        vertData.append(coords[i][0]);
        vertData.append(coords[i][1]);
        vertData.append(coords[i][2]);

        // texture coordinate
        vertData.append(texCoord[i][0]);
        vertData.append(texCoord[i][1]);
    }

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(vertData.constData(), vertData.count() * sizeof(GLfloat));
}

void OpenGLWidget::initializeGL() {
    makeCurrent();
    initializeOpenGLFunctions();

    createVBO();
    createShaderProgram();
}

void OpenGLWidget::paintGL() {
    // devicePixelRatio is needed for retina displays
    glViewport(0, 0, this->width() * devicePixelRatio(), this->height() * devicePixelRatio());
    if (m_texture) {
        m_texture->bind(0);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    } else {
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    glFinish();
    glFlush();
}