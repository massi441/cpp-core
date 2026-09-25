#pragma once

#include <string>
#include <system_error>
#include <utility>

#include "Core/String/StringUtil.h"

namespace ml {

/**
 * A wrapper around a function's return value, with an error message when the value could not be produced.
 * Acts as an alternative to exceptions.
 */
template <typename T>
requires std::is_default_constructible_v<T>
class ReturnValue {
public:
    static ReturnValue Success(const T& value) {
        return ReturnValue(value, "");
    }

    static ReturnValue Failure(const std::string& message) {
        return ReturnValue(T(), message);
    }

    template <typename ...Args>
    requires (std::same_as<Args, const char*>&& ...)
    static ReturnValue Failure(Args... strings) {
        return ReturnValue(T(), ml::concatString(strings...));
    }

    static ReturnValue FromErrorCode(std::error_code ec, const T& valueIfSuccess) {
        return ec ? Failure(ec.message()) : Success(valueIfSuccess);
    }

    bool hasValue() const {
        return mMessage.empty();
    }

    T value() const {
        return mValue;
    }

    T get() const {
        return mValue;
    }

    const char* message() const {
        return mMessage.c_str();
    }

    operator bool() const {
        return this->hasValue();
    }

    explicit operator T() const {
        return mValue;
    }

private:
    T mValue;
    std::string mMessage;

    ReturnValue(const T& value, std::string message)
        : mValue(value), mMessage(std::move(message)) {}
};

}
