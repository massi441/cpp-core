#pragma once

#include <bit>

#include "Core/Container/Array.h"
#include "Core/Container/ArrayPoolConfig.h"
#include "Core/Container/ArrayStack.h"
#include "Core/Container/Pool.h"
#include "Core/Util/MathHelpers.h"

namespace ml {

/**
 * A pool of reusable Array's with varying sizes
 * @tparam T The type stored by the array's
 */
template <typename T>
class ArrayPool : public Pool<Array<T>*> {
public:
    explicit ArrayPool() : ArrayPool(ArrayPoolConfig()) {}

    explicit ArrayPool(ArrayPoolConfig config) {
        mConfig.arrayMinSize = mathul::max(1, config.arrayMinSize);
        mConfig.totalBuckets = mathul::max(1, config.totalBuckets);
        mConfig.bucketSize = mathul::max(1, config.bucketSize);

        mBuckets = ml::Array<Bucket>(mConfig.totalBuckets);

        for (Bucket& bucket : mBuckets) {
            bucket = Bucket(mConfig.bucketSize);
        }
    }

    /**
     * Attempts to rent a fixed array of the nearest requested size from the current pool.
     * If no fixed array is found, a fallback array is allocated on the heap
     * @param requestedSize The minimum size of the fixed array
     * @return A pointer to a fixed array, that can later be returned to the pool
     */
    ml::Array<T>* rent(size_t requestedSize) override {
        uint64_t bufferSize = this->calcNearestBucketSize(requestedSize);
        uint64_t bucketIndex = this->calcBucketIndex(bufferSize);

        if (!mBuckets.isInBounds(bucketIndex)) {
            return new Array<T>(bufferSize);
        }

        Bucket& bucket = mBuckets[bucketIndex];

        ml::Array<T>* buffer;
        if (bucket.pop(&buffer)) {
            return buffer;
        }

        return new Array<T>(bufferSize);
    }

    /**
     * Attempts to rent a array of the nearest requested size from the current pool,
     * falling back to larger buckets up to maxOverflowCount before allocating on the heap.
     * @param requestedSize The minimum size of the fixed array
     * @param maxOverflowCount The maximum number of larger buckets to try before heap allocating
     * @return A pointer to a fixed array, that can later be returned to the pool
     */
    ml::Array<T>* rent(size_t requestedSize, byte maxOverflowCount) override {
        uint64_t bucketSize = this->calcNearestBucketSize(requestedSize);
        uint64_t bucketIndex = this->calcBucketIndex(bucketSize);

        if (!mBuckets.isInBounds(bucketIndex)) {
            return new Array<T>(bucketSize);
        }

        uint64_t maxIndex = mathul::min(bucketIndex + maxOverflowCount, mBuckets.maxIndex());

        for (uint64_t i = bucketIndex; i <= maxIndex; i++) {
            Bucket& bucket = mBuckets[i];

            ml::Array<T>* buffer;
            if (bucket.pop(&buffer)) {
                return buffer;
            }
        }

        return new Array<T>(bucketSize);
    }

    /**
     * Returns a previously rented array back to the pool.
     * If the corresponding bucket is full or the buffer size is out of pool range, the buffer is deleted.
     * @param buffer The buffer to return
     * @return True if the buffer was successfully returned to the pool, false if it was deleted
     */
    bool release(ml::Array<T>* buffer) override {
        uint64_t bucketIndex = this->calcBucketIndex(buffer->size());
        if (!mBuckets.isInBounds(bucketIndex)) {
            delete buffer;
            return false;
        }

        Bucket& bucket = mBuckets[bucketIndex];
        if (bucket.push(buffer)) {
            return true;
        }

        delete buffer;
        return false;
    }

    ~ArrayPool() override {
        for (Bucket& bucket : mBuckets) {
            ml::Array<T>* buffer;
            while (bucket.pop(&buffer)) {
                delete buffer;
            }
        }
    }

private:
    using Bucket = ml::ArrayStack<ml::Array<T>*>;

private:
    ArrayPoolConfig mConfig;
    ml::Array<Bucket> mBuckets;

    uint64_t calcNearestBucketSize(size_t requestedSize) {
        if (requestedSize <= mConfig.arrayMinSize) {
            return mConfig.arrayMinSize;
        }

        return std::bit_ceil(requestedSize);
    }

    /**
     * Returns the bucket index of a normalized size for the array pool
     * @param size The size, normalized to an exponent of 2
     * @return The index of the bucket containing arrays of the requested size
     */
    uint64_t calcBucketIndex(size_t size) const {
        return std::bit_width(size) - std::bit_width(mConfig.arrayMinSize);
    }
};

}
