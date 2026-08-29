#pragma once

#include <utility>

namespace ml {

template <typename T>
class Diffable {
public:
    Diffable() : mCurrent() {

    }

    Diffable(const T& initial) {
        mCurrent = initial;
    }

    bool diff(const T& newValue) {
        return newValue != mCurrent;
    }

    bool updateAndDiff(const T& newValue) {
        bool isNew = newValue != mCurrent;
        mCurrent = newValue;
        return isNew;
    }

    template <typename F>
    void update(F&& newValue) {
        mCurrent = std::forward<F>(newValue);
    }

    T& current() { return mCurrent; }
    const T& current() const { return mCurrent; }

    operator T&() { return mCurrent; }
    operator const T&() const { return mCurrent; }

private:
    T mCurrent;
};

}
