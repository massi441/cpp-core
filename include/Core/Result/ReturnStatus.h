#pragma once

#include <format>
#include <string>
#include <system_error>
#include <utility>

namespace ml {

/**
 * A wrapper around a function's success status, with an optional error message for failures.
 * Acts as an alternative to exceptions.
 */
class ReturnStatus {
public:
    static ReturnStatus Success() {
        return ReturnStatus(true, "");
    }

    static ReturnStatus Failure() {
        return ReturnStatus(false, "");
    }

    static ReturnStatus Failure(const std::string& message) {
        return ReturnStatus(false, message);
    }

    template <typename ...Args>
    static ReturnStatus Failure(std::format_string<Args...> fmt, Args&&... args) {
        return ReturnStatus(false, std::format(fmt, std::forward<Args>(args)...));
    }

    static ReturnStatus FromErrorCode(std::error_code ec) {
        return ec ? Failure(ec.message()) : Success();
    }

    bool isSuccess() const {
        return mIsSuccess;
    }

    bool isFailed() const {
        return !mIsSuccess;
    }

    const char* message() const {
        return mMessage.c_str();
    }

private:
    bool mIsSuccess;
    std::string mMessage;

    ReturnStatus(bool isSuccess, std::string message)
        : mIsSuccess(isSuccess), mMessage(std::move(message)) {}
};

}
