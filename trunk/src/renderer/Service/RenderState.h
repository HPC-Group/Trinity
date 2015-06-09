//
//  IOService.h
//  DemoRendering
//
//  Created by Andre Waschk on 02/04/15.
//  Copyright (c) 2014 CoViDaG. All rights reserved.
//

#ifndef __RenderSate__
#define __RenderSate__

#pragma once

#include <string>
#include <memory>
#include <vector>
#include <core/Math/Vectors.h>
#include <renderer/RenderEnums.h>

namespace Tuvok {
	namespace Renderer{
		class AbstrRenderer;
	}
	class RenderState{
	public:
		RenderState() :
			m_vResolution(640, 480),
			m_bHasToResize(false),
			m_eRendermode(Renderer::ERenderMode::RM_ISOSURFACE),
			//m_sDataSetName("WholeBody.uvf"),
			m_sDataSetName("walnut.uvf"),
			m_sTFName("none"),
			//m_uiUseableKilobyte(3072000),
			m_uiUseableKilobyte(524288),
			m_bUseLighting(true),
			m_eMissingBrickStrategy(Tuvok::Renderer::MissingBrickStrategy::SkipOneLevel),
			m_bHideRemoteDisplay(true),
			m_fIsoValue(0.0f),
			m_vCameraPosition(0, 0, 2),
			m_vCameraRotation(0, 0, 0),
			m_fCameraZoom(2.0f),
			m_bFirstPerson(false),
			m_dRefreshRateInHz(60),
			m_vAmbientColor(0.1f, 0.1f, 0.1f, 1.0f),
			m_vDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f),
			m_vSpecularColor(1.0f, 1.0f, 1.0f, 1.0f),
			m_bClearFrameBuffer(true),
			m_vBackgroundTopColor(0, 0, 1),
			m_vBackgroundBottomColor(0, 1, 1),
			m_bNeedUpdate1DTF(false),
			m_v1DTransferFunctionData()
		{};

		Core::Math::Vec2ui GetResolution() const { return m_vResolution; }
		bool GetHasToResize() const { return m_bHasToResize; }
		Renderer::ERenderMode GetRenderMode() const { return m_eRendermode; }
		std::string GetDataSetName() const { return m_sDataSetName; }
		std::string GetTFName() const { return m_sTFName; }
		uint64_t GetUseableKilobyte() const { return m_uiUseableKilobyte; }
		bool GetUseLighting() const { return m_bUseLighting; }
		Tuvok::Renderer::MissingBrickStrategy GetPagingStrategy() const { return m_eMissingBrickStrategy; }
		bool GetHideRemoteDisplay() const { return m_bHideRemoteDisplay; }
		float GetIsoValue() const { return m_fIsoValue; }
		Core::Math::Vec3f GetCameraPosition() const { return m_vCameraPosition; }
		Core::Math::Vec3f GetCameraRotation() const { return m_vCameraRotation; }
		float GetCameraZoom() const { return m_fCameraZoom; }
		bool GetFirstPerson() const { return m_bFirstPerson; }
		bool GetClearFrameBuffer() const { return m_bClearFrameBuffer; }
		Core::Math::Vec3f GetBackgroundTopColor() const { return m_vBackgroundTopColor; }
		Core::Math::Vec3f GetBackgroundBottomColor() const { return m_vBackgroundBottomColor; }
		std::vector<Core::Math::Vec4f> Get1DTransferFunctionData() const { return m_v1DTransferFunctionData; }
		bool Get1DTFNeedsUpdate() const { return m_bNeedUpdate1DTF; }

		Core::Math::Vec4f GetAmbientColor() const { return m_vAmbientColor; }
		Core::Math::Vec4f GetDiffuseColor() const { return m_vDiffuseColor; }
		Core::Math::Vec4f GetSpecularColor() const { return m_vSpecularColor; }

		double GetRefreshRate() const { return m_dRefreshRateInHz; }

		void SetResolution(Core::Math::Vec2ui v){ m_vResolution = v; }
		void SetHasToResize(bool b){ m_bHasToResize = b; }
		void SetRenderMode(Renderer::ERenderMode e){ m_eRendermode = e; }
		void SetDataSetName(std::string n){ m_sDataSetName = n; }
		void SetTFName(std::string n){ m_sTFName = n; }
		void SetUseableKiloByte(uint64_t b){ m_uiUseableKilobyte = b; }
		void SetUseLighting(bool b){ m_bUseLighting = b; }
		void SetPagingStrategy(Tuvok::Renderer::MissingBrickStrategy e) { m_eMissingBrickStrategy = e; }
		void SetHideRemoteDisplay(bool b) { m_bHideRemoteDisplay = b; }
		void SetIsoValue(float f){ m_fIsoValue = f; }
		void SetCameraPostion(Core::Math::Vec3f v){ m_vCameraPosition = v; }
		void SetCameraRotation(Core::Math::Vec3f v){ m_vCameraRotation = v; }
		void SetCameraZoom(float f){ m_fCameraZoom = f; }
		void SetFirstPerson(bool b){ m_bFirstPerson = b; }
		void SetRefreshRateInHz(double d){ m_dRefreshRateInHz = d; }
		void SetClearFrameBuffer(double b){ m_bClearFrameBuffer = b; }
		void SetBackgroundTopColor(Core::Math::Vec3f color) { m_vBackgroundTopColor = color; }
		void SetBackgroundBottomColor(Core::Math::Vec3f color) { m_vBackgroundBottomColor = color; }

		void SetAmbientColor(Core::Math::Vec4f color) { m_vAmbientColor = color; }
		void SetDiffuseColor(Core::Math::Vec4f color) { m_vDiffuseColor = color; }
		void SetSpecularColor(Core::Math::Vec4f color) { m_vSpecularColor = color; }
		void Set1DTransferFunction(std::vector<Core::Math::Vec4f> data){ m_v1DTransferFunctionData = data; m_bNeedUpdate1DTF = true; }
		void Set1DTFNeedsUpdate(bool b) { m_bNeedUpdate1DTF = b; }
		void Set1DTransferFunctionSinglePoint(uint64_t index, Core::Math::Vec4f color){ m_v1DTransferFunctionData[index] = color; m_bNeedUpdate1DTF = true; }

	private:
		Core::Math::Vec2ui								m_vResolution;
		bool											m_bHasToResize;
		Renderer::ERenderMode							m_eRendermode;
		std::string										m_sDataSetName;
		std::string										m_sTFName;
		uint64_t										m_uiUseableKilobyte;
		bool											m_bUseLighting;
		Tuvok::Renderer::MissingBrickStrategy			m_eMissingBrickStrategy;
		bool											m_bHideRemoteDisplay;
		float										    m_fIsoValue;
		Core::Math::Vec3f								m_vCameraPosition;
		Core::Math::Vec3f								m_vCameraRotation;
		float											m_fCameraZoom;
		bool											m_bFirstPerson;
		double											m_dRefreshRateInHz;
		bool											m_bClearFrameBuffer;

		Core::Math::Vec4f								m_vAmbientColor;
		Core::Math::Vec4f								m_vDiffuseColor;
		Core::Math::Vec4f								m_vSpecularColor;

		Core::Math::Vec3f								m_vBackgroundTopColor;
		Core::Math::Vec3f								m_vBackgroundBottomColor;

		std::vector<Core::Math::Vec4f>						m_v1DTransferFunctionData;
		bool											m_bNeedUpdate1DTF;
	};

};

#endif
