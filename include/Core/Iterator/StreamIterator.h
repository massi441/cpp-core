#pragma once

#include "Core/Util/Types.h"
#include "cstdint"
#include <concepts>

namespace ml {

template <typename T>
concept StreamIteratorNode = requires(T& t, const void* src) {
    { t.deserialize(src) } -> std::same_as<const void*>;
};

/**
 * An iterator for a raw memory stream
 * @tparam T The type of element in the stream
 */
template <StreamIteratorNode T>
class StreamIterator {
public:
    explicit StreamIterator(const void* start, uint count) {
        mCursor = start;
        mCount = count;
    }

    bool deserializeNext(T* outItem) {
        if (mCount == 0) {
            return false;
        }

        mCount--;
        mCursor = outItem->deserialize(mCursor);

        return true;
    }

private:
    const void* mCursor;
    uint64_t mCount;
};

template <typename T>
class StreamIterableView {
public:

private:

};

}
