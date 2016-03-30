#include "GLProgram.h"
#include "GLTexture.h"

#include "opengl-base/ShaderDescriptor.h"
#include "silverbullet/io/FileTools.h"
#include "mocca/log/LogManager.h"

#include <opengl-base/OpenGLError.h>

#include <iostream>
#include <cstdio>
#include <cstdarg>

using namespace std;

GLProgram::GLProgram():
m_bInitialized(false)
{}

GLProgram::~GLProgram(){}

bool GLProgram::isEnabled() const {
  GLuint program;
  glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*) &program);
  return m_ShaderProgramm == program;
}

bool GLProgram::CompileShader(GLuint& handle, const char* source, GLenum type ){
  handle = glCreateShader(type);
  GL_CHECK(glShaderSource(handle, 1, &source, NULL));
  GL_CHECK(glCompileShader(handle));
  
  if (WriteInfoLog("shader\n", handle, false))
  {
    std::cout << type << std::endl;
    std::cout << "Could not compile shader" << std::endl;
    return false;
  }
  
  return true;
}

bool GLProgram::CheckProgramStatus(){
  int status;
  GL_CHECK(glGetProgramiv(m_ShaderProgramm, GL_LINK_STATUS, &status));
  if (status != 1)
  {
    std::cout << "Could not create programm " << std::endl;
    
    GLint maxLength = 0;
    GL_CHECK(glGetProgramiv(m_ShaderProgramm, GL_INFO_LOG_LENGTH, &maxLength));
    
    
    std::cout << maxLength<< std::endl;
    //The maxLength includes the NULL character
    std::vector<GLchar> infoLog(maxLength);
    GL_CHECK(glGetProgramInfoLog(m_ShaderProgramm, maxLength, &maxLength, &infoLog[0]));
    
    for(int i = 0; i < maxLength;++i){
      std::cout << infoLog[i];
    }
    //The program is useless now. So delete it.
    GL_CHECK(glDeleteProgram(m_ShaderProgramm));
    return false;
  }
  return true;
}


bool GLProgram::Load(ShaderDescriptor& sd){
  //create program
  m_ShaderProgramm = glCreateProgram();
  
  for (uint32_t i = 0; i < sd.GetVertexElementSize(); ++i){
    GLuint vsHandle = GL_INVALID_INDEX;
    if (!CompileShader(vsHandle, sd.GetVertexSource(i).c_str(), GL_VERTEX_SHADER)) return false;
    GL_CHECK(glAttachShader(m_ShaderProgramm, vsHandle));
  }
  for (uint32_t i = 0; i < sd.GetFragmentElementSize(); ++i){
    GLuint fsHandle = GL_INVALID_INDEX;
    if (!CompileShader(fsHandle, sd.GetFragmentSource(i).c_str(), GL_FRAGMENT_SHADER)) return false;
    GL_CHECK(glAttachShader(m_ShaderProgramm, fsHandle));
  }
  
  // Link the shader program
  GL_CHECK(glLinkProgram(m_ShaderProgramm));
  
  
  if(WriteInfoLog("programm\n", m_ShaderProgramm, true)) return false;
  
  GLenum err = glGetError();
  if(err == GL_OUT_OF_MEMORY) {
    //throw OUT_OF_MEMORY("allocating 3d texture");
  } else if(err != GL_NO_ERROR) {
    printf("Unknown error (%x) occurred while setting program.\n",
           static_cast<unsigned int>(err));
			 m_bInitialized = false;
    return false;
  }
  m_bInitialized = true;
  return true;
}

void GLProgram::Enable(){
  if(m_bInitialized){
    GL_CHECK(glUseProgram(m_ShaderProgramm));
  }
}
void GLProgram::Disable(){
  GL_CHECK(glUseProgram(0));
}

bool GLProgram::IsValid() const{
  return m_bInitialized;
}

void GLProgram::Set(const char *name, const float value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform1f(currentVariableLocation, value));
}

void GLProgram::Set(const char *name, const Core::Math::Vec2f value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform2f(currentVariableLocation, value.x, value.y));
}

void GLProgram::Set(const char *name, const Core::Math::Vec3f value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform3f(currentVariableLocation, value.x, value.y,value.z));
}

void GLProgram::Set(const char *name, const Core::Math::Vec4f value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform4f(currentVariableLocation, value.x, value.y,value.z,value.w));
}

void GLProgram::Set(const char *name, const int32_t value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform1i(currentVariableLocation, value));
}

void GLProgram::Set(const char *name, const Core::Math::Vec2i value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform2i(currentVariableLocation, value.x, value.y));
}

void GLProgram::Set(const char *name, const Core::Math::Vec3i value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform3i(currentVariableLocation, value.x, value.y,value.z));
}

void GLProgram::Set(const char *name, const Core::Math::Vec3ui value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform3ui(currentVariableLocation, value.x, value.y,value.z));
}

void GLProgram::Set(const char *name, const Core::Math::Vec3ui8 value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform3ui(currentVariableLocation, value.x, value.y,value.z));
}

void GLProgram::Set(const char *name, const Core::Math::Vec3ui64 value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform3ui(currentVariableLocation, value.x, value.y,value.z));
}

void GLProgram::Set(const char *name, const Core::Math::Vec4i value){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform4i(currentVariableLocation, value.x, value.y,value.z,value.w));
}

void GLProgram::Set(const char *name, const Core::Math::Mat4f& value,
                    bool bTranspose){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniformMatrix4fv(currentVariableLocation, 1, bTranspose, (float*) &value.array[0]));
}

void GLProgram::Set(const char *name, const std::vector<Core::Math::Mat4f>& value,
                    bool bTranspose){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniformMatrix4fv(currentVariableLocation, value.size(), bTranspose, (float*) &((value[0]).array[0])));
}

void GLProgram::SetTexture3D(const char *name, const GLuint value, const uint8_t id){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name);
  GL_CHECK(glUniform1i(currentVariableLocation, id));
  GL_CHECK(glActiveTexture(GL_TEXTURE0+id));
  GL_CHECK(glBindTexture(GL_TEXTURE_3D, value));
}

void GLProgram::SetTexture(const string& name,
                           const GLTexture& pTexture) {
  assert(isEnabled());
  if (m_mBindings.find(name) == m_mBindings.end ()) {
    // find a free texture unit
    int iUnusedTexUnit = 0;
    for (texMap::iterator i = m_mBindings.begin();i != m_mBindings.end();++i){
      if (i->second <= iUnusedTexUnit)
        iUnusedTexUnit = i->second+1;
    }
    ConnectTextureID(name, iUnusedTexUnit);
    pTexture.Bind(iUnusedTexUnit);
  } else {
    pTexture.Bind(m_mBindings[name]);
  }
}

void GLProgram::SetTexture(const std::string& name, const std::shared_ptr<GLTexture> pTexture) {
  assert(isEnabled());
  if (m_mBindings.find(name) == m_mBindings.end ()) {
    // find a free texture unit
    
    int iUnusedTexUnit = (int)m_mBindings.size();
    /*int iUnusedTexUnit = 0;
     for (texMap::iterator i = m_mBindings.begin();i != m_mBindings.end();++i){
     if (i->second <= iUnusedTexUnit)
     iUnusedTexUnit = i->second+1;
     }*/
    std::cout << "-------------" << name << "  " << iUnusedTexUnit << std::endl;
    ConnectTextureID(name, iUnusedTexUnit);
    std::cout << "new ptexture bind " << iUnusedTexUnit << std::endl;
    pTexture->Bind(iUnusedTexUnit);
  } else {
    pTexture->Bind(m_mBindings[name]);
  }
}

void GLProgram::SetTexture(const std::string& name, const GLuint value) {
  assert(isEnabled());
  if (m_mBindings.find(name) == m_mBindings.end ()) {
    // find a free texture unit
    int iUnusedTexUnit = 0;
    for (texMap::iterator i = m_mBindings.begin();i != m_mBindings.end();++i){
      if (i->second <= iUnusedTexUnit)
        iUnusedTexUnit = i->second+1;
    }
    ConnectTextureID(name, iUnusedTexUnit);
    std::cout << "ID FOR : " << name << " - " << iUnusedTexUnit << std::endl;
    
    GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name.c_str());
    GL_CHECK(glUniform1i(currentVariableLocation, iUnusedTexUnit));
    GL_CHECK(glActiveTexture(GLenum(GL_TEXTURE0 + iUnusedTexUnit)));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, value));
    
  } else {
    GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name.c_str());
    GL_CHECK(glUniform1i(currentVariableLocation, m_mBindings[name]));
    GL_CHECK(glActiveTexture(GLenum(GL_TEXTURE0 + m_mBindings[name])));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, value));
  }
}

void GLProgram::SetTexture1D(const std::string& name, const GLuint value, const GLuint shaderLocation){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name.c_str());
  GL_CHECK(glUniform1i(currentVariableLocation, shaderLocation));
  GL_CHECK(glActiveTexture(GLenum(GL_TEXTURE0 + shaderLocation)));
  GL_CHECK(glBindTexture(GL_TEXTURE_1D, value));
}

void GLProgram::SetTexture2D(const std::string& name, const GLuint value, const GLuint shaderLocation){
  assert(isEnabled());
  GLuint currentVariableLocation = glGetUniformLocation(m_ShaderProgramm, name.c_str());
  GL_CHECK(glUniform1i(currentVariableLocation, shaderLocation));
  GL_CHECK(glActiveTexture(GLenum(GL_TEXTURE0 + shaderLocation)));
  GL_CHECK(glBindTexture(GL_TEXTURE_2D, value));
}

void GLProgram::ConnectTextureID(const string& name,
                                 const int iUnit) {
  Enable();
  m_mBindings[name] = iUnit;
  
  GLint location = GetLocation(name.c_str());
  CheckSamplerType(name.c_str());
  GL_CHECK(glUniform1i(location,iUnit));
}

#ifdef GLSL_DEBUG
void GLProgram::CheckSamplerType(const char *name) const {
  GLenum eTypeInShader = get_type(name);
  
  if (eTypeInShader != GL_SAMPLER_1D &&
      eTypeInShader != GL_SAMPLER_2D &&
      eTypeInShader != GL_SAMPLER_3D &&
      eTypeInShader != GL_SAMPLER_CUBE &&
      eTypeInShader != GL_SAMPLER_1D_SHADOW &&
      eTypeInShader != GL_SAMPLER_2D_SHADOW &&
      eTypeInShader != GL_SAMPLER_2D_RECT_ARB &&
      eTypeInShader != GL_SAMPLER_2D_RECT_SHADOW_ARB) {
    LWARNING("Shader definition (%i) does not match any "
             "sampler type." << int(eTypeInShader));
  }
}
#else
void GLProgram::CheckSamplerType(const char *) const { }
#endif

GLuint GLProgram::GetLocation(const char *name) const{
  return glGetUniformLocation(m_ShaderProgramm, name);
}


bool GLProgram::WriteInfoLog(const char* shaderdesc, GLuint hObject,
                             bool bProgram) {
  // Check for errors
  GLint iLength=0;
  if (bProgram)
    glGetProgramiv(hObject,GL_INFO_LOG_LENGTH,&iLength);
  else
    glGetShaderiv(hObject,GL_INFO_LOG_LENGTH,&iLength);
  
#ifdef _DEBUG
  GLenum err = glGetError();
  assert(GL_NO_ERROR == err);
#endif
  
  GLboolean bAtMostWarnings = GL_TRUE;
  if (iLength > 1) {
    char *pcLogInfo=new char[iLength];
    if (bProgram) {
      glGetProgramInfoLog(hObject,iLength,&iLength,pcLogInfo);
      bAtMostWarnings = glIsProgram(hObject);
    } else {
      glGetShaderInfoLog(hObject,iLength,&iLength,pcLogInfo);
      bAtMostWarnings = glIsShader(hObject);
    }
    if (bAtMostWarnings) {
      printf("%s", shaderdesc);
      printf("%s", pcLogInfo);
      delete[] pcLogInfo;
      return false;
    } else {
      printf("%s", shaderdesc);
      printf("%s", pcLogInfo);
      delete[] pcLogInfo;
#ifdef GLSLPROGRAM_STRICT
      return true;
#endif
    }
#ifdef _DEBUG
  } else {
    //MESSAGE("No info log available.");
#endif
  }
  return !bool(bAtMostWarnings==GL_TRUE); // error occured?
}

GLProgram* GLProgram::FromFiles(const std::vector<std::string> strDirs,
                                ...)
{
  // first build list of fragment shaders
  std::vector<std::string> vertex;
  std::vector<std::string> frag;
  
  va_list args;
  va_start(args, strDirs);
  {
    const char* filename;
    // We expect two NULLs; the first terminates the vertex shader list, the
    // latter terminates the fragment shader list.
    
    while(NULL != (filename = va_arg(args, const char*)) ) {
      std::string shader = Core::IO::FileTools::findFileInDirs(std::string(filename), strDirs);
      if (shader.empty()) {
        LERROR("Vertex shader file " << filename << " not found.");
        return nullptr;
      }
      vertex.push_back(shader);
    }
    
    while(NULL != (filename = va_arg(args, const char*)) ) {
      std::string shader = Core::IO::FileTools::findFileInDirs(std::string(filename), strDirs);
      if (shader.empty()) {
        LERROR("Fragment shader file " << filename << " not found.");
        return nullptr;
      }
      frag.push_back(shader);
    }
  }
  
  va_end(args);
  
  return FromFiles(vertex, frag);
}

GLProgram* GLProgram::FromFiles(const std::vector<std::string>& vert,
                                const std::vector<std::string>& frag)
{
  
  for(auto v = vert.begin(); v != vert.end(); ++v)
  {
    if (!Core::IO::FileTools::fileExists(*v)) {
      LERROR("Vertex shader file " << *v << " not found.");
      return nullptr;
    }
  }
  
  for(auto f = frag.begin(); f != frag.end(); ++f) {
    if (!Core::IO::FileTools::fileExists(*f)) {
      LERROR("Fragment shader file " << *f << " not found.");
      return nullptr;
    }
  }
  
  ShaderDescriptor sd(vert, frag);
  GLProgram* program = new GLProgram();
  program->Load(sd);
  
  if(program == NULL || !program->IsValid()) {
    LERROR("Invalid shader.");
    delete program;
    return nullptr;
  }
  
  return program;
}
