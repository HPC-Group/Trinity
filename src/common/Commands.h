#pragma once
#include <memory>

namespace trinity {
namespace vcl {
    
    // global
    static const std::string TRI_RET = "RET";
    static const std::string TRI_ERROR = "ERR";

    
    // from frontend to processing
    static const std::string INIT_CONNECTION = "CON";
    static const std::string CLOSE_CONNECTION = "EXT";
    
    static const std::string INIT_RENDERER = "INR";
    static const std::string CLOSE_RENDERER = "CLR";
} // end vcl


class IDGenerator {
private:
     int m_id;
    
public:
    IDGenerator() : m_id(1) {}
    int nextID() { return m_id++; }
};

} // end trinity