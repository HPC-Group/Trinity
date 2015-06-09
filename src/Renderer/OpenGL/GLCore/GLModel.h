//
//  GLModel.h
//  TNG
//
//  Created by Andre Waschk on 13/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLModel__
#define __TNG__GLModel__

#include <Renderer/OpenGL/OpenGLDefines.h>
#include <Core/Math/Vectors.h>

namespace Tuvok{
namespace Renderer{
namespace OpenGL{
namespace GLCore{

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

};
};
};
};

#endif /* defined(__TNG__GLModel__) */
