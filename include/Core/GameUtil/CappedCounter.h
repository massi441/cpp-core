#pragma once

#include "../Util/MathHelpers.h"

namespace ml {

class CappedCounter {
public:
    CappedCounter() = default;

    CappedCounter(int maxValue) {
        mMaxValue = mathi::max(0, maxValue);
    }

    CappedCounter(int initialValue, int maxValue) {
        mCount = initialValue;
        mMaxValue = maxValue;
    }

    /**
     * Increments the current count by one (if possible) and returns the new incremented value
     * @return The value incremented by one, if the value was previously smaller than the max value for this counter
     */
    int increment() {
        if (mCount < mMaxValue) {
            mCount++;
        }

        return mCount;
    }

    int decrement() {
        if (mCount > 0) {
            mCount--;
        }

        return mCount;
    }

    void reset() {
        mCount = 0;
    }

    bool isFull() const { return mCount >= mMaxValue; }
    bool isNotFull() const { return mCount < mMaxValue; }
    bool isEmpty() const { return mCount == 0; }

    int value() const { return mCount; }
    int max() const { return mMaxValue;}

    int peekPrevious() const { return mathi::max(0, mCount - 1); }
    int peekNext() const { return mathi::min(mCount + 1, mMaxValue); }

private:
    int mCount = 0;
    int mMaxValue = 0;
};

}
