#include "Vcl.h"

#include "mocca/log/LogManager.h"

using namespace trinity::commands;

namespace trinity {
namespace commands {

const Vcl& trinity::commands::Vcl::instance() {
    static Vcl inst;
    return inst;
}

VclType& operator++(VclType& x) {
    return x = (VclType)(std::underlying_type<VclType>::type(x) + 1);
}


VclType operator*(VclType c) {return c;}

VclType begin(VclType r) {return VclType::First;}
VclType end(VclType r)   {VclType l=VclType::Last; return ++l; }
}
}

void Vcl::assertCompleteLanguage() const {
    for(const auto& token : VclType()) {
        try {
            m_cmdMap.getBySecond(token);
        } catch (const mocca::Error&) {
            throw mocca::Error("(common) no string entry for enum number "
                               + std::to_string((int)token), __FILE__, __LINE__);
        }
        
    }
}


std::string Vcl::toString(const VclType& t) const {
    return m_cmdMap.getBySecond(t);
}

// throws Error if entry not found
VclType Vcl::toType(const std::string& str) const {
    return m_cmdMap.getByFirst(str);
}

std::string Vcl::toString(const int errorCode) const {
    auto it = m_errorCodeMap.find(errorCode);
    return it->second;
}


IDGenerator::IDGenerator() : m_id(1) {}
int IDGenerator::nextID() { return m_id++; }