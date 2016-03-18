//
//  GLRenderPlane.h
//  TNG
//
//  Created by Andre Waschk on 18/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLRenderPlane__
#define __TNG__GLRenderPlane__

#include "GLModel.h"

namespace OpenGL{
namespace GLCore{
        
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
        
};
};

#endif /* defined(__TNG__GLRenderPlane__) */
