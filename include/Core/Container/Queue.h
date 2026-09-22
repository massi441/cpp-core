#pragma once

#include <utility>

#include "Core/Container/Array.h"

namespace ml {

/**
 * A queue backed by a ring array
 * @tparam T The type of element stored in the queue
 */
template <typename T>
class Queue {
public:
    Queue() = default;

    explicit Queue(uint64_t size) {
        mArray = ml::Array<T>(size);
        mCount = 0;
    }

    bool dequeue(T* outItem) {
        if (this->isEmpty()) {
            return false;
        }

        *outItem = std::move(mArray[mHead]);

        mHead = this->calcIndex(mHead + 1);
        mCount--;

        return true;
    }

    template <typename F>
    bool enqueue(F&& item) {
        if (this->isFull()) {
            return false;
        }

        mArray[this->calcIndex(mHead + mCount)] = std::forward<F>(item);
        mCount++;

        return true;
    }

    /**
     * Resets the queue and resets all currently occupied slots
     */
    void reset() requires std::is_default_constructible_v<T> {
        if constexpr (!std::is_trivially_destructible_v<T>) {
            for (uint64_t i = 0; i < mCount; i++) {
                mArray[this->calcIndex(mHead + i)] = T();
            }
        }

        mHead = 0;
        mCount = 0;
    }

    bool isEmpty() const {
        return mCount == 0;
    }

    bool isFull() const {
        return mCount == mArray.size();
    }

    uint64_t count() const {
        return mCount;
    }

    uint64_t size() const {
        return mCount;
    }

    uint64_t capacity() const {
        return mArray.size();
    }

private:
    ml::Array<T> mArray;
    uint64_t mHead = 0;
    uint64_t mCount = 0;

    uint64_t calcIndex(uint64_t index) const {
        if (index >= mArray.size()) {
            index -= mArray.size();
        }

        return index;
    }
};

template <typename T>
using RingQueue = ml::Queue<T>;

}
