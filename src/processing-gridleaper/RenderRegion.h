//
//  RenderRegion.h
//  TNG
//
//  Created by Andre Waschk on 28/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__RenderRegion__
#define __TNG__RenderRegion__

#include <core/Math/Vectors.h>

namespace Tuvok {
  namespace Renderer{

    class RenderRegion{

    public:
      RenderRegion(Core::Math::Vec2ui minCoord, Core::Math::Vec2ui maxCoord);
      virtual ~RenderRegion();

      bool ContainsPoint(Core::Math::Vec2ui pos);

      const Core::Math::Vec2ui getMinCoord() const;
      const Core::Math::Vec2ui getMaxCoord() const;

    private:
      Core::Math::Vec2ui m_minCoord;
      Core::Math::Vec2ui m_maxCoord;
    };
  }
}
#endif /* defined(__TNG__RenderRegion__) */
