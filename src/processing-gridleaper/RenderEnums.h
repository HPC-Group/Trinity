#ifndef TNG_RenderEnums_h
#define TNG_RenderEnums_h

namespace Tuvok{
  namespace Renderer{

		  enum ERenderMode {
			  RM_1DTRANS = 0,  /**< one dimensional transfer function */
			  RM_2DTRANS,      /**< two dimensional transfer function */
			  RM_ISOSURFACE,   /**< render isosurfaces                */
			  RM_INVALID
		  };

		  enum ECompositeDisplay{
			  D_FINALCOLOR = 0,
			  D_RESUMECOLOR,
			  D_RESUMEPOSITION,
			  D_DEBUGVIEW,
			  D_FINALDEBUGMIX,
			  D_FINALDEBUGMIXALPHA
		  };

		  enum MissingBrickStrategy {
			  OnlyNeeded,
			  RequestAll,
			  SkipOneLevel,
			  SkipTwoLevels
		  };

		  enum System{
            Windows = 0,
            Linux,
            OSX
		  };

		  enum Visibility{
            Windowed = 0,
            Fullscreen,
            hidden
		  };
  };
};

#endif
