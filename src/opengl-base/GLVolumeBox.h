#pragma once

#include "GLModel.h"

    class GLVolumeBox: public GLModel{
    public:
        GLVolumeBox();
        virtual ~GLVolumeBox();


        void SetSlideLevels(Core::Math::Vec3f min, Core::Math::Vec3f max);
			
                
    private:
        void FillCube(Core::Math::Vec3f min, Core::Math::Vec3f max);
        float*		m_Cube;

    };
          
