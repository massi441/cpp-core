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

    /**
     * Updates the current value and diffs it against the previous value.
     * Note: Updates the value regardless of diff result
     * @param newValue The new value to update and diff
     * @return True if the new value is different from the previous one, false otherwise
     */
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

/**
 * A diffable item that stores a previous and current value
 * @tparam T The type of item
 */
template <typename T>
class StatefulDiffable {
public:
    StatefulDiffable() = default;

    StatefulDiffable(const T& initial) {
        mPrevious = initial;
        mCurrent = initial;
    }

    /**
     * Sets the previous value with the current value, then updates the current
     * value with the new value, returning true if the new value is different than the previous value
     */
    template <typename F>
    bool update(F&& newValue) {
        mPrevious = std::move(mCurrent);
        mCurrent = std::forward<F>(newValue);
        return this->isNew();
    }

    bool isNew() const {
        return mPrevious != mCurrent;
    }

    bool isSame() const {
        return mCurrent == mPrevious;
    }

    const T& previous() const { return mPrevious; }
    const T& current() { return mCurrent; }

    operator const T&() { return mCurrent; }

private:
    T mPrevious;
    T mCurrent;
};

}
