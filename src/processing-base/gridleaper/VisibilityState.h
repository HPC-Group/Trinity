#pragma once

#include <common/IRenderer.h>

/** \class VisibilityState
 * Caches last given visibility state.
 *
 * Simply stores last given render mode along with transfer function
 * (min, max, ...)
 * parameters and tells you whether visibility needs to be recomputed or not. */
class VisibilityState
{
public:
  struct RM1DTransfer {
    double fMin;
    double fMax;
  };
  struct RM2DTransfer {
    double fMin;
    double fMax;
    double fMinGradient;
    double fMaxGradient;
  };
  struct RMIsoSurface {
    double fIsoValue;
  };
  struct RMIsoSurfaceCV {
    double fIsoValue1;
    double fIsoValue2;
  };
  
  VisibilityState();
  
  bool needsUpdate(double fMin,
                   double fMax);
  bool needsUpdate(double fMin,
                   double fMax,
                   double fMinGradient,
                   double fMaxGradient);
  bool needsUpdate(double fIsoValue);
  bool needsUpdateCV(double fIsoValue1,
                   double fIsoValue2);
  
  trinity::IRenderer::ERenderMode getRenderMode() const { return m_eRenderMode; }
  RM1DTransfer const& get1DTransfer() const { return m_rm1DTrans; }
  RM2DTransfer const& get2DTransfer() const { return m_rm2DTrans; }
  RMIsoSurface const& getIsoSurface() const { return m_rmIsoSurf; }
  RMIsoSurfaceCV const& getIsoSurfaceCV() const { return m_rmIsoSurfCV; }
  
private:
  trinity::IRenderer::ERenderMode m_eRenderMode;
  union {
    RM1DTransfer m_rm1DTrans;
    RM2DTransfer m_rm2DTrans;
    RMIsoSurface m_rmIsoSurf;
    RMIsoSurfaceCV m_rmIsoSurfCV;
  };
};

