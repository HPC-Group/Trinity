#pragma once

#include <map>
#include <memory>

#include "mocca/base/BidirectionalMap.h"
#include "mocca/base/ByteArray.h"
#include "mocca/base/Error.h"
#include "mocca/base/StringTools.h"


namespace trinity {
namespace commands {

// visualization command language types
// everything that is part of trinity protocol has to be specified here
// e.g., each command, each passed struct/class
enum class VclType {
    InitRenderer,
    TrinityReturn,
    TrinityError,
    DummyRenderer,
    GridLeaper,
    DummyIO,
    FractalIO,
    SetIsoValue,
    GetLODLevelCount,
    ListFiles,
    InitIOSession,
    StreamingParams,
    IOData,
    TrinityFile,
    TrinityDirectory,
    First = InitRenderer,
    Last = StreamingParams,
};

VclType& operator++(VclType& x);
VclType operator*(VclType c);
VclType begin(VclType r);
VclType end(VclType r);
std::ostream& operator<<(std::ostream& os, VclType obj);

// use this class to create and parse trinity commands
class Vcl {
public:
    static const Vcl& instance();

    // throws Error if entry not found
    std::string toString(const VclType& t) const;

    // throws Error if entry not found
    VclType toType(const std::string& str) const;

    // todo test
    std::string toString(const int errorCode) const;

private:
    // keep these synchronized with the VclType enums
    Vcl() {
        m_cmdMap.insert("INR", VclType::InitRenderer);
        m_cmdMap.insert("DRN", VclType::DummyRenderer);
        m_cmdMap.insert("GRN", VclType::GridLeaper);
        m_cmdMap.insert("RET", VclType::TrinityReturn);
        m_cmdMap.insert("ERR", VclType::TrinityError);
        m_cmdMap.insert("PAR", VclType::StreamingParams);
        m_cmdMap.insert("ISO", VclType::SetIsoValue);
        m_cmdMap.insert("INI", VclType::InitIOSession);
        m_cmdMap.insert("LST", VclType::ListFiles);
        m_cmdMap.insert("LOD", VclType::GetLODLevelCount);
        m_cmdMap.insert("DIO", VclType::DummyIO);
        m_cmdMap.insert("FIO", VclType::FractalIO);
        m_cmdMap.insert("DIR", VclType::TrinityDirectory);
        m_cmdMap.insert("FIL", VclType::TrinityFile);
        m_cmdMap.insert("IOD", VclType::IOData);

        m_errorCodeMap.insert(std::make_pair(1, "no such command"));
        m_errorCodeMap.insert(std::make_pair(2, "no such renderer type"));

        assertCompleteLanguage();
    }

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
}
} // end trinity