//  GLDemoRayCaster.cpp
//  TNG
//
//  Created by Andre Waschk on 23/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.

#include <iostream>
#include <fstream>


#include "GLDemoRayCaster.h"

#include "GLCore/GLProgram.h"
#include "GLCore/GLVolumeBox.h"
#include "GLCore/GLRenderPlane.h"
#include "GLCore/GLBoundingBox.h"
#include "GLCore/GLFrameBuffer.h"
#include "GLCore/GLTexture1D.h"
#include "GLCore/GLFrameBufferObject.h"

#include "GLVolume/GLVolume3DTex.h"
#include <renderer/ShaderDescriptor.h>
#include <IO/VolumeIO/RawVolume.h>


#include <IO/IDataIO.h>

#include <IO/VolumeIO/QVisVolume.h>

#include "GLVolumePool.h"

using namespace Tuvok::Renderer::OpenGL;
using namespace Tuvok::Renderer::OpenGL::GLCore;
using namespace Tuvok::Renderer;
using namespace Core::Math;
using namespace DataIO;



GLDemoRayCaster::GLDemoRayCaster()
    :AbstrRenderer(),
    m_minTF(0.03f),
    m_TransferFunction(nullptr),
    m_VolumeData(nullptr),
    m_pNearPlane(nullptr),
    m_pGLProgramBoundingBox(nullptr),
    m_pBB(nullptr)
{
}
GLDemoRayCaster::~GLDemoRayCaster(){
}

bool GLDemoRayCaster::Initialize(){

  //! initialize the FrameBufferObject to store hte frontfaces of the cube
	/*m_pFrontFace = std::make_shared<GLCore::GLFBOTex>(
		GL_LINEAR,
		GL_LINEAR, GL_CLAMP, 640, 480,
		GL_RGB32F, GL_RGB, GL_FLOAT,
		true, 1
		);*/
  m_pFrontFace = std::make_shared<GLFrameBuffer>();
  m_pFrontFace->Initialize(GetSize(), GL_RGB32F, 1);

  //--- FrontFace Shader
   m_pGLProgramFirstpass = LoadShader("CubeVertex.glsl", "CubeFragment.glsl");


  //--- NearPlane Shader
  m_pGLProgramNearPlane = LoadShader("NearPlaneVS.glsl", "NearPlaneFS.glsl");

  //--- BoundingBox Shader
  m_pGLProgramBoundingBox = LoadShader("GLSL-BoundingBox-VS.glsl", "GLSL-BoundingBox-FS.glsl");


  std::vector<std::string> vs;
  std::vector<std::string> fs;

  vs.push_back("CubeVertex.glsl");
  //fs.push_back("hashtable.glsl");
  fs.push_back("Raytracer2Fragment.glsl");



  //--- RayCast Shader
  m_pGLProgramBackFaceRayCasting = LoadShader(vs, fs);

  m_pGLProgramBackFaceRayCasting->Enable();
  std::cout << "hashtable loc: " << m_pGLProgramBackFaceRayCasting->GetLocation("hashTable") << std::endl;
  std::cout << "volume loc: " << m_pGLProgramBackFaceRayCasting->GetLocation("volume") << std::endl;
  std::cout << "front loc: " << m_pGLProgramBackFaceRayCasting->GetLocation("front") << std::endl;


  //create a surrounding box
  m_pVolumeBox = std::make_shared<GLCore::GLVolumeBox>();
  //create a bounding box
  m_pBB = std::make_shared<GLBoundingBox>();
  //init a nearplane
  m_pNearPlane = std::make_shared<GLRenderPlane>(GetSize());

  //init some matricies
  m_scale.Scaling(1,1,1);
  m_rot.RotationY(m_r);
  m_world = m_scale*m_rot;

  SetViewParameters(45.0f, 0.1f, 100.0f);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);












  /*


  Vec3ui v(300, 300, 300);

  Tuvok::Renderer::OpenGL::GLVolumePool  m_pVolumePool(v, GetDataIO(), GL_NEAREST);


  const std::string poolFragment = m_pVolumePool.GetShaderFragment(
	  3, 4,
	  GLVolumePool::MissingBrickStrategy::OnlyNeeded
	  );
  const std::string hashFragment = m_pHashTable->GetShaderFragment(5);

  vs.clear();
  fs.clear();
  vs.push_back("GLGridLeaper-entry-VS.glsl");
  fs.push_back("GLGridLeaper-blend.glsl");
  fs.push_back("GLGridLeaper-Method-1D.glsl");
  fs.push_back("Compositing.glsl");
  fs.push_back("hashtable.glsl");
  fs.push_back("volpool.glsl");
  ShaderDescriptor sd(vs, fs);
  //sd.AddFragmentShaderString(poolFragment);
  //sd.AddFragmentShaderString(hashFragment);



  GLProgramPtr testgridleaper = LoadShader(vs, fs);

  std::string source = sd.getFragmentSource();

   std::ofstream myfile;
  myfile.open ("gridleaper complete.glsl");
  myfile << source;
  myfile.close();

  */



















  return true;
}

GLCore::GLProgramPtr GLDemoRayCaster::LoadShader(const std::string VertexShader,
                                                 const std::string FragmentShader){
  std::vector<std::string> vertex;
  std::vector<std::string> fragment;

  vertex.push_back(VertexShader);
  fragment.push_back(FragmentShader);

  //initialize a new shader descriptor with a given list of files
  ShaderDescriptor sd(vertex,fragment);

  //create the opengl program
  GLProgramPtr program = std::make_shared<GLCore::GLProgram>();

  //load the shader and compile them
  program->Load(sd);

  return program;
}

GLCore::GLProgramPtr GLDemoRayCaster::LoadShader(const std::vector<std::string> VertexShader,
												 const std::vector<std::string> FragmentShader){

	//initialize a new shader descriptor with a given list of files
	ShaderDescriptor sd(VertexShader, FragmentShader);


	//create the opengl program
	GLProgramPtr program = std::make_shared<GLCore::GLProgram>();

	//load the shader and compile them
	program->Load(sd);

	return program;
}

void GLDemoRayCaster::Cleanup(){
}

bool GLDemoRayCaster::Paint(){
	if (GetDataIO() != nullptr){


		//Render Front Faces
		paintCubeFrontBuffer(m_pFrontFace, GL_BACK);

		//raycast
		raycast();

		//auswerten der hashtable!
		//std::vector<Vec4ui> data = m_pHashTable->GetData();


		//simply print hashtable!
		/*if (data.size() != 0)
		{
			std::cout << "data size: " << data.size() << std::endl;
			for (int i = 0; i < data.size(); ++i){
				std::cout << "brickId: " << data[i].x << " " << data[i].y << " " << data[i].z << std::endl;
			}
		}*/



		Vec3f scale(1,GetDataIO()->GetDomainSize().y/(float)GetDataIO()->GetDomainSize().x,GetDataIO()->GetDomainSize().z/(float)GetDataIO()->GetDomainSize().x);

		Vec3f lod1Scale = scale/2.0f;

		//draw bounding box?
		paintBoundingBox(Vec3f(0,0,0),scale);
	   /*
		paintBoundingBox(Vec3f(scale.x*-0.25,scale.y*-0.25,scale.z*-0.25),lod1Scale);
		paintBoundingBox(Vec3f(scale.x*0.25,scale.y*0.25,scale.z*-0.25),lod1Scale);
		paintBoundingBox(Vec3f(scale.x*-0.25,scale.y*0.25,scale.z*-0.25),lod1Scale);
		paintBoundingBox(Vec3f(scale.x*0.25,scale.y*-0.25,scale.z*-0.25),lod1Scale);

		paintBoundingBox(Vec3f(scale.x*-0.25,scale.y*-0.25,scale.z*0.25),lod1Scale);
		paintBoundingBox(Vec3f(scale.x*0.25,scale.y*0.25,scale.z*0.25),lod1Scale);
		paintBoundingBox(Vec3f(scale.x*-0.25,scale.y*0.25,scale.z*0.25),lod1Scale);
		paintBoundingBox(Vec3f(scale.x*0.25,scale.y*-0.25,scale.z*0.25),lod1Scale);
		*/

		//update
		m_scale.Scaling(1,GetDataIO()->GetDomainSize().y/(float)GetDataIO()->GetDomainSize().x,GetDataIO()->GetDomainSize().z/(float)GetDataIO()->GetDomainSize().x);

		m_world = m_scale;
  }
  return true;
}

void GLDemoRayCaster::SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                  bool decrease_screen_res){
  glViewport(lower_left.x, lower_left.y, upper_right.x-lower_left.x, upper_right.y-lower_left.y);
}

// darws the front face of the surronding cube into a buffer
void GLDemoRayCaster::paintCubeFrontBuffer(GLCore::GLFrameBufferPtr buffer, GLenum cullmode){
  //Render Frontface
  buffer->Write();
  buffer->SetViewport();
  //buffer->ClearFramebuffer(Vec4f(-1.0f,-1.0f,-1.0f,-1.0f));
  glCullFace(cullmode);

  //near plane
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_BLEND);
  glDisable(GL_DEPTH_WRITEMASK);

  m_pGLProgramNearPlane->Enable();
  m_pGLProgramFirstpass->Set("viewMatrix", GetViewMatrix().inverse());
  m_pGLProgramFirstpass->Set("projectionMatrix", GetProjectionMatrix().inverse());
  m_pGLProgramFirstpass->Set("worldMatrix", m_world.inverse());

  m_pNearPlane->paint();



  m_pGLProgramFirstpass->Enable();
  m_pGLProgramFirstpass->Set("worldMatrix", m_world);
  m_pGLProgramFirstpass->Set("viewMatrix", GetViewMatrix());
  m_pGLProgramFirstpass->Set("projectionMatrix", GetProjectionMatrix());


  m_pVolumeBox->paint();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glEnable(GL_DEPTH_WRITEMASK);
}


void GLDemoRayCaster::raycast(){
  glCullFace(GL_FRONT);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  SetViewPort(Vec2ui(0,0),GetSize());

  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBlendFunc(GL_ONE, GL_ZERO);


  m_pGLProgramBackFaceRayCasting->Enable();

  m_pGLProgramBackFaceRayCasting->Set("worldMatrix", m_world);
  m_pGLProgramBackFaceRayCasting->Set("viewMatrix", GetViewMatrix());
  m_pGLProgramBackFaceRayCasting->Set("projectionMatrix", GetProjectionMatrix());
  m_pGLProgramBackFaceRayCasting->Set("domainSize", Vec3f(GetDataIO()->GetDomainSize().x,GetDataIO()->GetDomainSize().y,GetDataIO()->GetDomainSize().z));


  m_pGLProgramBackFaceRayCasting->Set("viewInverse",GetViewMatrix().inverse());

  //bind the dummy single iso value
  m_pGLProgramBackFaceRayCasting->Set("minTF",GetIsoValue());


  //bind the buffers of the cubes front and backface
  m_pGLProgramBackFaceRayCasting->SetTexture("front", m_pFrontFace->GetFramebufferTexture(0));
  //m_pGLProgramBackFaceRayCasting->SetTexture("front", m_pFrontFace->GetTextureHandle());

  //bind the volume
  m_VolumeData->Bind(m_pGLProgramBackFaceRayCasting,"volume");

  //clear hashtable for next draw
  //m_pHashTable->ClearData();
  //enable hashtable
  //m_pHashTable->Enable();

 // std::cout << m_pGLProgramBackFaceRayCasting->GetLocation("hashTable") << std::endl;

  m_pVolumeBox->paint();
}


void GLDemoRayCaster::paintBoundingBox(Core::Math::Vec3f vCenter, Core::Math::Vec3f vExtend){

  Mat4f translation,scaling,world;
  translation.Translation(vCenter);
  scaling.Scaling(vExtend);
  world = scaling*translation;

  m_pGLProgramBoundingBox->Enable();
  m_pGLProgramBoundingBox->Set("worldMatrix", world);
  m_pGLProgramBoundingBox->Set("viewMatrix", GetViewMatrix());
  m_pGLProgramBoundingBox->Set("projectionMatrix", GetProjectionMatrix());


  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glBlendFunc(GL_ONE_MINUS_DST_ALPHA,GL_DST_ALPHA);

  m_pBB->paint(GL_LINES);

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  m_pBB->paint(GL_LINES);
}


void GLDemoRayCaster::SetDataset(Tuvok::DataIOPtr dio){
	AbstrRenderer::SetDataset(dio);

	DataIOPtr volumeIO = GetDataIO();
	Vec3ui64 domainsize = volumeIO->GetDomainSize();

	if (!volumeIO->GetIsSigned()){
		switch (volumeIO->GetBitWidth()) {
		case 8:
		{
				  m_VolumeData = std::make_shared<GLVolume::GLVolume3DTex>(domainsize.x, domainsize.y, domainsize.z, GL_R8, GL_RED, GL_UNSIGNED_BYTE, volumeIO->GetBrick(), GL_LINEAR, GL_LINEAR);
				  break;
		}
		case 16:
		{
				   m_VolumeData = std::make_shared<GLVolume::GLVolume3DTex>(domainsize.x, domainsize.y, domainsize.z, GL_R16, GL_RED, GL_UNSIGNED_SHORT, volumeIO->GetBrick(), GL_LINEAR, GL_LINEAR);
				   break;
		}
		case 32:
		{
				   std::cout << "doing this" << std::endl;
				   m_VolumeData = std::make_shared<GLVolume::GLVolume3DTex>(domainsize.x, domainsize.y, domainsize.z, GL_R32UI, GL_RED, GL_UNSIGNED_INT, volumeIO->GetBrick(), GL_LINEAR, GL_LINEAR);
				   break;
		}

		}

	}
	else{
		//....
	}

}
