#pragma once

#include <cstdint>
#include <cstring>

namespace ml {

/**
 * A stream for measuring the size of a buffer.
 */
class SizeStream {
public:
    template <typename T>
    void write() { mSize += sizeof(T); }

    template <typename T>
    void writeTimes(uint64_t times) { mSize += sizeof(T) * times; }

    void writeBytes(uint64_t byteCount) { mSize += byteCount; }
    void writeStr(const char* str) { mSize += strlen(str); }

    uint64_t size() const { return mSize; }

private:
    uint64_t mSize = 0;
};

}
