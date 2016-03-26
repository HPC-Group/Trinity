#pragma once
#include "GLModel.h"

        
class GLRenderPlane: public GLModel{
    public:
      GLRenderPlane(const Core::Math::Vec2ui screenResolution);
      GLRenderPlane();
      virtual ~GLRenderPlane();
    private:
};
