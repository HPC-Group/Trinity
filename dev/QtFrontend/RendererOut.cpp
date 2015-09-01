#include "RendererOut.h"


RendererOut::RendererOut(int w, int h) :
	m_renderer(NULL),
	m_frameSize(w, h)
{
}

RendererOut::~RendererOut()
{
	glDeleteVertexArrays(1, &m_frameVAO);
	glDeleteTextures(1, &m_frameTexture2D);
}

void RendererOut::initializeGL()
{
	initializeOpenGLFunctions();
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	m_shaderProgram = new QOpenGLShaderProgram(this);

	m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, readShaderFile("textureFrameVS.glsl").c_str());
	m_shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, readShaderFile("textureFrameFS.glsl").c_str());

	m_shaderProgram->link();

	m_shaderProgram->uniformLocation("frameTexture");

	createTexture2D();
	createFrameRenderTarget();
}

void RendererOut::resizeGL(int w, int h)
{
	m_frameSize.setX(w);
	m_frameSize.setY(h);

	glDeleteTextures(1, &m_frameTexture2D);
	createTexture2D();

	// Update projection matrix and other size related settings:
	//m_projection.setToIdentity();
	//m_projection.perspective(45.0f, w / float(h), 0.01f, 100.0f);
}

void RendererOut::paintGL()
{
	if (m_renderer != NULL)
	{
		FrameData frame = m_renderer->ReadFrameBuffer();

		if (frame._data.size() > 0)
		{
			glBindTexture(GL_TEXTURE_2D, m_frameTexture2D);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_frameSize.x(), m_frameSize.y(), GL_RGB, GL_UNSIGNED_BYTE, &frame._data[0]);

			glDisable(GL_DEPTH_TEST);

			//glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_frameSize.x(), m_frameSize.y());

			m_shaderProgram->bind();

			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			/*GLuint frameTexLocation = m_shaderProgram->uniformLocation("frameTexture");
			if (frameTexLocation >= 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, m_frameTexture2D);
				glUniform1i(frameTexLocation, 0);
			}*/

			glBindVertexArray(m_frameVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			glBindVertexArray(0);
			m_shaderProgram->release();
		}
	}
}

void RendererOut::initRenderServer(std::string dataset, std::string tf)
{
	m_renderer = RenderManager::getInstance().createRenderer(Visibility::hidden, Vec2ui(m_frameSize.x(), m_frameSize.y()), dataset, tf);
}

void RendererOut::createTexture2D()
{
	/*int size = (int)(m_frameSize.x() * m_frameSize.y()) * 3;

	std::cout << m_frameSize.x() << " " << m_frameSize.y() << std::endl;

	GLubyte *buffer = new GLubyte[size];

	for (int i = 0; i < size; i+= 3)
	{
		buffer[i] = 211;
		buffer[i + 1] = 0;
		buffer[i + 2] = 0;
	}*/

	glGenTextures(1, &m_frameTexture2D);
	glBindTexture(GL_TEXTURE_2D, m_frameTexture2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, (int)m_frameSize.x(), (int)m_frameSize.y(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	if (glGetError() != GL_NO_ERROR)
		std::cout << "2D texture Error: " << std::endl;
}

void RendererOut::createFrameRenderTarget()
{
	static const GLfloat quad[] =
	{
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &m_frameVAO);
	glBindVertexArray(m_frameVAO);

	GLuint frameVBO;
	glGenBuffers(1, &frameVBO);
	glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);

	if (glGetError() != GL_NO_ERROR)
		std::cout << "target Error: " << std::endl;

	glBindVertexArray(0);
}

std::string RendererOut::readShaderFile(std::string str_filePath)
{
	const char *filePath = str_filePath.c_str();
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open())
	{
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof())
	{
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}