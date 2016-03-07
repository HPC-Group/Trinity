#include "commands/Vcl.h"

#include "common/TrinityError.h"

#include "mocca/log/LogManager.h"

#include <limits>

using namespace trinity::commands;
using namespace trinity::common;


namespace trinity {
namespace commands {

const Vcl& trinity::commands::Vcl::instance() {
    static Vcl inst;
    return inst;
}

VclType& operator++(VclType& x) {
    return x = (VclType)(std::underlying_type<VclType>::type(x) + 1);
}

VclType operator*(VclType c) {
    return c;
}

VclType begin(VclType r) {
    return VclType::First;
}
VclType end(VclType r) {
    VclType l = VclType::Last;
    return ++l;
}

std::ostream& operator<<(std::ostream& os, VclType obj) {
    return os << Vcl::instance().toString(obj);
}
}
}

void Vcl::assertCompleteLanguage() const {
    for (const auto& token : VclType()) {
        try {
            m_cmdMap.getBySecond(token);
        } catch (const mocca::Error&) {
            throw TrinityError("(common) no string entry for enum number " + std::to_string((int)token), __FILE__, __LINE__);
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

uint32_t IDGenerator::nextID() {
    static uint32_t id = 0;
    return (id + 1) % std::numeric_limits<uint32_t>::max();
}