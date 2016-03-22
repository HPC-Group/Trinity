#pragma once

#include "opengl-base/OpenGLincludes.h"
#include "silverbullet/math/Vectors.h"

  /*! \brief This class is simply used to render the volume-cube/ boundingbox and nearplane
   *
   */
class GLModel{
public:
    GLModel();
    virtual ~GLModel();

    virtual void Initialize(float vertices[], int indices[], int m_vertexCount, int m_indexCount);

    void paint(GLenum mode = GL_TRIANGLES);



	uint64_t GetGPUSize() const;

protected:
	void updateVertexData(float vertices[], int m_vertexCount);

private:
    bool  m_bInitialized;

    GLuint m_VertexArrayObjectHandle;
    GLuint m_VertexBufferObjectHandle;
    GLuint m_VertexIndexObjectHandle;

    GLuint m_ElementCount;


	uint64_t m_gpuSize;

    Core::Math::Vec3f m_position;
};

class GLSphere: public GLModel{
public:
  GLSphere();
  virtual ~GLSphere();

private:
};
