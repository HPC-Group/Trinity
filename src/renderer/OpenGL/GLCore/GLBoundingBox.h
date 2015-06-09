//
//  GLBoundingBox.h
//  TNG
//
//  Created by Andre Waschk on 30/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLBoundingBox__
#define __TNG__GLBoundingBox__

#include "GLModel.h"

namespace Tuvok{
  namespace Renderer{
    namespace OpenGL{
      namespace GLCore{
        
        class GLBoundingBox: public GLModel{
        public:
          GLBoundingBox();
          virtual ~GLBoundingBox();
          
        private:
        };
        typedef std::shared_ptr<GLBoundingBox> GLBoundingBoxPtr;
        
      };
    };
  };
};

#endif /* defined(__TNG__GLBoundingBox__) */
