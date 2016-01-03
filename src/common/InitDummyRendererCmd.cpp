#include "InitDummyRendererCmd.h"

using namespace trinity::common;

InitDummyRendererCmd::InitDummyRendererCmd(int sid, int rid) : ICommand(sid, rid){}


InitDummyRendererCmd::~InitDummyRendererCmd() {}

VclType InitDummyRendererCmd::getType() const {
    return VclType::InitDummyRenderer;
}


void InitDummyRendererCmd::serialize(std::ostream& stream) {
    // todo
}


void InitDummyRendererCmd::deserialize(std::istream& stream) {
    // todo
}