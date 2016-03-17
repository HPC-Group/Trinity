//
//  GLVolumeBox.h
//  TNG
//
//  Created by Andre Waschk on 12/01/15.
//  Copyright (c) 2015 CoViDaG. All rights reserved.
//

#ifndef __TNG__GLVolumeBox__
#define __TNG__GLVolumeBox__

#include "GLModel.h"

namespace OpenGL{
namespace GLCore{
            
    class GLVolumeBox: public GLModel{
    public:
        GLVolumeBox();
        virtual ~GLVolumeBox();


        void SetSlideLevels(Core::Math::Vec3f min, Core::Math::Vec3f max);
			
                
    private:
        void FillCube(Core::Math::Vec3f min, Core::Math::Vec3f max);
        float*		m_Cube;

    };
          

};
};

#endif /* defined(__TNG__GLVolumeBox__) */
