#include "VisibilityState.h"

VisibilityState::VisibilityState() :
m_eRenderMode(trinity::IRenderer::ERenderMode::RM_INVALID)
{}

bool VisibilityState::needsUpdate(double fMin, double fMax)
{
  bool const bNeedsUpdate = (m_eRenderMode != trinity::IRenderer::ERenderMode::RM_1DTRANS) ||
    (m_rm1DTrans.fMin != fMin) ||
    (m_rm1DTrans.fMax != fMax);
  m_eRenderMode = trinity::IRenderer::ERenderMode::RM_1DTRANS;
  m_rm1DTrans.fMin = fMin;
  m_rm1DTrans.fMax = fMax;
  return bNeedsUpdate;
}

bool VisibilityState::needsUpdate(double fMin, double fMax, double fMinGradient,
                                  double fMaxGradient)
{
  bool const bNeedsUpdate = (m_eRenderMode != trinity::IRenderer::ERenderMode::RM_2DTRANS) ||
    (m_rm2DTrans.fMin != fMin) ||
    (m_rm2DTrans.fMax != fMax) ||
    (m_rm2DTrans.fMinGradient != fMinGradient) ||
    (m_rm2DTrans.fMaxGradient != fMaxGradient);
  m_eRenderMode = trinity::IRenderer::ERenderMode::RM_2DTRANS;
  m_rm2DTrans.fMin = fMin;
  m_rm2DTrans.fMax = fMax;
  m_rm2DTrans.fMinGradient = fMinGradient;
  m_rm2DTrans.fMaxGradient = fMaxGradient;
  return bNeedsUpdate;
}

bool VisibilityState::needsUpdate(double fIsoValue)
{
  bool const bNeedsUpdate = (m_eRenderMode != trinity::IRenderer::ERenderMode::RM_ISOSURFACE) ||
    (m_rmIsoSurf.fIsoValue != fIsoValue);
  m_eRenderMode = trinity::IRenderer::ERenderMode::RM_ISOSURFACE;
  m_rmIsoSurf.fIsoValue = fIsoValue;
  return bNeedsUpdate;
}

bool VisibilityState::needsUpdateCV(double fIsoValue1, double fIsoValue2)
{
  bool const bNeedsUpdate = (m_eRenderMode != trinity::IRenderer::ERenderMode::RM_CLEARVIEW )||
                            (m_rmIsoSurfCV.fIsoValue1 != fIsoValue1) ||
                            (m_rmIsoSurfCV.fIsoValue2 != fIsoValue2);
  
  m_eRenderMode = trinity::IRenderer::ERenderMode::RM_CLEARVIEW;
  m_rmIsoSurfCV.fIsoValue1 = fIsoValue1;
  m_rmIsoSurfCV.fIsoValue2 = fIsoValue2;
  return bNeedsUpdate;
}

