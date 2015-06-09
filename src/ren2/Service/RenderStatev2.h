#ifndef __RenderSateV2__
#define __RenderSateV2__

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <Core/Math/Vectors.h>
#include <Renderer/RenderEnums.h>
#include <Renderer/Camera.h>

namespace Tuvok{
    namespace Renderer{


        class State{
        public:
        State():
         m_resolution(640,480),
         m_eRenderSystem(System::Linux),
         m_eWindowVisibility(Visibility::Windowed),
         m_bHasToResize(false),
         m_iUseableVRAM(500*1024),
         m_sDataSet("none"),
         m_sTransferFunction("none"),
         m_pCameraPtr(),
         m_vCameraPostion(0,0,-2),
         m_vCameraRotation(0,0,0),
         m_fCameraZoom(2.0f),
         m_bFirstPersonCamera(false),
         m_fFOV(45.0f),
         m_fZNear(0.1f),
         m_fZFar(100.0f),
         m_eRenderMode(ERenderMode::RM_1DTRANS),
         m_eCompsoitingMode(ECompositeDisplay::D_FINALCOLOR),
         m_eMissingBrickStrategy(MissingBrickStrategy::SkipOneLevel),
         m_mProjection(),
         m_mView(),
         m_bClearFrameBuffer(true),
         m_vBackGroundColorTop(0,0,1),
         m_vBackGroundColorBottom(0,1,1),
         m_fSampleRateModifier(1),
         m_bFirstDrawAfterResize(true),
         m_bIsColor(true),
         m_fLODFactor(0.0f),
         m_bUseLighting(true),
         m_cAmbient(0.1f, 0.1f, 0.1f, 1.0f),
         m_cDiffuse(0.8f, 0.8f, 0.8f, 1.0f),
		 m_cSpecular(1.0f, 1.0f, 1.0f, 1.0f),
         m_fIsoValue(0.01f),
         m_vIsoColor(1,1,1),
         m_bClearViewEnabled(false),
         m_fClearViewIsoValue(0.2),
         m_bClearViewIso(true),
         m_fClearViewRadius(0.05f),
         m_v1DTransferFunctionData(),
         m_bNeedUpdate1DTF(false)
        {
        };

        State(const State &s)
         {
         };

         State& operator=(const State& rhs) {};

            //basics
            Core::Math::Vec2ui      m_resolution;
            System                  m_eRenderSystem;
            Visibility              m_eWindowVisibility;
            bool                    m_bHasToResize;
            uint64_t                m_iUseableVRAM;

            //Dataset
            std::string             m_sDataSet;
            std::string             m_sTransferFunction;

            //Camera
            Camera                  m_pCameraPtr;
            Core::Math::Vec3f       m_vCameraPostion;
            Core::Math::Vec3f       m_vCameraRotation;
            float                   m_fCameraZoom;
            bool                    m_bFirstPersonCamera;
            float                   m_fFOV;
            float                   m_fZNear;
            float                   m_fZFar;

            //Rendering
            ERenderMode             m_eRenderMode;
            ECompositeDisplay       m_eCompsoitingMode;
            MissingBrickStrategy    m_eMissingBrickStrategy;

            //globalRendering
            Core::Math::Mat4f       m_mProjection;
            Core::Math::Mat4f       m_mView;
            bool                    m_bClearFrameBuffer;
            Core::Math::Vec3f		m_vBackGroundColorTop;
            Core::Math::Vec3f		m_vBackGroundColorBottom;
            float                   m_fSampleRateModifier;
            bool                    m_bFirstDrawAfterResize;
            bool                    m_bIsColor;
            float                   m_fLODFactor;

            //lighting
            bool                    m_bUseLighting;
            Core::Math::Vec4f		m_cAmbient;
            Core::Math::Vec4f		m_cDiffuse;
            Core::Math::Vec4f		m_cSpecular;
            Core::Math::Vec3f		m_vLightDir;

            //isoRendering
            float                   m_fIsoValue; //outer iso value for clearview
            Core::Math::Vec3f       m_vIsoColor;

            //clearview-iso
            bool                    m_bClearViewEnabled;
            float                   m_fClearViewIsoValue;   //inner iso value for clearview
            bool                    m_bClearViewIso;
            float                   m_fClearViewRadius;

            //1DTF
            std::vector<Core::Math::Vec4f>		m_v1DTransferFunctionData;
            bool								m_bNeedUpdate1DTF;

            //axisbasedClipping
        private:
        };
    };
};

#endif
