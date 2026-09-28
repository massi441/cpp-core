#pragma once

#include <cstdint>
#include <type_traits>

namespace ml {

/**
 * A utility wrapper around a bitset configuration.
 * @tparam T The underlying type of the bit set (int32, int64...)
 * @tparam F The different flags values for the bit set.
 */
template <typename T, typename F>
requires std::is_integral_v<T> && std::is_enum_v<F>
class ConfigFlags {
public:
    void enable(F flag) {
        mFlags |= static_cast<T>(flag);
    }

    void disable(F flag) {
        mFlags &= static_cast<T>(~flag);
    }

    void enableAll() {
        mFlags = ~T(0);
    }

    void disableAll() {
        mFlags = 0;
    }

    bool isEnabled(F flag) const {
        return (mFlags & static_cast<T>(flag)) != 0;
    }

    bool isEmpty() const {
        return mFlags == 0;
    }

    bool hasFlags() const {
        return mFlags != 0;
    }

    T getFlags() const {
        return mFlags;
    }

private:
    T mFlags;
};

template <typename F>
using UInt32ConfigFlags = ConfigFlags<uint32_t, F>;

}