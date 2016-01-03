#include "InitDummyRendererCmd.h"

using namespace trinity::common;

InitDummyRendererCmd::InitDummyRendererCmd(int sid, int rid) : ICommand(sid, rid){}

InitDummyRendererCmd::InitDummyRendererCmd(int sid,
                                           int rid,
                                           const std::string& protocol,
                                           StreamingParams p) :
ICommand(sid, rid), m_protocol(protocol), m_streamingParams(p) {}


InitDummyRendererCmd::~InitDummyRendererCmd() {}

VclType InitDummyRendererCmd::getType() const {
    return VclType::InitDummyRenderer;
}


void InitDummyRendererCmd::serialize(std::ostream& stream) {
    stream << m_sid << " " << m_rid << " ";
    m_streamingParams.serialize(stream);
}


void InitDummyRendererCmd::deserialize(std::istream& stream) {
    stream >> m_sid >> m_rid;
    m_streamingParams.deserialize(stream);
}