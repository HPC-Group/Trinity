//
//  GLDemoRayCaster.h
//  TNG
//
//  Created by Andre Waschk on 23/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//



#ifndef __TNG__GLDemoRayCaster__
#define __TNG__GLDemoRayCaster__

#include <renderer/AbstrRenderer.h>
#include "OpenGLDefines.h"

namespace DataIO{
  class TransferFunction1D;
  typedef std::shared_ptr<TransferFunction1D> TransferFunction1DPtr;
}

namespace Tuvok{
	class IDataIO;
	typedef std::shared_ptr<IDataIO> DataIOPtr;

  namespace Renderer{
    namespace OpenGL{

		class GLHashTable;
		typedef std::shared_ptr<GLHashTable> GLHashTablePtr;

      // forward declarations of used classes
      namespace GLCore{
        class GLProgram;
        typedef std::shared_ptr<GLProgram> GLProgramPtr;
        class GLFBOTex;
        typedef std::shared_ptr<GLFBOTex> GLFBOTexPtr;
        class GLFrameBuffer;
        typedef std::shared_ptr<GLFrameBuffer> GLFrameBufferPtr;
        class GLVolumeBox;
        typedef std::shared_ptr<GLVolumeBox> GLVolumeBoxPtr;
        class GLRenderPlane;
        typedef std::shared_ptr<GLRenderPlane> GLRenderPlanePtr;
        class GLTexture1D;
        typedef std::shared_ptr<GLTexture1D> GLTexture1DPtr;
        class GLBoundingBox;
        typedef std::shared_ptr<GLBoundingBox> GLBoundingBoxPtr;
      }

      namespace GLVolume{
        class GLVolume3DTex;
        typedef std::shared_ptr<GLVolume3DTex> GLVolume3DTexPtr;
      }

      /*! \brief GLDemoRayCaster is used to test simple Tuvok features
       *
       *  This Class is used to test basic features of Tuvok before integrating them into more
       *  compilcated renderers.
       */
      class GLDemoRayCaster: public AbstrRenderer{
        public:

          GLDemoRayCaster();
          ~GLDemoRayCaster();

          //! initializes the renderer \todo more parameters like resolution etc
          virtual bool Initialize();

          //! cleanup
          virtual void Cleanup();

          //! main paint function for a frame
          virtual bool Paint();

          //! function to set a pointer to a dataio object \todo should later communicate over the network
          virtual void SetDataset(DataIOPtr dio);

          //! setting the viewport \note add renderregion
          virtual void SetViewPort(Core::Math::Vec2ui lower_left, Core::Math::Vec2ui upper_right,
                                 bool decrease_screen_res = false);

        private:
          //! first step of the raycaster is to render the frontfaces of a cube which contains the complete domain
			void paintCubeFrontBuffer(GLCore::GLFrameBufferPtr buffer, GLenum cullmode);
          //! second step is to use a gpu based raycaster to render the volume
          void raycast();
          //! function to paint a global surrounding bounding box \todo but into a glrenderer, can be reused
          void paintBoundingBox(Core::Math::Vec3f vCenter, Core::Math::Vec3f vExtend);

		  GLCore::GLProgramPtr LoadShader(const std::string VertexShader,
			  const std::string FragmentShader);
		  GLCore::GLProgramPtr LoadShader(const std::vector<std::string> VertexShader,
			  const std::vector<std::string> FragmentShader);

          //! FrameBuffer to store the frontface of the volume
          GLCore::GLFrameBufferPtr      m_pFrontFace;

          //! shader search path \todo move to abstrrenderer
          std::vector<std::string>      m_vShaderSearchPath;


          //! This geometry is a box arround the acutal volume
          GLCore::GLVolumeBoxPtr        m_pVolumeBox;
          //! A NearPlane is used to calculate the position inside the volume (camera inside the volume)
          GLCore::GLRenderPlanePtr      m_pNearPlane;
          //! Axis-Aligned Bounding Box of the Volume
          GLCore::GLBoundingBoxPtr      m_pBB;

          //! Programs used
          //! FirstPass renders the m_pVolumeBox into m_pFrontFace
          GLCore::GLProgramPtr          m_pGLProgramFirstpass;
          //! NearPlane renders the m_pNearPlane into m_pFrontFace
          GLCore::GLProgramPtr          m_pGLProgramNearPlane;
          //! Actual RayCast Program
          GLCore::GLProgramPtr          m_pGLProgramBackFaceRayCasting;

          //! Program used to render a bounding box
          GLCore::GLProgramPtr          m_pGLProgramBoundingBox;

          //! this 3D Texture contains the Data of the Volume
          GLVolume::GLVolume3DTexPtr    m_VolumeData;

          //! TransferFunction
          DataIO::TransferFunction1DPtr m_TransferFunction;
          GLCore::GLTexture1DPtr        m_TFTexture;


		  GLHashTablePtr				m_pHashTable;

          //dummy matrix for testpurpose
          Core::Math::Mat4f m_world,m_rot,m_scale;
          float m_r;
          float m_minTF;
      };
    }
  }
}

#endif /* defined(__TNG__GLDemoRayCaster__) */
