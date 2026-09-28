#pragma once

#include <cstdint>
#include <cstring>

#include "Core/Util/Types.h"

namespace ml {

template <uint64_t Size>
class ByteBuffer {
public:
    ByteBuffer() = default;

    ByteBuffer& operator=(const ByteBuffer& other) {
        if (&other == this) {
            return *this;
        }

        std::memcpy(mBuf, other.mBuf, Size);
        return *this;
    }

    static uint64_t size() { return Size; }

    byte* data() { return mBuf; }
    const byte* cdata() const { return mBuf; }

private:
    byte mBuf[Size] = {};
};

}
