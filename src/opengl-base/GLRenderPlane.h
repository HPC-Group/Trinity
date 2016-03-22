#pragma once
#include "GLModel.h"

        
class GLRenderPlane: public GLModel{
    public:
      GLRenderPlane(const Core::Math::Vec2ui screenResolution);
      virtual ~GLRenderPlane();

      const Core::Math::Mat4f getPlaneView() const;
      const Core::Math::Mat4f getPlaneProjection() const;
      const Core::Math::Mat4f getPlaneWorld() const;

    private:
      Core::Math::Mat4f m_mView;
      Core::Math::Mat4f m_mProjection;
      Core::Math::Mat4f m_mWorld;
};
