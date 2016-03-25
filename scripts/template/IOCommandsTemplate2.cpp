VclType {{CommandNameCmd}}::Type = VclType::{{VclType}};

{{CommandNameCmd}}::RequestParams::{{RequestCtorDeclaration}}
    : {{RequestInitializerList}} {}

void {{CommandNameCmd}}::RequestParams::serialize(ISerialWriter& writer) const {
{{RequestParamSerialization}}
}

void {{CommandNameCmd}}::RequestParams::deserialize(const ISerialReader& reader) {
	// TODO
}

bool {{CommandNameCmd}}::RequestParams::equals(const {{CommandNameCmd}}::RequestParams& other) const {
	// TODO
	return true;
}

{{RequestGetterDefinitions}}

std::string {{CommandNameCmd}}::RequestParams::toString() const {
    std::stringstream stream;
	// TODO
    return stream.str();
}

void {{CommandNameCmd}}::ReplyParams::serialize(ISerialWriter& writer) const {
{{ReplyParamSerialization}}
}

void {{CommandNameCmd}}::ReplyParams::deserialize(const ISerialReader& reader) {
	// TODO
}

bool {{CommandNameCmd}}::ReplyParams::equals(const {{CommandNameCmd}}::ReplyParams& other) const {
	// TODO
    return true;
}

{{ReplyGetterDefinitions}}

std::string {{CommandNameCmd}}::ReplyParams::toString() const {
    std::stringstream stream;
    return stream.str();
}
