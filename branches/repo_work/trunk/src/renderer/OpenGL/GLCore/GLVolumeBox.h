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


struct Side{
	Side() :
	m_p1(0, 0, 0),
	m_p2(0, 0, 0),
	m_p3(0, 0, 0),
	m_p4(0, 0, 0){
	};

	Side(Core::Math::Vec3f p1, Core::Math::Vec3f p2, Core::Math::Vec3f p3, Core::Math::Vec3f p4) :
		m_p1(p1),
		m_p2(p2),
		m_p3(p3),
		m_p4(p4){
	};

	Core::Math::Vec3f m_p1;
	Core::Math::Vec3f m_p2;
	Core::Math::Vec3f m_p3;
	Core::Math::Vec3f m_p4;
};

namespace Tuvok{
  namespace Renderer{
    namespace OpenGL{
      namespace GLCore{
            
        class GLVolumeBox: public GLModel{
        public:
            GLVolumeBox();
            virtual ~GLVolumeBox();


			void SetSlideLevels(Core::Math::Vec3f min, Core::Math::Vec3f max);

			
			
                
        private:
			void FillCube(Core::Math::Vec3f min, Core::Math::Vec3f max);

			Side*		m_Cube;

        };
          
      };
    };
  };
};

#endif /* defined(__TNG__GLVolumeBox__) */
