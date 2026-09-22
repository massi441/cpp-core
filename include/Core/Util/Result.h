#pragma once

#include <optional>

#include "Core/Util/UtilConcepts.h"

namespace ml {

template <EnumClass E>
struct Result final {
public:
    static Result Success() {
        return Result();
    }

    static Result Failure(E error) {
        return Result(error);
    }

    bool isSuccess() const {
        return !mError.has_value();
    }

    bool isFailed() const {
        return mError.has_value();
    }

    E error() const {
        return mError.value();
    }

    operator bool() const {
        return !mError.has_value();
    }

private:
    std::optional<E> mError;

    Result() {}

    Result(E error) {
        mError = std::optional<E>(error);
    }
};

template <typename T, EnumClass E>
struct ReturnResult final {
public:
    template <typename F>
    static ReturnResult Success(F&& data) {
        return ReturnResult(std::forward<F>(data));
    }

    static ReturnResult Failure(E error) {
        return ReturnResult(error);
    }

    bool isSuccess() const {
        return !mError.has_value();
    }

    bool isFailed() const {
        return mError.has_value();
    }

    T& data() & {
        return mData.value();
    }

    const T& data() const& {
        return mData.value();
    }

    T&& data() && {
        return std::move(mData.value());
    }

    E error() const {
        return mError.value();
    }

    operator bool() const {
        return !mError.has_value();
    }

private:
    std::optional<T> mData;
    std::optional<E> mError;

    template <typename F>
    ReturnResult(F&& data) : mData(std::forward<F>(data)) {}

    ReturnResult(E error) : mError(error) {}
};

}
