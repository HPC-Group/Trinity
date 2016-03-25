VclType {{CommandNameCmd}}::Type = VclType::{{VclType}};

{{CommandNameCmd}}::RequestParams::{{RequestCtorDeclaration}}
{{RequestInitializerList}} {}

void {{CommandNameCmd}}::RequestParams::serialize(ISerialWriter& writer) const {
{{RequestParamSerialization}}
}

void {{CommandNameCmd}}::RequestParams::deserialize(const ISerialReader& reader) {
{{RequestParamDeserialization}}
}

bool {{CommandNameCmd}}::RequestParams::equals(const {{CommandNameCmd}}::RequestParams& other) const {
{{RequestParamEquals}}
}

{{RequestGetterDefinitions}}

std::string {{CommandNameCmd}}::RequestParams::toString() const {
    std::stringstream stream;
{{RequestParamStreaming}}
    return stream.str();
}

void {{CommandNameCmd}}::ReplyParams::serialize(ISerialWriter& writer) const {
{{ReplyParamSerialization}}
}

void {{CommandNameCmd}}::ReplyParams::deserialize(const ISerialReader& reader) {
{{ReplyParamDeserialization}}
}

bool {{CommandNameCmd}}::ReplyParams::equals(const {{CommandNameCmd}}::ReplyParams& other) const {
{{ReplyParamEquals}}
}

{{ReplyGetterDefinitions}}

std::string {{CommandNameCmd}}::ReplyParams::toString() const {
    std::stringstream stream;
{{ReplyParamStreaming}}
    return stream.str();
}