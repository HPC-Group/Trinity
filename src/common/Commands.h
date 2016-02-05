#pragma once
#include <memory>
#include <map>
#include "mocca/base/ByteArray.h"
#include "mocca/base/StringTools.h"
#include "mocca/base/BidirectionalMap.h"


// subject of refactoring
namespace trinity {
namespace common {
    
enum class VclType {

    InitRenderer,
    TrinityReturn,
    TrinityError,
    DummyRenderer,
    GridLeaper,
    StreamingParams
};
    
typedef std::unique_ptr<mocca::ByteArray> Frame;


// use this class to create and parse trinity commands
class Vcl {
    
public:
    
    // throws Error if entry not found
    std::string toString(const VclType& t) const;
    
    // throws Error if entry not found
    VclType toType(const std::string& str) const;
    
    // todo test
    std::string toString(const int errorCode);

    Vcl();
    
    
private:
    mocca::BidirectionalMap<std::string, VclType> m_cmdMap;
    std::map<int, std::string> m_errorCodeMap;
};
    
class IDGenerator {
private:
     int m_id;
    
public:
    IDGenerator();
    int nextID();
};
}
} // end trinity