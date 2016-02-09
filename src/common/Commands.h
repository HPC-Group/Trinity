#pragma once
#include <memory>
#include <map>
#include "mocca/base/ByteArray.h"
#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"
#include "mocca/base/BidirectionalMap.h"


namespace trinity {
namespace common {

// visualization command language types
// everything that is part of trinity protocol has to be specified here
// e.g., each command, each passed struct/class
enum class VclType {

    InitRenderer,
    TrinityReturn,
    TrinityError,
    DummyRenderer,
    GridLeaper,
    StreamingParams,
    First = InitRenderer,
    Last = StreamingParams,
};



VclType& operator++(VclType& x);
VclType operator*(VclType c);
VclType begin(VclType r);
VclType end(VclType r);

    
// use this class to create and parse trinity commands
class Vcl {
public:
    
    // throws Error if entry not found
    std::string toString(const VclType& t) const;
    
    // throws Error if entry not found
    VclType toType(const std::string& str) const;
    
    // todo test
    std::string toString(const int errorCode);

    // keep these synchronized with the VclType enums
    Vcl() {
        m_cmdMap.insert("INR", VclType::InitRenderer);
        m_cmdMap.insert("DRN", VclType::DummyRenderer);
        m_cmdMap.insert("GRN", VclType::GridLeaper);
        m_cmdMap.insert("RET", VclType::TrinityReturn);
        m_cmdMap.insert("ERR", VclType::TrinityError);
        m_cmdMap.insert("PAR", VclType::StreamingParams);
        
        m_errorCodeMap.insert(std::make_pair(1, "no such command"));
        m_errorCodeMap.insert(std::make_pair(2, "no such renderer type"));
        
        assertCompleteLanguage();
    }
    
private:
    mocca::BidirectionalMap<std::string, VclType> m_cmdMap;
    std::map<int, std::string> m_errorCodeMap;
    void assertCompleteLanguage() const;
};

    
// the trinity protocol uses request- and session id's (rid and sid)
class IDGenerator {
private:
     int m_id;
    
public:
    IDGenerator();
    int nextID();
};
    
typedef std::unique_ptr<mocca::ByteArray> Frame;
    
}
} // end trinity