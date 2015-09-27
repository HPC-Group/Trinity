#ifndef __TNG__GLGridleaper2__
#define __TNG__GLGridleaper2__

#include <renderer/AbstrRenderer.h>
#include "OpenGLDefines.h"

#include <renderer/Context/Context.h>

#include <thread>

#define GLGRIDLEAPER_DEBUGVIEW



namespace DataIO{
	class TransferFunction1D;
	typedef std::shared_ptr<TransferFunction1D> TransferFunction1DPtr;
}

namespace Tuvok{

	namespace Renderer{
		class ShaderDescriptor;

		class VisibilityState;
		typedef std::shared_ptr<VisibilityState> VisibilityStatePtr;

		namespace OpenGL{

			class GLHashTable;
			typedef std::shared_ptr<GLHashTable> GLHashTablePtr;
			class GLVolumePool;
			typedef std::shared_ptr<GLVolumePool> GLVolumePoolPtr;
			class GLTargetBinder;
			typedef std::shared_ptr<GLTargetBinder> GLTargetBinderPtr;


			// forward declarations of used classes
			namespace GLCore{
				class GLProgram;
				typedef std::shared_ptr<GLProgram> GLProgramPtr;
				class GLFBOTex;
				typedef std::shared_ptr<GLFBOTex> GLFBOTexPtr;
				class GLVolumeBox;
				typedef std::shared_ptr<GLVolumeBox> GLVolumeBoxPtr;
				class GLRenderPlane;
				typedef std::shared_ptr<GLRenderPlane> GLRenderPlanePtr;
				class GLTexture1D;
				typedef std::shared_ptr<GLTexture1D> GLTexture1DPtr;
				class GLBoundingBox;
				typedef std::shared_ptr<GLBoundingBox> GLBoundingBoxPtr;
			}

			class GLGridLeaper : public AbstrRenderer{
			public:
				struct GridLeaperDataSetCache{
					GridLeaperDataSetCache() :m_DomainSize(0, 0, 0), m_Scale(0, 0, 0){};

					Core::Math::Vec3ui64  m_DomainSize;
					Core::Math::Vec3d	  m_Scale;
				};

				GLGridLeaper(std::shared_ptr<Context::Context> context,Core::Math::Vec2ui vWinSize = Core::Math::Vec2ui(640, 480), ERenderMode mode = RM_1DTRANS);
				~GLGridLeaper();

				//! initializes the renderer \todo more parameters like resolution etc
				bool Initialize(uint64_t gpuMemorySizeInByte);

				//! cleanup
				void Cleanup();

				//! main paint function for a frame
				bool Paint();

				//! setting the viewport \note add renderregion
				void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
					bool decrease_screen_res = false);

				bool RecreateAfterResize();

				void SetRenderMode(ERenderMode mode);

				void Set1DTransferFunctionptr(GLCore::GLTexture1DPtr texturePtr);
				void Set1DTransferFunction(std::vector<Core::Math::Vec4f> data);

				void SwitchPagingStrategy(MissingBrickStrategy brickStrategy);

				void ClipVolume(Core::Math::Vec3f minValues, Core::Math::Vec3f maxValues);

				void ReadFrameBuffer(std::vector<uint8_t>& pixels, int& width, int& height, int& componentCount);
				FrameData& ReadFrameBuffer();

				Core::Math::Vec4f readVolumePosition(Core::Math::Vec2ui v);

			private:
				bool InitOffScreenBuffers();
				void InitVolumePool(uint64_t gpuMemorySizeInByte);
				void InitHashTable();
				void InitGeometry();

				void CleanupShader();
				void CleanupTraversalShader();
				void CleanupFrameBuffer();
				void CleanupGeometry();

				bool LoadTraversalShader(MissingBrickStrategy brickStrategy);
				bool LoadFrontFaceShader();
				bool LoadComposeShader();

				bool LoadCheckShader(GLCore::GLProgramPtr& programptr, ShaderDescriptor& sd, std::string name);
				Core::Math::Vec4ui RecomputeBrickVisibility(bool bForceSynchronousUpdate);

				void recreateFBO();
				void computeMathMember();

				void FillRayEntryBuffer();
				void Raycast(Tuvok::Renderer::ERenderMode rendermode);
				void FinalCompose();
				void ClearView();

				void SetupRaycastShader(Tuvok::Renderer::ERenderMode rendermode);
				void SelectShader(Tuvok::Renderer::ERenderMode rendermode);

				void CalculateUsedGPUMemory();
				Core::Math::Vec3ui CalculateVolumePoolSize(uint64_t GPUMemorySizeInByte);

				uint32_t UpdateToVolumePool(const Core::Math::Vec4ui& brick);
				uint32_t UpdateToVolumePool(std::vector<Core::Math::Vec4ui>& hash);

				void swapBuffers(bool clearview);
				void swapToNextBuffer(bool clearview);

				GridLeaperDataSetCache			m_DatasetCache;

				GLCore::GLVolumeBoxPtr			m_pVolumeBox;
				GLCore::GLRenderPlanePtr		m_pNearPlane;

//actual used framebuffers
				GLCore::GLFBOTexPtr				m_pFBORayStart;
				GLCore::GLFBOTexPtr				m_pFBORayStartNext;
				GLCore::GLFBOTexPtr				m_pFBOStartColor;
				GLCore::GLFBOTexPtr				m_pFBOStartColorNext;
				GLCore::GLFBOTexPtr				m_pFBOFinalColor;
				GLCore::GLFBOTexPtr				m_pFBOFinalColorNext;
#ifdef GLGRIDLEAPER_DEBUGVIEW
				GLCore::GLFBOTexPtr				m_pFBODebug;
				GLCore::GLFBOTexPtr				m_pFBODebugNext;
#endif
                Core::Math::Vec4f               m_vVolumePicking;

//---------------CLEARVIEW START
				GLCore::GLFBOTexPtr				m_pFBORayStartMAIN;
				GLCore::GLFBOTexPtr				m_pFBORayStartNextMAIN;
				GLCore::GLFBOTexPtr				m_pFBOStartColorMAIN;
				GLCore::GLFBOTexPtr				m_pFBOStartColorNextMAIN;
				GLCore::GLFBOTexPtr				m_pFBOFinalColorMAIN;
				GLCore::GLFBOTexPtr				m_pFBOFinalColorNextMAIN;
#ifdef GLGRIDLEAPER_DEBUGVIEW
				GLCore::GLFBOTexPtr				m_pFBODebugMAIN;
				GLCore::GLFBOTexPtr				m_pFBODebugNextMAIN;
#endif

                GLCore::GLFBOTexPtr				m_pFBORayStartCV;
				GLCore::GLFBOTexPtr				m_pFBORayStartNextCV;
				GLCore::GLFBOTexPtr				m_pFBOStartColorCV;
				GLCore::GLFBOTexPtr				m_pFBOStartColorNextCV;
				GLCore::GLFBOTexPtr				m_pFBOFinalColorCV;
				GLCore::GLFBOTexPtr				m_pFBOFinalColorNextCV;
#ifdef GLGRIDLEAPER_DEBUGVIEW
				GLCore::GLFBOTexPtr				m_pFBODebugCV;
				GLCore::GLFBOTexPtr				m_pFBODebugNextCV;
#endif

                GLCore::GLProgramPtr            m_pProgramComposeClearViewIso;

				float							m_fLODFactor;

//---------------CLEARVIEW END

				GLCore::GLProgramPtr			m_pProgramRenderFrontFaces;
				GLCore::GLProgramPtr			m_pProgramRenderFrontFacesNearPlane;

				GLCore::GLProgramPtr			m_pProgramRayCast1D;
				GLCore::GLProgramPtr			m_pProgramRayCast1DColor;
				GLCore::GLProgramPtr			m_pProgramRayCast1DLighting;
				GLCore::GLProgramPtr			m_pProgramRayCastISO;
				GLCore::GLProgramPtr			m_pProgramRayCastISOLighting;
				GLCore::GLProgramPtr			m_pProgramRayCastISOColor;

				GLCore::GLProgramPtr			m_pCurrentShaderProgram;

				GLCore::GLProgramPtr			m_pProgramCompose;
				GLCore::GLProgramPtr			m_pProgramComposeColorDebugMix;
				GLCore::GLProgramPtr			m_pProgramComposeColorDebugMixAlpha;



				GLVolumePoolPtr					m_pVolumePool;
				GLHashTablePtr					m_pglHashTable;

				GLTargetBinderPtr				m_pTargetBinder;


				DataIO::TransferFunction1DPtr	m_pTransferFunction;
				GLCore::GLTexture1DPtr			m_pTransferFunctionTexture;
				uint32_t						m_iTransferFunctionSize;

				VisibilityStatePtr				m_VisibilityState;
				uint32_t						m_iTimestep;

				//Math member
				Core::Math::Mat4f				m_mWorldMatrix;
				Core::Math::Vec3f				m_vScale;
				Core::Math::Vec3f				m_vExtend;

				uint64_t						m_UsedGPUMemory;

				//framebuffer qt hack
				void readFB();
				bool							m_bCreateFrameBuffer;
				int								m_lastFrame;
				int								m_nextFrame;
				std::vector<uint8_t>			m_pixels;
				int								m_width;
				int								m_height;
				int								m_componentCount;

				//cebit hack
				bool							m_bTFRescale;

                void run() override;

			};
		}
	}
}
#endif
