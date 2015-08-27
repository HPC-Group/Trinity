#pragma once

#include <renderer/RenderManager.h>
#include <renderer/RenderEnums.h>

#include <core/Math/Vectors.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_2_Core>
#include <qvector2d.h>
#include <qmatrix4x4.h>
#include <qopenglshaderprogram.h>

#include <chrono>
#include <thread>
#include <iostream>
#include <fstream>

using namespace Tuvok::Renderer;
using namespace Tuvok;
using namespace Core::Math;

class RendererOut :
	public QOpenGLWidget, protected QOpenGLFunctions_3_2_Core
{
public:
	RendererOut(int w, int h);
	~RendererOut();

	void initRenderServer(std::string dataset, std::string tf);

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	std::string readShaderFile(std::string str_filePath);
	void createFrameRenderTarget();
	void createTexture2D();

	std::shared_ptr<IRenderer> m_renderer;

	QVector2D m_frameSize;

	QOpenGLShaderProgram *m_shaderProgram;
	GLuint m_frameTexture2D;
	GLuint m_frameVAO;
};

