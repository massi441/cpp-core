#pragma once

#include "Core/Util/MathHelpers.h"

namespace ml {

class CircularCounter {
public:
    CircularCounter() = default;

    CircularCounter(int min, int max) {
        mMin = min;
        mMax = max;
    }

    CircularCounter(int min, int max, int initialValue) : CircularCounter(min, max) {
        mValue = initialValue;
    }

    void setValue(int newValue) {
        if (mathi::inRangeIncl(newValue, mMin, mMax)) {
            mValue = newValue;
        }
    }

    void increment() {
        int newValue = mValue + 1;
        mValue = mathi::loopVal(newValue, mMin, mMax);
    }

    void increment(int times) {
        if (times <= 0) {
            return;
        }

        this->increment();
        this->increment(times - 1);
    }

    void decrement() {
        int newValue = mValue - 1;
        mValue = mathi::loopVal(newValue, mMin, mMax);
    }

    void decrement(int times) {
        if (times <= 0) {
            return;
        }

        this->decrement();
        this->decrement(times - 1);
    }

    int getValue() const { return mValue; }

private:
    int mValue = 0;
    int mMin = 0;
    int mMax = 0;
};

}
