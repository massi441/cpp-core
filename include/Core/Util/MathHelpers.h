#pragma once

#include <cmath>
#include "Core/Util/Types.h"

template <typename T>
requires std::is_arithmetic_v<T>
struct MathHelpers {
    static T min(T a, T b) {
        if (a < b) {
            return a;
        }

        return b;
    }

    static T min(T a, T b, T c) {
        return min(min(a, b), c);
    }

    static T max(T a, T b) {
        if (a > b) {
            return a;
        }

        return b;
    }

    static T max(T a, T b, T c) {
        return max(max(a, b), c);
    }

    static T clamp(T value, T min, T max) {
        if (value < min) {
            return min;
        }
        if (value > max) {
            return max;
        }
        return value;
    }

    static void clamp(T* outValue, T min, T max) {
        *outValue = clamp(*outValue, min, max);
    }

    static T clampMax(T value, T max) {
        if (value > max) {
            return max;
        }
        return value;
    }

    static T clampMin(T value, T min) {
        if (value < min) {
            return min;
        }
        return value;
    }

    static T abs(T value) {
        return value > 0 ? value : -value;
    }

    /**
     * Computes the exponent a base has be raised by, to get the closest value to the desired value.
     * @param base The base to raise
     * @param targetValue The desired value
     * @return The exponent a base has to be raised by to get the closest value to the desired value
     */
    static T powCeil(T base, T targetValue) {
        return std::ceil(std::log(targetValue) / std::log(base));
    }

    static T logBase(T base, T value) {
        return std::log(value) / std::log(base);
    }

    /**
     * Loops a value within a given range. If the value is smaller than the minimum, back to the maximum.
     * If the value is bigger than the maximum, it is looped back to the minimum
     * @param value The value to loop
     * @param min The inclusive minimum value
     * @param max The inclusive maximum value
     * @return The looped value
     */
    static T loopVal(T value, T min, T max) {
        if (value > max) {
            return min;
        }
        if (value < min) {
            return max;
        }
        return value;
    }

    static bool tryDivide(T& out, T a, T b, T defaultValue) {
        if (b == 0) {
            out = defaultValue;
            return false;
        }

        out = a / b;
        return true;
    }

    static bool inRangeIncl(T value, T min, T max) {
        return value >= min && value <= max;
    }

    static bool inRangeExcl(T value, T min, T max) {
        return value > min && value < max;
    }

    static T sigmoid(T x, T maxValue, T sharpness, T midPoint) {
        return maxValue / (static_cast<T>(1) + std::exp(-sharpness * (x - midPoint)));
    }

    static T clampedSigmoid(T x, T minValue, T maxValue, T sharpness, T midPoint) {
        return minValue + (maxValue - minValue) * MathHelpers::sigmoid(x, static_cast<T>(1), sharpness, midPoint);
    }

    static T sigmoidStart(T sharpness, T midPoint, T threshold) {
        return midPoint - std::log(1.0 / threshold - 1.0) / sharpness;
    }
};

using mathb = MathHelpers<ml::byte>;
using mathus = MathHelpers<ml::ushort>;
using mathi = MathHelpers<int>;
using mathul = MathHelpers<ml::ulong>;
using mathl = MathHelpers<int64_t>;
using mathf = MathHelpers<float>;
