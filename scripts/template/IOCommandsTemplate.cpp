bool operator==(const {{CommandNameCmd}}::RequestParams& lhs, const {{CommandNameCmd}}::RequestParams& rhs) {
    return lhs.equals(rhs);
}
bool operator==(const {{CommandNameCmd}}::ReplyParams& lhs, const {{CommandNameCmd}}::ReplyParams& rhs) {
    return lhs.equals(rhs);
}
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::RequestParams& obj) {
    return os << obj.toString();
}
std::ostream& operator<<(std::ostream& os, const {{CommandNameCmd}}::ReplyParams& obj) {
    return os << obj.toString();
}
