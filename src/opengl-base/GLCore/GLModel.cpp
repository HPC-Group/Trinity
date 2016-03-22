#include "GLModel.h"

GLModel::GLModel():
m_bInitialized(false){
}
GLModel::~GLModel(){}



void GLModel::Initialize(float vertices[], int indices[], int m_vertexCount, int m_indexCount){
  
  m_gpuSize = m_vertexCount * sizeof(float)* 3 + m_indexCount* sizeof(unsigned int);

  m_ElementCount = m_indexCount;

  glGenVertexArrays(1, &m_VertexArrayObjectHandle);
  glBindVertexArray(m_VertexArrayObjectHandle);

  
  glGenBuffers(1, &m_VertexBufferObjectHandle);
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectHandle);

  glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float)* 3, vertices, GL_STATIC_DRAW);

  // Enable vertex array attributes.
  glEnableVertexAttribArray(0);

  // Specify the location and format
  glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectHandle);
  glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(float)* 3, 0);

  
  glGenBuffers(1, &m_VertexIndexObjectHandle);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndexObjectHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexCount* sizeof(unsigned int), indices, GL_STATIC_DRAW);
  
  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    printf("++Unknown error (%x) occurred while creating an vbo.\n",
           static_cast<unsigned int>(err));
  }
  
  m_bInitialized = true;
}

void GLModel::updateVertexData(float vertices[], int m_vertexCount){
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferObjectHandle);
	glBufferData(GL_ARRAY_BUFFER, m_vertexCount * sizeof(float)* 3, vertices, GL_STATIC_DRAW);
}

void GLModel::paint(GLenum mode){
  if(m_bInitialized){
    glBindVertexArray(m_VertexArrayObjectHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexIndexObjectHandle);

    glDrawElements(mode, m_ElementCount, GL_UNSIGNED_INT, 0);
  }
}


uint64_t GLModel::GetGPUSize() const{
	return m_gpuSize;
}

static void generateSphere(float r, int nRings, int nSegments, std::vector<float>* vertexBuffer, std::vector<int>* indexbuffer){
    float fDeltaRingAngle = (3.14159265359f / nRings);
    float fDeltaSegAngle = (2 * 3.14159265359f / nSegments);
    unsigned short wVerticeIndex = 0 ;

    // Generate the group of rings for the sphere
         for( int ring = 0; ring <= nRings; ring++ ) {
             float r0 = r * sinf (ring * fDeltaRingAngle);
             float y0 = r * cosf (ring * fDeltaRingAngle);

             // Generate the group of segments for the current ring
             for(int seg = 0; seg <= nSegments; seg++) {
                 float x0 = r0 * sinf(seg * fDeltaSegAngle);
                 float z0 = r0 * cosf(seg * fDeltaSegAngle);

                 vertexBuffer->push_back(x0);
                 vertexBuffer->push_back(y0);
                 vertexBuffer->push_back(z0);

                 if (ring != nRings) {
                     indexbuffer->push_back(wVerticeIndex + nSegments + 1);
                     indexbuffer->push_back(wVerticeIndex);
                     indexbuffer->push_back(wVerticeIndex + nSegments);
                     indexbuffer->push_back(wVerticeIndex + nSegments + 1);
                     indexbuffer->push_back(wVerticeIndex + 1);
                     indexbuffer->push_back(wVerticeIndex);

                     wVerticeIndex ++;
                 }
             }; // end for seg
         } // end for ring
}


GLSphere::GLSphere(){
    std::vector<float> data;
    std::vector<int> indices;
    generateSphere(0.5f,20,20,&data,&indices);

    Initialize(&(data[0]),&(indices[0]), data.size()/3, indices.size());
}
GLSphere::~GLSphere(){}
