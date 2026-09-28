#pragma once

#include <cstddef>
#include <cstdint>

#include "Core/Util/Types.h"

namespace ml {

/**
 * An unsafe reader of arbitrary memory
 */
class SpanReader {
public:
    SpanReader(const void* stream) {
        mCursor = static_cast<const ml::sbyte*>(stream);
    }

    const ml::sbyte* cursor() const { return mCursor; }

    template <typename T>
    void readInto(T* outVal) {
        *outVal = *reinterpret_cast<const T*>(mCursor);
        mCursor += sizeof(T);
    }

    template <typename  T>
    T read() {
        T val = *reinterpret_cast<const T*>(mCursor);
        mCursor += sizeof(T);
        return val;
    }

    template <typename T>
    void readDeserializable(T& deserializable) {
        deserializable.deserialize(this);
    }

    template <typename T>
    T* readView(size_t count) {
        T* view = reinterpret_cast<T*>(mCursor);
        mCursor += count * sizeof(T);
        return view;
    }

    template <typename T>
    T* readAsRef() {
        T* outVal = reinterpret_cast<T*>(mCursor);
        mCursor += sizeof(T);
        return outVal;
    }

    template <typename T>
    void skip() {
        mCursor += sizeof(T);
    }

    void skip(uint byteCount) {
        mCursor += byteCount;
    }

    byte readByte() { return this->read<byte>(); }
    sbyte readSByte() { return this->read<sbyte>(); }

    ushort readUInt16() { return this->read<ushort>(); }
    short readInt16() { return this->read<short>(); }

    int readInt32() { return this->read<int>(); }
    uint32_t readUInt32() { return this->read<uint32_t>(); }

    float readSingle() { return this->read<float>(); }

    int64_t readInt64() { return this->read<int64_t>(); }
    uint64_t readUInt64() { return this->read<uint64_t>(); }

private:
    const ml::sbyte* mCursor;
};

}
