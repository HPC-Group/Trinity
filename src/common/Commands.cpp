#include "Commands.h"

using namespace trinity::common;

Vcl::Vcl() {
    m_cmdMap.insert("INR", VclType::InitRenderer);
    m_cmdMap.insert("DRN", VclType::DummyRenderer);
    m_cmdMap.insert("GRN", VclType::GridLeaper);
    m_cmdMap.insert("RET", VclType::TrinityReturn);
    m_cmdMap.insert("ERR", VclType::TrinityError);
    m_cmdMap.insert("PAR", VclType::StreamingParams);
    
    m_errorCodeMap.insert(std::make_pair(1, "no such command"));
    m_errorCodeMap.insert(std::make_pair(2, "no such renderer type"));
    
}




std::string Vcl::toString(const VclType& t) const {
    return m_cmdMap.getBySecond(t);
}

// throws Error if entry not found
VclType Vcl::toType(const std::string& str) const {
    return m_cmdMap.getByFirst(str);
}

std::string Vcl::toString(const int errorCode) {
    return m_errorCodeMap[errorCode];
}


IDGenerator::IDGenerator() : m_id(1) {}
int IDGenerator::nextID() { return m_id++; }