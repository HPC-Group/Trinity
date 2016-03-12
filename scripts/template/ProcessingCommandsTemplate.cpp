bool operator==(const {CommandName}Cmd::RequestParams& lhs, const {CommandName}Cmd::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const {CommandName}Cmd::ReplyParams& lhs, const {CommandName}Cmd::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const {CommandName}Cmd::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const {CommandName}Cmd::ReplyParams& obj) {
    return os << obj.toString();
}
