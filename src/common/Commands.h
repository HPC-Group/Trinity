#pragma once
#include <memory>

namespace trinity {
namespace vcl {
    
    static const int MIN_CMD_LENGTH = 3;
    // global
    static const std::string TRI_RET = "RET";
    static const std::string TRI_ERROR = "ERR";

    
    // managing connections
    static const std::string INIT_CONNECTION = "CON";
    static const std::string CLOSE_CONNECTION = "EXT";
    
    // managing processing/rendering instances
    static const std::string INIT_RENDERER = "INR";
    static const std::string CLOSE_RENDERER = "CLR";
    
    // renderer types
    static const std::string DUMMY_RENDERER = "DRN";
    static const std::string GRID_LEAPER_RENDERER = "GRN";
    
    static const std::string GET_FRAMEBUFFER = "GFB";
    
    
} // end vcl


class IDGenerator {
private:
     int m_id;
    
public:
    IDGenerator() : m_id(1) {}
    int nextID() { return m_id++; }
};

} // end trinity