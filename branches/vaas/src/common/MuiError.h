#pragma once

#include <string>
#include <sstream>
#include <stdexcept>

// VS2013 doesn't support the 'noexcept' directive
#ifndef _MSC_VER
#define NOEXCEPT noexcept
#else
#define NOEXCEPT
#endif

#define CODE_LOCATION_IN_ERROR

class MuiError : public std::runtime_error {
public:
    MuiError(const std::string& msg, const std::string& file, int line)
        : std::runtime_error(""), msg_(msg), file_(file), line_(line) {
        std::stringstream str;
        str << msg_;
        std::string internalMsg = internalError();
        if (!internalMsg.empty())
            str << " (internal error: " << internalMsg << ")";
#ifdef CODE_LOCATION_IN_ERROR
        if (file_.empty() && line_ == -1)
            str << " (" << file_ << ":" << std::to_string(line_) << ")";
#endif
        msg_ = str.str();
    }

    virtual ~MuiError() {}

    const char* what() const NOEXCEPT override { return msg_.c_str(); }

protected:
    virtual std::string internalError() const { return std::string(); }

private:
    std::string msg_;
    std::string file_;
    int line_;
};