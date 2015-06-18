#include "logging/logmanager.h"

#include "GLGridLeaper.h"



#include "GLCore/GLVolumeBox.h"
#include "GLCore/GLRenderPlane.h"
#include "GLCore/GLBoundingBox.h"
#include "GLCore/GLTexture1D.h"

#include "GLVolume/GLVolume3DTex.h"

#include "GLVolumePool.h"
#include "GLHashTable.h"
#include "GLTargetBinder.h"

#include "GLCore/GLFrameBufferObject.h"
#include "GLCore/GLProgram.h"

#include <renderer/ShaderDescriptor.h>
#include <renderer/VisibilityState.h>

#include <IO/TransferFunction1D.h>
#include <renderer/Camera.h>

#include <IO/Service/IOLocal.h>

#include <iostream>
#include <fstream>
#include <thread>


#include <core/FileFinder.h>

using namespace Tuvok;
using namespace Tuvok::Renderer;
using namespace Tuvok::Renderer::Context;
using namespace Tuvok::Renderer::OpenGL;
using namespace Tuvok::Renderer::OpenGL::GLCore;
using namespace Tuvok;

using namespace Core::Math;
using namespace std;
using namespace DataIO;

static int screenshot(int i)
{
	unsigned char *pixels;
	FILE *image;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	pixels = new unsigned char[viewport[2] * viewport[3] * 3];

	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGB,
		GL_UNSIGNED_BYTE, pixels);

	char tempstring[50];
	sprintf(tempstring, "screenshot_%i.tga", i);
	if ((image = fopen(tempstring, "wb")) == NULL) return 1;

	unsigned char TGAheader[12] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	unsigned char header[6] = { static_cast<unsigned char>(((int)(viewport[2] % 256))),
		static_cast<unsigned char>(((int)(viewport[2] / 256))),
		static_cast<unsigned char>(((int)(viewport[3] % 256))),
		static_cast<unsigned char>(((int)(viewport[3] / 256))), 24, 0 };

	// TGA header schreiben
	fwrite(TGAheader, sizeof(unsigned char), 12, image);
	// Header schreiben
	fwrite(header, sizeof(unsigned char), 6, image);

	fwrite(pixels, sizeof(unsigned char),
		viewport[2] * viewport[3] * 3, image);

	fclose(image);
	delete[] pixels;

	return 0;
}

static void CheckGLError(std::string comment){
	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
        LWARNINGC("GLGridLeaper"," Error occured (" << static_cast<unsigned int>(err) <<") comment: " << comment );
		//printf("[GLGridLeaper] Unknown error (%x) occurred | Comment : %s \n", static_cast<unsigned int>(err), comment.c_str());
	}
}

//! initializes a opengl based gridleaper with default values
GLGridLeaper::GLGridLeaper(std::shared_ptr<Tuvok::Renderer::Context::Context> context,Vec2ui vWinSize, ERenderMode mode) :
AbstrRenderer(context,vWinSize, mode),
m_pFBORayStart(nullptr),
m_pFBORayStartNext(nullptr),
m_pFBOStartColor(nullptr),
m_pFBOStartColorNext(nullptr),
m_pFBOFinalColor(nullptr),
m_pFBOFinalColorNext(nullptr),
m_pTargetBinder(nullptr),
m_pVolumePool(nullptr),
m_VisibilityState(new VisibilityState()),
m_iTimestep(0),
m_iTransferFunctionSize(0),
m_bTFRescale(false),
m_bCreateFrameBuffer(false),
m_lastFrame(0),
m_nextFrame(0),
m_pixels(),
m_width(640),
m_height(480),
m_vVolumePicking(-1,-1,-1,0),
m_componentCount(3) {
}

//! cleanup
GLGridLeaper::~GLGridLeaper(){
	Cleanup();
	CleanupShader();
	CleanupTraversalShader();
	CleanupFrameBuffer();
	CleanupGeometry();
}



//! initializes the renderer \todo more parameters like resolution etc
bool GLGridLeaper::Initialize(uint64_t gpuMemorySizeInByte){
    LDEBUGC("GLGRIDLEAPER","will lock the context");
    m_pContext->lockContext();
	//create the target binder
	LDEBUGC("GLGRIDLEAPER", "Creating Target Binder");
	m_pTargetBinder = std::make_shared<GLTargetBinder>();

	//check if the dataset and transferfunction is set
	LDEBUGC("GLGRIDLEAPER", "checking for datasethandle");
	if (m_pToCDataset == nullptr || m_pTransferFunction == nullptr) return false;

	//! cache reused datasetinfos \todo move to io!
	m_DatasetCache.m_DomainSize = m_pToCDataset->GetDomainSize();
	m_DatasetCache.m_Scale = m_pToCDataset->GetScale();
    if(m_pToCDataset->GetComponentCount() == 4){
        m_pRenderState->m_bIsColor = true;
    }else{
        m_pRenderState->m_bIsColor = false;
    }

	computeMathMember();

	//inititalize parts of the gridleaper
	LDEBUGC("GLGRIDLEAPER", "Init offscreen buffers");
	InitOffScreenBuffers();
	LDEBUGC("GLGRIDLEAPER", "Init volume geometry");
	InitGeometry();
	LDEBUGC("GLGRIDLEAPER", "Init hashtbale");
	InitHashTable();
	LDEBUGC("GLGRIDLEAPER", "Init volumepool");
	InitVolumePool(gpuMemorySizeInByte);

	//try to load the shaders
	LDEBUGC("GLGRIDLEAPER", "Load compose shader");
	if (!LoadComposeShader()) return false;
	LDEBUGC("GLGRIDLEAPER", "Load frontface shaders");
	if (!LoadFrontFaceShader()) return false;
	LDEBUGC("GLGRIDLEAPER", "Load traversel shaders");
	if (!LoadTraversalShader(MissingBrickStrategy::OnlyNeeded)) return false;

	//relocate this
	SetViewParameters(45.0f, 0.1f, 100.0f);
	SetViewPort(Vec2ui(0, 0), GetSize());

	m_fLODFactor = 2.0f * tan(45.0f * ((3.1416f / 180.0f) / 2.0f)) *
		1.0f / float(GetSize().y);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//check for error
	CheckGLError("ERROR AFTER INIT!");

	CalculateUsedGPUMemory();

	m_pContext->unlockContext();

	//start runthread;
	//LINFOC("GLGridLeaper","try to start new renderthread");
	//m_pRenderThread = std::make_shared<std::thread>(&GLGridLeaper::run, this);

    LINFOC("GLGridLeaper","return");
	return true;
}



//! cleanup
void GLGridLeaper::Cleanup(){
    CleanupShader();
    CleanupTraversalShader();
    CleanupFrameBuffer();
    CleanupGeometry();
}
void GLGridLeaper::CleanupShader(){
	m_pProgramRenderFrontFaces.reset();
	m_pProgramRenderFrontFacesNearPlane.reset();
	m_pProgramCompose.reset();
	m_pProgramComposeColorDebugMix.reset();
	m_pProgramComposeColorDebugMixAlpha.reset();
	m_pProgramComposeClearViewIso.reset();
}
void GLGridLeaper::CleanupTraversalShader(){
	m_pProgramRayCast1D.reset();
	m_pProgramRayCast1DColor.reset();
	m_pProgramRayCast1DLighting.reset();
	m_pProgramRayCastISO.reset();
	m_pProgramRayCastISOLighting.reset();
	m_pProgramRayCastISOColor.reset();
}
void GLGridLeaper::CleanupFrameBuffer(){
	m_pFBORayStart.reset();
	m_pFBORayStartNext.reset();
	m_pFBOStartColor.reset();
	m_pFBOStartColorNext.reset();
	m_pFBOFinalColor.reset();
	m_pFBOFinalColorNext.reset();
#ifdef GLGRIDLEAPER_DEBUGVIEW
	m_pFBODebug.reset();
	m_pFBODebugNext.reset();
#endif
}
void GLGridLeaper::CleanupGeometry(){
	m_pVolumeBox.reset();
	m_pNearPlane.reset();
}



//switch paging strategy
void GLGridLeaper::SwitchPagingStrategy(MissingBrickStrategy brickStrategy){
	CleanupTraversalShader();
	LoadTraversalShader(brickStrategy);
}



//! \brief main paint function for the renderpath
bool GLGridLeaper::Paint(){
    //! check for new framestart
	RecreateAfterResize();
	if (m_bCompleteRedraw){
        if(m_pRenderState->m_bClearViewEnabled){
            swapBuffers(true);
            FillRayEntryBuffer();
		}
		swapBuffers(false);
		FillRayEntryBuffer();
		m_bCompleteRedraw = false;
	}

	//! raycast
	m_pglHashTable->ClearData();
	if (!m_pRenderState->m_bClearViewEnabled)
        Raycast(m_pRenderState->m_eRenderMode);
    else
        Raycast(RM_ISOSURFACE);
	swapToNextBuffer(false);

	//! update volumepool
	std::vector<Vec4ui> hash = m_pglHashTable->GetData();
	if (hash.size() > 0){
		m_pVolumePool->UploadBricks(hash, true);
	}


	//clear view IFF m_bClearView == true
	ClearView();

	//! 5) final compose
	FinalCompose();


    //! \todo remove, just for michael atm
	if (m_bCreateFrameBuffer){
		readFB();
		m_bCreateFrameBuffer = false;
		m_nextFrame++;

	}
	return true;

}



void GLGridLeaper::ClearView(){
	if (m_pRenderState->m_bClearViewEnabled){
		float isoBackup = m_pRenderState->m_fIsoValue;
		m_pRenderState->m_fIsoValue = 0.02; // remove !
		swapBuffers(true);

		//! 2) raycast IN
		m_pglHashTable->ClearData();
		Raycast(m_pRenderState->m_eRenderMode);
		swapToNextBuffer(true);
		//! 3) hashtable IN
		std::vector<Vec4ui> hash = m_pglHashTable->GetData();

		//! 4) update IN
		if (hash.size() > 0){
			m_pVolumePool->UploadBricks(hash, true);
		}
		m_pRenderState->m_fIsoValue = isoBackup;  // remove !
		//FINAL COMPOSE
		swapBuffers(false);
	}
}



//! setting the viewport \note add renderregion
void GLGridLeaper::SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
	bool decrease_screen_res){
	glViewport(lower_left.x, lower_left.y, upper_right.x, upper_right.y);
}



//! ----- Shader Init ------------------------------------------
bool GLGridLeaper::LoadTraversalShader(MissingBrickStrategy brickStrategy){

	const std::string poolFragment = m_pVolumePool->GetShaderFragment(
		3, 4,
		GLVolumePool::MissingBrickStrategy(brickStrategy)
		);
	const std::string hashFragment = m_pglHashTable->GetShaderFragment(5);

	//! \todo -> add FindFileInDirs
	//! 1D TransferFunction Shader---------------------------
	std::vector<std::string> vs, fs;
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-entry-VS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-blend.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-Method-1D.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"Compositing.glsl"));
	ShaderDescriptor sd(vs, fs);
	//sd.AddDefines(vDefines);
	sd.AddFragmentShaderString(poolFragment);
	sd.AddFragmentShaderString(hashFragment);
	if (!LoadCheckShader(m_pProgramRayCast1D, sd, "1D TF")) return false;

#ifdef DEBUG_OUTS
	std::string fsString = sd.getFragmentSource();
	std::string vsString = sd.getVertexSource();

	std::ofstream out;
	out.open("COMPLETE_GRIDLEAPER_FRAGMENT_1dTF.glsl");
	out << fsString;
	out.close();
	out.open("COMPLETE_GRIDLEAPER_VERTEX_1dTF.glsl");
	out << vsString;
	out.close();
#endif

	//! 1D TransferFunction Color Shader----------------------
	vs.clear(); fs.clear();
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-entry-VS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-blend.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-Method-1D-color.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"Compositing.glsl"));
	sd = ShaderDescriptor(vs, fs);
	//sd.AddDefines(vDefines);
	sd.AddFragmentShaderString(poolFragment);
	sd.AddFragmentShaderString(hashFragment);
#ifdef GLGRIDLEAPER_WORKINGSET
	sd.AddFragmentShaderString(infoFragment);
#endif
	if (!LoadCheckShader(m_pProgramRayCast1DColor, sd, "Color 1D TF")) return false;

	//! 1D TransferFunction Lighting Shader--------------------
	vs.clear(); fs.clear();
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-entry-VS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-blend.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-Method-1D-L.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-GradientTools.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"lighting.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"Compositing.glsl"));
	sd = ShaderDescriptor(vs, fs);
	//sd.AddDefines(vDefines);
	sd.AddFragmentShaderString(poolFragment);
	sd.AddFragmentShaderString(hashFragment);
#ifdef GLGRIDLEAPER_WORKINGSET
	sd.AddFragmentShaderString(infoFragment);
#endif
	if (!LoadCheckShader(m_pProgramRayCast1DLighting, sd, "1D TF lighting")) return false;

	//! IsoSurface Shader-------------------------------------
	vs.clear(); fs.clear();
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-entry-VS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-iso.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-Method-iso.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-GradientTools.glsl"));
	sd = ShaderDescriptor(vs, fs);
	//sd.AddDefines(vDefines);
	sd.AddFragmentShaderString(poolFragment);
	sd.AddFragmentShaderString(hashFragment);
#ifdef GLGRIDLEAPER_WORKINGSET
	sd.AddFragmentShaderString(infoFragment);
#endif
	if (!LoadCheckShader(m_pProgramRayCastISO, sd, "Isosurface")) return false;


	vs.clear(); fs.clear();
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-entry-VS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-iso.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-Method-iso-color.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-GradientTools.glsl"));
	sd = ShaderDescriptor(vs, fs);
	//sd.AddDefines(vDefines);
	sd.AddFragmentShaderString(poolFragment);
	sd.AddFragmentShaderString(hashFragment);
#ifdef GLGRIDLEAPER_WORKINGSET
	sd.AddFragmentShaderString(infoFragment);
#endif
	if (!LoadCheckShader(m_pProgramRayCastISOColor, sd, "Color Isosurface")) return false;

	vs.clear(); fs.clear();
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-entry-VS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-iso-lighting.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-Method-iso.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"GLGridLeaper-GradientTools.glsl"));
	sd = ShaderDescriptor(vs, fs);
	//sd.AddDefines(vDefines);
	sd.AddFragmentShaderString(poolFragment);
	sd.AddFragmentShaderString(hashFragment);
#ifdef GLGRIDLEAPER_WORKINGSET
	sd.AddFragmentShaderString(infoFragment);
#endif
	if (!LoadCheckShader(m_pProgramRayCastISOLighting, sd, "Color Isosurface Lighting")) return false;


	return true;
}




bool GLGridLeaper::LoadFrontFaceShader(){
	std::vector<std::string> vs, fs;
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"CubeVertex.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"CubeFragment.glsl"));
	ShaderDescriptor sdCube(vs, fs);
	if (!LoadCheckShader(m_pProgramRenderFrontFaces, sdCube, "CubeFrontFaces")) return false;

	vs.clear();
	fs.clear();
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"NearPlaneVS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"NearPlaneFS.glsl"));
	ShaderDescriptor sdNear(vs, fs);
	if (!LoadCheckShader(m_pProgramRenderFrontFacesNearPlane, sdNear, "NearFrontFace")) return false;

	return true;
}



bool GLGridLeaper::LoadComposeShader(){
	std::vector<std::string> vs, fs;
	vs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"ComposeVS.glsl"));
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"ComposeFS.glsl"));
	ShaderDescriptor sdCube(vs, fs);
	if (!LoadCheckShader(m_pProgramCompose, sdCube, "ComposeShader")) return false;


	fs.clear();
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"ComposeFSColorDebug.glsl"));
	ShaderDescriptor sdComposeFinalColorDebug(vs, fs);
	if (!LoadCheckShader(m_pProgramComposeColorDebugMix, sdComposeFinalColorDebug, "ComposeShaderColorDebug")) return false;

	fs.clear();
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"ComposeFSColorDebugAlpha.glsl"));
	ShaderDescriptor sdComposeFinalColorDebugAlpha(vs, fs);
	if (!LoadCheckShader(m_pProgramComposeColorDebugMixAlpha, sdComposeFinalColorDebugAlpha, "ComposeShaderColorDebugAlpha")) return false;

    fs.clear();
	fs.push_back(Core::FileFinder::getInstance().findFilePath(m_vSearchPathes,"ComposeFS_CViso.glsl"));
	ShaderDescriptor sdComposeFinalCViso(vs, fs);
	if (!LoadCheckShader(m_pProgramComposeClearViewIso, sdComposeFinalCViso, "ComposeShaderCViso")) return false;

	return true;
}



bool GLGridLeaper::LoadCheckShader(GLCore::GLProgramPtr& programptr, ShaderDescriptor& sd, std::string name){
	//create the opengl program
	programptr = std::make_shared<GLCore::GLProgram>();

	//load the shader and compile them
	programptr->Load(sd);

	if (!programptr->IsValid()){
		return false;
	}

	return true;
}
//! ----- Shader Init End --------------------------------------


//! ----- Member Init ------------------------------------------
//! Initialize offscreen buffers, also initialize the clearview buffers
bool GLGridLeaper::InitOffScreenBuffers(){
	GLenum format = GL_RGBA32F;
	m_pFBORayStartMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, GL_RGBA32F, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBORayStartNextMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, GL_RGBA32F, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOStartColorMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOStartColorNextMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOFinalColorMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOFinalColorNextMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBORayStartCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, GL_RGBA32F, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBORayStartNextCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, GL_RGBA32F, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOStartColorCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOStartColorNextCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOFinalColorCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBOFinalColorNextCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);

#ifdef GLGRIDLEAPER_DEBUGVIEW
	m_pFBODebugMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBODebugNextMAIN = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBODebugCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
	m_pFBODebugNextCV = std::make_shared<GLCore::GLFBOTex>(GL_NEAREST, GL_NEAREST, GL_CLAMP, GetSize().x, GetSize().y, format, GL_RGBA, GL_FLOAT, false, 1);
#endif

    m_pFBORayStart = m_pFBORayStartMAIN;
	m_pFBORayStartNext = m_pFBORayStartNextMAIN;
    m_pFBOStartColor = m_pFBOStartColorMAIN;
	m_pFBOStartColorNext = m_pFBOStartColorNextMAIN;
	m_pFBOFinalColor = m_pFBOFinalColorMAIN;
	m_pFBOFinalColorNext = m_pFBOFinalColorNextMAIN;
    m_pFBODebug = m_pFBODebugMAIN;
	m_pFBODebugNext = m_pFBODebugNextMAIN;
	return true;
}



//swap between inner and outer clearview faces
void GLGridLeaper::swapBuffers(bool clearview){
    if(!clearview){
        m_pFBORayStart = m_pFBORayStartMAIN;
        m_pFBORayStartNext = m_pFBORayStartNextMAIN;
        m_pFBOStartColor = m_pFBOStartColorMAIN;
        m_pFBOStartColorNext = m_pFBOStartColorNextMAIN;
        m_pFBOFinalColor = m_pFBOFinalColorMAIN;
        m_pFBOFinalColorNext = m_pFBOFinalColorNextMAIN;
        m_pFBODebug = m_pFBODebugMAIN;
        m_pFBODebugNext = m_pFBODebugNextMAIN;
    }else{
        m_pFBORayStart = m_pFBORayStartCV;
        m_pFBORayStartNext = m_pFBORayStartNextCV;
        m_pFBOStartColor = m_pFBOStartColorCV;
        m_pFBOStartColorNext = m_pFBOStartColorNextCV;
        m_pFBOFinalColor = m_pFBOFinalColorCV;
        m_pFBOFinalColorNext = m_pFBOFinalColorNextCV;
        m_pFBODebug = m_pFBODebugCV;
        m_pFBODebugNext = m_pFBODebugNextCV;
    }
}



void GLGridLeaper::recreateFBO(){
	CleanupFrameBuffer();
	InitOffScreenBuffers();
}

bool GLGridLeaper::RecreateAfterResize(){
	if (AbstrRenderer::RecreateAfterResize()){
        m_fLODFactor = 2.0f * tan(45.0f * ((3.1416f / 180.0f) / 2.0f)) *
		1.0f / float(GetSize().y);
		recreateFBO();
		SetViewPort(Vec2ui(0, 0), GetSize());
		return true;
	}
	return false;
}



//! \todo The HT_SIZE is now set by a default value 509, taken out of the old mastercontroller, change it
void GLGridLeaper::InitHashTable() {
	Vec3ui const finestBrickLayout(m_pToCDataset->GetBrickLayout(0, 0));

	unsigned ht_size = 509; // default taken out of old mastercontroller
	if (ht_size == 0){ // with default value this will never get executed !
		const float rmax = m_pToCDataset->GetMaxBrickSize().volume() / 32768.;
		unsigned ht_size = std::max<unsigned>(512, static_cast<unsigned>(509 / rmax));
	}
	LDEBUGC("GLGridLeaper" , "Using " << ht_size <<"-element hash table.");

	uint32_t rehashcount = 10; // default taken out of old mastercontroller
	m_pglHashTable = std::make_shared<GLHashTable>(finestBrickLayout, uint32_t(ht_size), rehashcount);
	m_pglHashTable->InitGL();
}



//! \todo fix init volumepool
void GLGridLeaper::InitVolumePool(uint64_t gpuMemorySizeInByte){
	Vec3ui64 size = m_DatasetCache.m_DomainSize;

	CalculateUsedGPUMemory();
	Vec3ui volumepoolsize = CalculateVolumePoolSize(gpuMemorySizeInByte);

	//! \todo volume size ? how ?
	m_pVolumePool = std::make_shared<GLVolumePool>(volumepoolsize, m_pToCDataset, GL_LINEAR);


	if (m_pVolumePool){
		// upload a brick that covers the entire domain to make sure have
		// something to render

		// find lowest LoD with only a single brick
		//Vec3ui64 temp = m_pToCDataset->IndexFrom4D(UINTVECTOR4(0, 0, 0, uint32_t(m_pToCDataset->GetLargestSingleBrickLOD(0))), 0);
		const Tuvok::BrickKey bkey = m_pToCDataset->IndexFrom4D(UINTVECTOR4(0, 0, 0, uint32_t(m_pToCDataset->GetLargestSingleBrickLOD(0))), 0);

		m_pVolumePool->UploadFirstBrick(bkey);

		RecomputeBrickVisibility(false);
	}
}



void GLGridLeaper::InitGeometry(){
	//create a surrounding box
	m_pVolumeBox = std::make_shared<GLCore::GLVolumeBox>();
	//init a nearplane
	m_pNearPlane = std::make_shared<GLRenderPlane>(GetSize());
}
//! ----- Member Init End ------------------------------------------

//! Render Entrance
void GLGridLeaper::FillRayEntryBuffer(){
	//! clear all the old buffers, we start from scratch
#ifdef GLGRIDLEAPER_DEBUGVIEW
	m_pTargetBinder->Bind(m_pFBOFinalColor, m_pFBOFinalColorNext, m_pFBOStartColor, m_pFBOStartColorNext);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	m_pTargetBinder->Bind(m_pFBODebug, m_pFBODebugNext,m_pFBORayStart,m_pFBORayStartNext);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
#else
	m_pTargetBinder->Bind(m_pFBOFinalColor, m_pFBOFinalColorNext, m_pFBOStartColor, m_pFBOStartColorNext);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_pTargetBinder->Bind(m_pFBORayStart,m_pFBORayStartNext);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
#endif
	m_pTargetBinder->Unbind();


	//bind the entrance buffer as target
	m_pTargetBinder->Bind(m_pFBORayStart);

	//! \todo add the statemanager here !
	glCullFace(GL_BACK);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);


	m_pProgramRenderFrontFacesNearPlane->Enable();
	m_pProgramRenderFrontFacesNearPlane->Set("viewMatrixInverse", GetViewMatrix().inverse());
	m_pProgramRenderFrontFacesNearPlane->Set("projectionMatrixInverse", GetProjectionMatrix().inverse());
	m_pProgramRenderFrontFacesNearPlane->Set("worldMatrixInverse", m_mWorldMatrix.inverse());

	m_pNearPlane->paint();


	m_pProgramRenderFrontFaces->Enable();
	m_pProgramRenderFrontFaces->Set("worldMatrix", m_mWorldMatrix);
	m_pProgramRenderFrontFaces->Set("viewMatrix", GetViewMatrix());
	m_pProgramRenderFrontFaces->Set("projectionMatrix", GetProjectionMatrix());

	m_pVolumeBox->paint();

	//reset opengl
	m_pProgramRenderFrontFacesNearPlane->Disable();
	m_pTargetBinder->Unbind();
	m_pFBORayStart->FinishWrite();
}

//! Raycast
void GLGridLeaper::Raycast(Tuvok::Renderer::ERenderMode rendermode){
	CheckGLError("Error befor Raycast Function");

	m_pTargetBinder->Bind(m_pFBORayStartNext);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	//Stop write to old shaders (just for safety)
	m_pFBOFinalColor->FinishWrite();
	m_pFBORayStart->FinishWrite();
	m_pFBOStartColor->FinishWrite();

	//bind targets
#ifdef GLGRIDLEAPER_DEBUGVIEW
	m_pTargetBinder->Bind(m_pFBOFinalColorNext,
		m_pFBOStartColorNext,
		m_pFBORayStartNext,
		m_pFBODebugNext);
#else
	m_pTargetBinder->Bind(m_pFBOFinalColorNext,
		m_pFBOStartColorNext,
		m_pFBORayStartNext);
#endif

	//setup the shader

    SelectShader(rendermode);
	SetupRaycastShader(rendermode);
	CheckGLError("Error after Raycast SETUP Function");

	//set states
	glCullFace(GL_FRONT);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//render volumebox
	m_pVolumeBox->paint();


	CheckGLError("Error after Raycast Function");
}

//swap to next buffer
void GLGridLeaper::swapToNextBuffer(bool clearview){
if(!clearview){
    std::swap(m_pFBOFinalColorMAIN, m_pFBOFinalColorNextMAIN);
	std::swap(m_pFBORayStartMAIN, m_pFBORayStartNextMAIN);
	std::swap(m_pFBOStartColorMAIN, m_pFBOStartColorNextMAIN);
#ifdef GLGRIDLEAPER_DEBUGVIEW
	std::swap(m_pFBODebugMAIN, m_pFBODebugNextMAIN);
#endif
    swapBuffers(false);
}
else{
    std::swap(m_pFBOFinalColorCV, m_pFBOFinalColorNextCV);
	std::swap(m_pFBORayStartCV, m_pFBORayStartNextCV);
	std::swap(m_pFBOStartColorCV, m_pFBOStartColorNextCV);
#ifdef GLGRIDLEAPER_DEBUGVIEW
	std::swap(m_pFBODebugCV, m_pFBODebugNextCV);
#endif
    swapBuffers(true);
}
}

void GLGridLeaper::SelectShader(Tuvok::Renderer::ERenderMode rendermode){
	GLProgramPtr temp = m_pCurrentShaderProgram;
	if (rendermode == RM_1DTRANS){
		if (!m_pRenderState->m_bUseLighting){
			m_pCurrentShaderProgram = m_pProgramRayCast1D;
		}
		else{
			if (m_pRenderState->m_bIsColor){
				m_pCurrentShaderProgram = m_pProgramRayCast1DColor;
			}
			else{
				m_pCurrentShaderProgram = m_pProgramRayCast1DLighting;
			}
		}
	}
	else if (rendermode == RM_ISOSURFACE){
		/*if (m_pRenderState->m_bIsColor)
            m_pCurrentShaderProgram = m_pProgramRayCastISOColor;
		else*/
            m_pCurrentShaderProgram = m_pProgramRayCastISOLighting;
	}

	if (temp != m_pCurrentShaderProgram){
		m_bCompleteRedraw = true;
	}
}

//! setup raycast shader
void GLGridLeaper::SetupRaycastShader(Tuvok::Renderer::ERenderMode rendermode){
	//! put matrix calculation in seperate method!
	Mat4f mv = m_mWorldMatrix*GetViewMatrix();
	Mat4f mvp = mv*GetProjectionMatrix();
	Mat4f mvInverse = mv.inverse();


	m_pCurrentShaderProgram->Enable(); //! \note somehow error on intel machines

	m_pCurrentShaderProgram->Set("mModelViewProjection", mvp);
	m_pCurrentShaderProgram->Set("mModelView", mv);

	m_pCurrentShaderProgram->Set("sampleRateModifier", GetSampleRateModifier());
	m_pCurrentShaderProgram->Set("mEyeToModel", mvInverse);
	m_pCurrentShaderProgram->Set("mEyeToModel2", GetViewMatrix().inverse());

	//setup textures
	m_pCurrentShaderProgram->SetTexture2D("rayStartPoint", m_pFBORayStart->GetTextureHandle(), 0);
	m_pCurrentShaderProgram->SetTexture2D("rayStartColor", m_pFBOStartColor->GetTextureHandle(), 1);


	//! \todo change the scale factor
	m_pVolumePool->Enable(m_fLODFactor,
		m_vExtend, m_vScale, m_pCurrentShaderProgram);
	m_pglHashTable->Enable();


	if (rendermode == RM_ISOSURFACE) {
		m_pCurrentShaderProgram->Set("fIsoval", GetIsoValue());
		FLOATVECTOR3 scale = 1.0f / m_vScale;
		m_pCurrentShaderProgram->Set("vDomainScale", m_vScale);
		m_pCurrentShaderProgram->Set("mModelToEye", mvInverse.inverse());
		m_pCurrentShaderProgram->Set("mModelViewIT", mv.inverse());


		Vec3f a = m_pRenderState->m_cAmbient.xyz()*m_pRenderState->m_cAmbient.w;
		Vec3f d = m_pRenderState->m_cDiffuse.xyz()*m_pRenderState->m_cDiffuse.w;
		Vec3f s = m_pRenderState->m_cSpecular.xyz()*m_pRenderState->m_cSpecular.w;
		Vec3f vModelSpaceLightDir = (Vec4f(m_pRenderState->m_vLightDir, 0.0f) * scale).xyz().normalized();
		Vec3f vModelSpaceEyePos = (Vec4f(0, 0, 0, 1) * scale).xyz();


		vModelSpaceLightDir = m_pRenderState->m_pCameraPtr.GetWorldPosition();;
		vModelSpaceEyePos = m_pRenderState->m_pCameraPtr.GetWorldPosition();;
		vModelSpaceLightDir.normalize();


		m_pCurrentShaderProgram->Set("vLightAmbient", a);
		m_pCurrentShaderProgram->Set("vLightDiffuse", d);
		m_pCurrentShaderProgram->Set("vLightSpecular", s);
		m_pCurrentShaderProgram->Set("vModelSpaceLightDir", vModelSpaceLightDir);
		m_pCurrentShaderProgram->Set("vModelSpaceEyePos", vModelSpaceEyePos);

	}
	else {
		//float fScale = CalculateScaling();

		//fast rescale hack, fix after cebit and exams
		float rate = 1.0f;
		if (m_bTFRescale){
			bool m_bDownSampleTo8Bits = false;
			double fMaxValue;
			if (m_pVolumePool->GetCacheInfo().m_iBitWidth != 8 && m_bDownSampleTo8Bits)
				fMaxValue = 255;
			else{
				fMaxValue = ((m_pVolumePool->GetCacheInfo().m_vRange.x > m_pVolumePool->GetCacheInfo().m_vRange.y) ? 3666 : m_pVolumePool->GetCacheInfo().m_vRange.y);
			}


			uint32_t iMaxRange = uint32_t(1 << m_pVolumePool->GetCacheInfo().m_iBitWidth);
			rate = (m_pVolumePool->GetCacheInfo().m_iBitWidth != 8 && m_bDownSampleTo8Bits) ?
				1.0f : float(iMaxRange / fMaxValue);
		}
		//fast hack ends here
		m_pCurrentShaderProgram->Set("fTransScale", rate);
		m_pCurrentShaderProgram->SetTexture1D("transferFunction", m_pTransferFunctionTexture->GetGLID(), 2);



		if (m_pRenderState->m_bUseLighting) {
			Vec3f a = m_pRenderState->m_cAmbient.xyz()*m_pRenderState->m_cAmbient.w;
			Vec3f d = m_pRenderState->m_cDiffuse.xyz()*m_pRenderState->m_cDiffuse.w;
			Vec3f s = m_pRenderState->m_cSpecular.xyz()*m_pRenderState->m_cSpecular.w;

			Vec3f scale = 1.0f / m_vScale;
			Vec3f vModelSpaceLightDir = (Vec4f(m_pRenderState->m_vLightDir, 0.0f) * scale).xyz().normalized();
			Vec3f vModelSpaceEyePos = (Vec4f(0, 0, 0, 1) * scale).xyz();

			vModelSpaceLightDir = m_pRenderState->m_pCameraPtr.GetWorldPosition();;
			vModelSpaceEyePos = m_pRenderState->m_pCameraPtr.GetWorldPosition();;
			vModelSpaceLightDir.normalize();

			m_pCurrentShaderProgram->Set("vLightAmbient", a);
			m_pCurrentShaderProgram->Set("vLightDiffuse", d);
			m_pCurrentShaderProgram->Set("vLightSpecular", s);
			m_pCurrentShaderProgram->Set("vModelSpaceLightDir", vModelSpaceLightDir);
			m_pCurrentShaderProgram->Set("vModelSpaceEyePos", vModelSpaceEyePos);
			m_pCurrentShaderProgram->Set("vDomainScale", scale);
		}
	}

#ifdef GLGRIDLEAPER_DEBUGVIEW
	m_pCurrentShaderProgram->SetTexture2D("debugColor", m_pFBODebug->GetTextureHandle(), 6);
#endif
}

//! Final Compose
void GLGridLeaper::FinalCompose(){
	GLenum err = glGetError();

	m_pTargetBinder->Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glCullFace(GL_BACK);
	if(!m_pRenderState->m_bClearViewEnabled){
        if (m_pRenderState->m_eCompsoitingMode != D_FINALDEBUGMIX && m_pRenderState->m_eCompsoitingMode != D_FINALDEBUGMIXALPHA){
            m_pProgramCompose->Enable();

            switch (m_pRenderState->m_eCompsoitingMode){
            case D_FINALCOLOR:		{
                                        m_pProgramCompose->SetTexture2D("compose", m_pFBOFinalColor->GetTextureHandle(), 0);
                                        break;
            }
            case D_RESUMECOLOR:		{
                                        m_pProgramCompose->SetTexture2D("compose", m_pFBOStartColor->GetTextureHandle(), 0);
                                        break;
            }
            case D_RESUMEPOSITION:	{
                                        m_pProgramCompose->SetTexture2D("compose", m_pFBORayStart->GetTextureHandle(), 0);
                                        break;
            }
            case D_DEBUGVIEW:		{
                                        m_pProgramCompose->SetTexture2D("compose", m_pFBODebug->GetTextureHandle(), 0);
                                        break;
            }
            default:				{
                                        m_pProgramCompose->SetTexture2D("compose", m_pFBOFinalColor->GetTextureHandle(), 0);
                                        break;
            }
            }
            m_pProgramCompose->Set("ColorOne", m_pRenderState->m_vBackGroundColorTop);
            m_pProgramCompose->Set("ColorTwo", m_pRenderState->m_vBackGroundColorBottom);
        }
        else if (m_pRenderState->m_eCompsoitingMode == D_FINALDEBUGMIX)
        {
            m_pProgramComposeColorDebugMix->Enable();
            m_pProgramComposeColorDebugMix->SetTexture2D("compose", m_pFBOFinalColor->GetTextureHandle(), 0);
            m_pProgramComposeColorDebugMix->SetTexture2D("debug", m_pFBODebug->GetTextureHandle(), 1);
            m_pProgramComposeColorDebugMix->Set("blendfactor", 0.5f);
        }
        else{
            m_pProgramComposeColorDebugMixAlpha->Enable();
            m_pProgramComposeColorDebugMixAlpha->SetTexture2D("compose", m_pFBOFinalColor->GetTextureHandle(), 0);
            m_pProgramComposeColorDebugMixAlpha->SetTexture2D("debug", m_pFBODebug->GetTextureHandle(), 1);

        }
    }else{
        m_pProgramComposeClearViewIso->Enable();
        m_pProgramComposeClearViewIso->SetTexture2D("outerPosition", m_pFBOFinalColorMAIN->GetTextureHandle(), 0);
        m_pProgramComposeClearViewIso->SetTexture2D("innerPosition", m_pFBOFinalColorCV->GetTextureHandle(), 1);
        m_pProgramComposeClearViewIso->SetTexture2D("outerNormal", m_pFBOStartColorMAIN->GetTextureHandle(), 2);
        m_pProgramComposeClearViewIso->SetTexture2D("innerNormal", m_pFBOStartColorCV->GetTextureHandle(), 3);
		m_pProgramComposeClearViewIso->Set("volumePosition", Vec3f(m_vVolumePicking.x, m_vVolumePicking.y, m_vVolumePicking.z));
		m_pProgramComposeClearViewIso->Set("rad", m_pRenderState->m_fClearViewRadius);
        m_pProgramComposeClearViewIso->Set("scale", m_vExtend);
    }
	m_pNearPlane->paint();


	// Debug screenshots
	/*m_pTargetBinder->Bind(m_pFBOFinalColor);
	screenshot(0);
	m_pTargetBinder->Bind(m_pFBOStartColor);
	screenshot(1);
	m_pTargetBinder->Bind(m_pFBORayStart);
	screenshot(2);
	m_pTargetBinder->Bind(m_pFBODebug);
	screenshot(3);*/

	m_pTargetBinder->Unbind();
}

//! Helper Functions ------------------------------------------------
Vec4ui GLGridLeaper::RecomputeBrickVisibility(bool bForceSynchronousUpdate) {
	// (totalProcessedBrickCount, emptyBrickCount, childEmptyBrickCount)
	Vec4ui vEmptyBrickCount(0, 0, 0, 0);
	if (!m_pVolumePool) return vEmptyBrickCount;

	double const fMaxValue = (m_pToCDataset->GetRange().x > m_pToCDataset->GetRange().y) ? m_pTransferFunction->GetSize() : m_pToCDataset->GetRange().y;
	double const fRescaleFactor = fMaxValue / double(m_pTransferFunction->GetSize() - 1);

	// render mode dictates how we look at data ...
	switch (m_pRenderState->m_eRenderMode) {
	case RM_1DTRANS: {
						 double const fMin = double(m_pTransferFunction->GetNonZeroLimits().x) * fRescaleFactor;
						 double const fMax = double(m_pTransferFunction->GetNonZeroLimits().y) * fRescaleFactor;
						 if (m_VisibilityState->NeedsUpdate(fMin, fMax) ||
							 bForceSynchronousUpdate) {
							 vEmptyBrickCount = m_pVolumePool->RecomputeVisibility(*(m_VisibilityState.get()), m_iTimestep, bForceSynchronousUpdate);
						 }
						 break; }
	case RM_2DTRANS: {
						 /* double const fMin = double(m_p2DTrans->GetNonZeroLimits().x) * fRescaleFactor;
						 double const fMax = double(m_p2DTrans->GetNonZeroLimits().y) * fRescaleFactor;
						 double const fMinGradient = double(m_p2DTrans->GetNonZeroLimits().z);
						 double const fMaxGradient = double(m_p2DTrans->GetNonZeroLimits().w);
						 if (m_VisibilityState->NeedsUpdate(fMin, fMax, fMinGradient, fMaxGradient) ||
						 bForceSynchronousUpdate) {
						 vEmptyBrickCount = m_pVolumePool->RecomputeVisibility(*(m_VisibilityState.get()), m_iTimestep, bForceSynchronousUpdate);
						 }*/
						 break; }
	case RM_ISOSURFACE: {
							double const fIsoValue = GetIsoValue();
							if (m_VisibilityState->NeedsUpdate(fIsoValue) ||
								bForceSynchronousUpdate) {
								vEmptyBrickCount = m_pVolumePool->RecomputeVisibility(*(m_VisibilityState.get()), m_iTimestep, bForceSynchronousUpdate);
							}
							break; }
	default:
        LERRORC("GLGridLeaper", "Unhandled rendering mode.");
		break;
	}
	return vEmptyBrickCount;
}

uint32_t GLGridLeaper::UpdateToVolumePool(const Vec4ui& brick) {
	std::vector<UINTVECTOR4> bricks;
	bricks.push_back(brick);
	return UpdateToVolumePool(bricks);
}

uint32_t GLGridLeaper::UpdateToVolumePool(std::vector<Vec4ui>& hash) {
	bool m_bDebugBricks = false; //! \todo find debugbricks variable in glgridleaper ori
	return m_pVolumePool->UploadBricks(hash, m_bDebugBricks);
}

void GLGridLeaper::computeMathMember(){
	m_vScale = Vec3f(m_DatasetCache.m_Scale);
	m_vExtend = Vec3f(m_DatasetCache.m_DomainSize) * m_vScale;
	m_vExtend /= m_vExtend.maxVal();
	m_vScale /= m_vScale.minVal();

	m_mWorldMatrix.Scaling(m_vExtend);
}

//! Set Functions --------------------------------------------------
void GLGridLeaper::SetRenderMode(ERenderMode mode){
	AbstrRenderer::SetRenderMode(mode);
	//! \todo find out what to do
}

void GLGridLeaper::Set1DTransferFunctionptr(GLCore::GLTexture1DPtr texturePtr){
	m_pTransferFunctionTexture = texturePtr;
}

void GLGridLeaper::ClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues){
	m_pVolumeBox->SetSlideLevels(minValues, maxValues);
	m_bCompleteRedraw = true;
}

void GLGridLeaper::Set1DTransferFunction(std::vector<Core::Math::Vec4f> data){
	if (data.size() > 3000.0f){
		m_bTFRescale = true;
	}
	if (m_pTransferFunction == nullptr || m_iTransferFunctionSize != data.size()){
		m_pTransferFunction = std::make_shared<DataIO::TransferFunction1D>(data.size());
		m_pTransferFunction->Set(data);
		vector<Vec4f> data2 = *(m_pTransferFunction->GetColorData().get());

		m_pTransferFunctionTexture = std::make_shared<GLTexture1D>(data2.size(), GL_RGBA32F, GL_RGBA,
			GL_FLOAT, &(data2[0]));
		m_pTransferFunctionTexture->SetData(&(data2[0]));
		m_iTransferFunctionSize = data.size();
	}
	else{
		m_pTransferFunction->Set(data);
		m_pTransferFunctionTexture->SetData(&(data[0]));
	}

	m_bCompleteRedraw = true;
}



void GLGridLeaper::CalculateUsedGPUMemory(){
	m_UsedGPUMemory = 0;
	if (m_pVolumeBox != nullptr){
		m_UsedGPUMemory += m_pVolumeBox->GetGPUSize();
	}
	if (m_pNearPlane != nullptr){
		m_UsedGPUMemory += m_pNearPlane->GetGPUSize();
	}
	if (m_pFBORayStart != nullptr){
		m_UsedGPUMemory += m_pFBORayStart->GetGPUSize();
	}
	if (m_pFBORayStartNext != nullptr){
		m_UsedGPUMemory += m_pFBORayStartNext->GetGPUSize();
	}
	if (m_pFBOStartColor != nullptr){
		m_UsedGPUMemory += m_pFBOStartColor->GetGPUSize();
	}
	if (m_pFBOStartColorNext != nullptr){
		m_UsedGPUMemory += m_pFBOStartColorNext->GetGPUSize();
	}
	if (m_pFBOFinalColor != nullptr){
		m_UsedGPUMemory += m_pFBOFinalColor->GetGPUSize();
	}
	if (m_pFBOFinalColorNext != nullptr){
		m_UsedGPUMemory += m_pFBOFinalColorNext->GetGPUSize();
	}

#ifdef GLGRIDLEAPER_DEBUGVIEW
	if (m_pFBODebug != nullptr){
		m_UsedGPUMemory += m_pFBODebug->GetGPUSize();
	}
	if (m_pFBODebugNext != nullptr){
		m_UsedGPUMemory += m_pFBODebugNext->GetGPUSize();
	}
#endif
	if (m_pProgramRenderFrontFaces != nullptr){
		m_UsedGPUMemory += m_pProgramRenderFrontFaces->GetGPUSize();
	}
	if (m_pProgramRenderFrontFacesNearPlane != nullptr){
		m_UsedGPUMemory += m_pProgramRenderFrontFacesNearPlane->GetGPUSize();
	}
	if (m_pProgramRayCast1D != nullptr){
		m_UsedGPUMemory += m_pProgramRayCast1D->GetGPUSize();
	}
	if (m_pProgramRayCast1DColor != nullptr){
		m_UsedGPUMemory += m_pProgramRayCast1DColor->GetGPUSize();
	}
	if (m_pProgramRayCast1DLighting != nullptr){
		m_UsedGPUMemory += m_pProgramRayCast1DLighting->GetGPUSize();
	}
	if (m_pProgramRayCastISO != nullptr){
		m_UsedGPUMemory += m_pProgramRayCastISO->GetGPUSize();
	}
	if (m_pProgramRayCastISOLighting != nullptr){
		m_UsedGPUMemory += m_pProgramRayCastISOLighting->GetGPUSize();
	}
	if (m_pProgramRayCastISOColor != nullptr){
		m_UsedGPUMemory += m_pProgramRayCastISOColor->GetGPUSize();
	}
	if (m_pCurrentShaderProgram != nullptr){
		m_UsedGPUMemory += m_pCurrentShaderProgram->GetGPUSize();
	}
	if (m_pProgramCompose != nullptr){
		m_UsedGPUMemory += m_pProgramCompose->GetGPUSize();
	}

	if (m_pglHashTable != nullptr){
		m_UsedGPUMemory += m_pglHashTable->GetGPUSize();
	}
	if (m_pTransferFunctionTexture != nullptr){
		m_UsedGPUMemory += m_pTransferFunctionTexture->GetGPUSize();
	}
	if (m_pVolumePool != nullptr){
		m_UsedGPUMemory += m_pVolumePool->GetGPUSize();
	}
}

Core::Math::Vec3ui GLGridLeaper::CalculateVolumePoolSize(uint64_t GPUMemorySizeInByte){
	const uint64_t iBitWidth = m_pToCDataset->GetBitWidth();
	const uint64_t iCompCount = m_pToCDataset->GetComponentCount();
	const Vec3ui vMaxBS = Vec3ui(m_pToCDataset->GetMaxUsedBrickSizes());

	// Compute the pool size as a (almost) cubed texture that fits
	// into the user specified GPU mem, is a multiple of the bricksize
	// and is no bigger than what OpenGL tells us is possible

	//Fake workaround for first :x \todo fix this
	uint64_t GPUmemoryInByte = GPUMemorySizeInByte;

	GLint iMaxVolumeDims;
	glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE_EXT, &iMaxVolumeDims);
	const uint64_t iMaxGPUMem = GPUmemoryInByte - m_UsedGPUMemory;

	const uint64_t iMaxVoxelCount = iMaxGPUMem / (iCompCount*iBitWidth / 8);
	const uint64_t r3Voxels = uint64_t(pow(double(iMaxVoxelCount), 1.0 / 3.0));
	Vec3ui maxBricksForGPU;


	// round the starting input size (r3Voxels) to the closest multiple brick size
	// to guarantee as cubic as possible volume pools and to better fill the
	// available amount of memory
	// (e.g. it creates a 1024x512x1536 pool instead of a 512x2048x512 pool for
	// a brick size of 512^3 and 3.4 GB available memory)
	uint64_t multipleOfBrickSizeThatFitsInX = uint64_t(((float)r3Voxels / vMaxBS.x) + 0.5f) *vMaxBS.x;
	if (multipleOfBrickSizeThatFitsInX > uint64_t(iMaxVolumeDims))
		multipleOfBrickSizeThatFitsInX = (iMaxVolumeDims / vMaxBS.x)*vMaxBS.x;
	maxBricksForGPU.x = (uint32_t)multipleOfBrickSizeThatFitsInX;

	uint64_t multipleOfBrickSizeThatFitsInY = ((iMaxVoxelCount / (maxBricksForGPU.x*maxBricksForGPU.x)) / vMaxBS.y)*vMaxBS.y;
	if (multipleOfBrickSizeThatFitsInY > uint64_t(iMaxVolumeDims))
		multipleOfBrickSizeThatFitsInY = (iMaxVolumeDims / vMaxBS.y)*vMaxBS.y;
	maxBricksForGPU.y = (uint32_t)multipleOfBrickSizeThatFitsInY;

	uint64_t multipleOfBrickSizeThatFitsInZ = ((iMaxVoxelCount / (maxBricksForGPU.x*maxBricksForGPU.y)) / vMaxBS.z)*vMaxBS.z;
	if (multipleOfBrickSizeThatFitsInZ > uint64_t(iMaxVolumeDims))
		multipleOfBrickSizeThatFitsInZ = (iMaxVolumeDims / vMaxBS.z)*vMaxBS.z;
	maxBricksForGPU.z = (uint32_t)multipleOfBrickSizeThatFitsInZ;

	// the max brick layout required by the dataset
	const uint64_t iMaxBrickCount = m_pToCDataset->GetTotalBrickCount();
	const uint64_t r3Bricks = uint64_t(pow(double(iMaxBrickCount), 1.0 / 3.0));
	Vec3ui64 maxBricksForDataset;

	multipleOfBrickSizeThatFitsInX = vMaxBS.x*r3Bricks;
	if (multipleOfBrickSizeThatFitsInX > uint64_t(iMaxVolumeDims))
		multipleOfBrickSizeThatFitsInX = (iMaxVolumeDims / vMaxBS.x)*vMaxBS.x;
	maxBricksForDataset.x = (uint32_t)multipleOfBrickSizeThatFitsInX;

	multipleOfBrickSizeThatFitsInY = vMaxBS.y*uint64_t(ceil(float(iMaxBrickCount) / ((maxBricksForDataset.x / vMaxBS.x) * (maxBricksForDataset.x / vMaxBS.x))));
	if (multipleOfBrickSizeThatFitsInY > uint64_t(iMaxVolumeDims))
		multipleOfBrickSizeThatFitsInY = (iMaxVolumeDims / vMaxBS.y)*vMaxBS.y;
	maxBricksForDataset.y = (uint32_t)multipleOfBrickSizeThatFitsInY;

	multipleOfBrickSizeThatFitsInZ = vMaxBS.z*uint64_t(ceil(float(iMaxBrickCount) / ((maxBricksForDataset.x / vMaxBS.x) * (maxBricksForDataset.y / vMaxBS.y))));
	if (multipleOfBrickSizeThatFitsInZ > uint64_t(iMaxVolumeDims))
		multipleOfBrickSizeThatFitsInZ = (iMaxVolumeDims / vMaxBS.z)*vMaxBS.z;
	maxBricksForDataset.z = (uint32_t)multipleOfBrickSizeThatFitsInZ;

	// now use the smaller of the two layouts, normally that
	// would be the maxBricksForGPU but for small datasets that
	// can be rendered entirely in-core we may need less space
	const Vec3ui poolSize = (maxBricksForDataset.volume() < Vec3ui64(maxBricksForGPU).volume())
		? Vec3ui(maxBricksForDataset)
		: maxBricksForGPU;

	return poolSize;
}

void GLGridLeaper::ReadFrameBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount){
	if (m_lastFrame < m_nextFrame){
		pixels = m_pixels;
		width = m_width;
		height = m_height;
		componentCount = m_componentCount;

		m_lastFrame = m_nextFrame;
	}
	if (!m_bCreateFrameBuffer){
		m_bCreateFrameBuffer = true;
	}
}

void GLGridLeaper::readFB(){
	m_pTargetBinder->Unbind();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	m_width = viewport[2];
	m_height = viewport[3];
	m_componentCount = 3;

	m_pixels.resize(viewport[2] * viewport[3] * 3);
	//glReadBuffer(GL_FRONT);
	glReadPixels(0, 0, viewport[2], viewport[3], GL_RGB,
		GL_UNSIGNED_BYTE, &m_pixels[0]);

}


Core::Math::Vec4f GLGridLeaper::readVolumePosition(Core::Math::Vec2ui v){
    Vec4ui8 data(255,255,255,255);

    //bind targets
    #ifdef GLGRIDLEAPER_DEBUGVIEW
	m_pTargetBinder->Bind(m_pFBOFinalColorNext,
		m_pFBOStartColorNext,
		m_pFBORayStartNext,
		m_pFBODebugNext);
    #else
	m_pTargetBinder->Bind(m_pFBOFinalColorMAIN,
		m_pFBOStartColorMAIN,
		m_pFBORayStartMAIN);
    #endif


    glReadBuffer((GLenum)GL_COLOR_ATTACHMENT0);
    glReadPixels(v.x, GetSize().y - v.y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid*)&data);

    if(data.x != 0 && data.y != 0 && data.z != 0)
        m_vVolumePicking = Vec4f((float)data.x/255.0f,(float)data.y/255.0f,(float)data.z/255.0f,(float)data.w/255.0f);


    m_pTargetBinder->Unbind();
    return m_vVolumePicking;
}


void GLGridLeaper::run(){
    while(true){
        m_pContext->lockContext();
        Paint();
        m_pContext->frameFinished();
        m_pContext->unlockContext();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}
