#include "VisibilityState.h"

using namespace Tuvok::Renderer;

VisibilityState::VisibilityState() : m_eRenderMode(RM_INVALID)
{}

bool VisibilityState::NeedsUpdate(double fMin, double fMax)
{
  bool const bNeedsUpdate = (m_eRenderMode != RM_1DTRANS) ||
    (m_rm1DTrans.fMin != fMin) ||
    (m_rm1DTrans.fMax != fMax);
  m_eRenderMode = RM_1DTRANS;
  m_rm1DTrans.fMin = fMin;
  m_rm1DTrans.fMax = fMax;
  return bNeedsUpdate;
}

bool VisibilityState::NeedsUpdate(double fMin, double fMax, double fMinGradient, double fMaxGradient)
{
  bool const bNeedsUpdate = (m_eRenderMode != RM_2DTRANS) ||
    (m_rm2DTrans.fMin != fMin) ||
    (m_rm2DTrans.fMax != fMax) ||
    (m_rm2DTrans.fMinGradient != fMinGradient) ||
    (m_rm2DTrans.fMaxGradient != fMaxGradient);
  m_eRenderMode = RM_2DTRANS;
  m_rm2DTrans.fMin = fMin;
  m_rm2DTrans.fMax = fMax;
  m_rm2DTrans.fMinGradient = fMinGradient;
  m_rm2DTrans.fMaxGradient = fMaxGradient;
  return bNeedsUpdate;
}

bool VisibilityState::NeedsUpdate(double fIsoValue)
{
  bool const bNeedsUpdate = (m_eRenderMode != RM_ISOSURFACE) ||
    (m_rmIsoSurf.fIsoValue != fIsoValue);
  m_eRenderMode = RM_ISOSURFACE;
  m_rmIsoSurf.fIsoValue = fIsoValue;
  return bNeedsUpdate;
}
