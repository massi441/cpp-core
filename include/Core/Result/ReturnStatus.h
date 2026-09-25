#pragma once

#include <string>
#include <system_error>
#include <utility>

#include "Core/String/StringUtil.h"

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
    requires (std::same_as<Args, const char*>&& ...)
    static ReturnStatus Failure(Args... strings) {
        return ReturnStatus(false, ml::concatString(strings...));
    }

    static ReturnStatus FromErrorCode(std::error_code ec) {
        return ec ? Failure(ec.message()) : Success();
    }

    bool isSuccess() const {
        return mIsSuccess;
    }

    const char* message() const {
        return mMessage.c_str();
    }

    operator bool() const {
        return mIsSuccess;
    }

private:
    bool mIsSuccess;
    std::string mMessage;

    ReturnStatus(bool isSuccess, std::string message)
        : mIsSuccess(isSuccess), mMessage(std::move(message)) {}
};

}
