#pragma once

#include "Core/Container/ByteBuffer.h"
#include "Core/Util/MathHelpers.h"

#include <string_view>

namespace ml {

/**
 * A container for an inline, fixed size, and null terminated string
 * @tparam Size The size of the string
 */
template <uint64_t Size>
class FixedString {
public:
    FixedString() = default;

    FixedString(const char* str, size_t length) {
        uint64_t maxLength = mathi::min(Size - 1, length);
        std::memcpy(mBuf.data(), str, maxLength);
    }

    FixedString(const char* str) : FixedString(str, std::strlen(str)) {

    }

    FixedString(std::string_view strView) : FixedString(strView.data(), strView.size()) {

    }

    uint64_t length() const {
        return std::strlen(this->cstr());
    }

    const char* cstr() const {
        return reinterpret_cast<const char*>(mBuf.cdata());
    }

private:
    ByteBuffer<Size> mBuf = ByteBuffer<Size>();
};

}
