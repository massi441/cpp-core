#pragma once

#include <memory>

#include "Core/Util/Types.h"

namespace ml {

// TODO: Add bytes written field

/**
 * An unsafe writer to arbitrary memory
 */
class SpanWriter {
public:
    SpanWriter(void* stream) {
        mCursor = static_cast<char*>(stream);
    }

    template <typename T>
    void write(const T& val) {
        *reinterpret_cast<T*>(mCursor) = val;
        mCursor += sizeof(T);
    }

    template <typename T>
    void writeSerializable(const T& serializable) {
        serializable.serialize(this);
    }

    void write(const void* src, size_t size) {
        std::memcpy(mCursor, src, size);
        mCursor += size;
    }

    /**
     * Writes a string into the stream, but does not include the null terminator
     * @param str the string to write into the stream
     */
    void writeStr(const char* str) {
        size_t len = strlen(str);
        write(str, len);
        mCursor += len;
    }

    template <typename T>
    void skip() {
        mCursor += sizeof(T);
    }

    void skip(uint byteCount) {
        mCursor += byteCount;
    }

    uint64_t offsetFrom(const void* start) const {
        return reinterpret_cast<uint64_t>(mCursor) - reinterpret_cast<uint64_t>(start);
    }

    char* cursor() const { return mCursor; }

private:
    char* mCursor;
};

}
