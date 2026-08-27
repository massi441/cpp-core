#pragma once

#include <string>

#include "Core/String/StringUtil.h"

namespace ml {

template <typename T>
requires std::is_default_constructible_v<T>
class ReturnValue {
public:
    template <typename F>
    ReturnValue(F&& value) {
        mValue = std::forward<F>(value);
        mMessage = "";
    }

    ReturnValue(const std::string& message) {
        mValue = T();
        mMessage = message;
    }

    template <typename ...Args>
    requires (std::same_as<Args, const char*>&& ...)
    explicit ReturnValue(Args... string) {
        mValue = T();
        mMessage = ml::concatString(string...);
    }

    template <typename F>
    explicit ReturnValue(std::error_code ec, F&& valueIfSuccess) {
        if (ec) {
            mValue = T();
            mMessage = ec.message();
        } else {
            mValue = valueIfSuccess;
            mMessage = "";
        }
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
};

}
