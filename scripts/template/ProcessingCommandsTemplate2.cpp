VclType {CommandName}Cmd::Type = VclType::{VclType};

void {CommandName}Cmd::RequestParams::serialize(ISerialWriter& writer) const {
	writer.appendString("renderMode", IRenderer::renderModeMapper().getByFirst(m_renderMode));
}

void {CommandName}Cmd::RequestParams::deserialize(const ISerialReader& reader) {
	m_renderMode = IRenderer::renderModeMapper().getBySecond(reader.getString("renderMode"));
}

bool {CommandName}Cmd::RequestParams::equals(const {CommandName}Cmd::RequestParams& other) const {
	return m_renderMode == other.m_renderMode;
}

std::string {CommandName}Cmd::RequestParams::toString() const {
	std::stringstream stream;
	stream << "renderMode: " << IRenderer::renderModeMapper().getByFirst(m_renderMode);
	return stream.str();
}

IRenderer::ERenderMode {CommandName}Cmd::RequestParams::getRenderMode() const {
	return m_renderMode;
}

void {CommandName}Cmd::ReplyParams::serialize(ISerialWriter& writer) const {
	// TODO
}

void {CommandName}Cmd::ReplyParams::deserialize(const ISerialReader& reader) {
	// TODO
}

bool {CommandName}Cmd::ReplyParams::equals(const {CommandName}Cmd::ReplyParams& other) const {
	// TODO
    return true;
}

std::string {CommandName}Cmd::ReplyParams::toString() const {
    std::stringstream stream;
    return stream.str();
}
