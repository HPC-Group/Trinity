#ifndef __RenderCmd__
#define __RenderCmd__

#pragma once

#include <string>

namespace RenderCmd {

    static std::string SET_RENDERMODE = "S_RNM";
    static std::string SET_COMPOSITING = "S_CMP";
    static std::string SET_CLEARVIEW = "S_CVM";

    static std::string MOV_CAMERA = "M_CAM";
    static std::string ROT_CAMERA = "R_CAM";
    static std::string ZOM_CAMERA = "Z_CAM";

    static std::string SET_CVPICK = "S_CVP";
};




#endif /* defined(__RenderServer__) */
