#pragma once

#include <memory>

#include "Core/Util/Types.h"

namespace ml {

/**
 * An unsafe writer to arbitrary memory
 */
class SpanWriter {
public:
    SpanWriter(void* stream) {
        mStart = static_cast<char*>(stream);
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
        size_t len = std::strlen(str);
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

    void reset() {
        mCursor = mStart;
    }

    char* cursor() const {
        return mCursor;
    }

    size_t bytesWritten() const {
        return mCursor - mStart;
    }

private:
    char* mStart;
    char* mCursor;
};

}
