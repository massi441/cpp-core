#pragma once

#include <format>
#include <optional>
#include <string>
#include <system_error>
#include <utility>

namespace ml {

/**
 * A wrapper around a function's return value, with an error message when the value could not be produced.
 * Acts as an alternative to exceptions.
 */
template <typename T>
class ReturnValue final {
public:
    template <typename F>
    static ReturnValue Success(F&& value) {
        return ReturnValue(ValueTag{}, std::forward<F>(value));
    }

    static ReturnValue Failure(const std::string& message) {
        return ReturnValue(FailureTag{}, message);
    }

    template <typename ...Args>
    static ReturnValue Failure(std::format_string<Args...> fmt, Args&&... args) {
        return ReturnValue(FailureTag{}, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename F>
    static ReturnValue FromErrorCode(std::error_code ec, F&& valueIfSuccess) {
        return ec ? Failure(ec.message()) : Success(std::forward<F>(valueIfSuccess));
    }

    bool hasValue() const {
        return mValue.has_value();
    }

    bool isSuccess() const {
        return mValue.has_value();
    }

    bool isFailed() const {
        return !mValue.has_value();
    }

    T& value() & {
        return mValue.value();
    }

    const T& value() const& {
        return mValue.value();
    }

    T&& value() && {
        return std::move(mValue.value());
    }

    T& get() & {
        return mValue.value();
    }

    const T& get() const& {
        return mValue.value();
    }

    T&& get() && {
        return std::move(mValue.value());
    }

    const char* message() const {
        return mMessage.c_str();
    }

    explicit operator const T&() const {
        return mValue.value();
    }

private:
    std::optional<T> mValue;
    std::string mMessage;

private:
    struct ValueTag {};
    struct FailureTag {};

    template <typename F>
    ReturnValue(ValueTag, F&& value) : mValue(std::forward<F>(value)) {}

    ReturnValue(FailureTag, std::string message) : mMessage(std::move(message)) {}
};

}
