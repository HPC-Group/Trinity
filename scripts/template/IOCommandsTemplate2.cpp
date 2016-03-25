VclType {CommandName}Cmd::Type = VclType::{VclType};

{CommandName}Cmd::RequestParams::RequestParams({Parameters})
    : m_index({Parameter_Variables}) {}

void {CommandName}Cmd::RequestParams::serialize(ISerialWriter& writer) const {
	// TODO
}

void {CommandName}Cmd::RequestParams::deserialize(const ISerialReader& reader) {
	// TODO
}

bool {CommandName}Cmd::RequestParams::equals(const {CommandName}Cmd::RequestParams& other) const {
	// TODO
	return true;
}

std::string {CommandName}Cmd::RequestParams::toString() const {
    std::stringstream stream;
	// TODO
    return stream.str();
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
