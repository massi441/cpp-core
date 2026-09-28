#pragma once

#include "Core/Util/Types.h"

namespace ml {

struct ArrayPoolConfig {
    /**
     * The size of the smallest array in the array pool
     */
    size_t arrayMinSize = 16;

    /**
     * The total amount of array buckets to use.
     * Each buckets holds slots for arrays of a specific size
     */
    ushort totalBuckets = 12;

    /**
     * The amount of arrays inside each array bucket
     */
    ushort bucketSize = 3;

    ArrayPoolConfig& withMinSize(size_t size) {
        arrayMinSize = size;
        return *this;
    }

    ArrayPoolConfig& withBuckets(ushort buckets) {
        totalBuckets = buckets;
        return *this;
    }

    ArrayPoolConfig& withBucketSize(ushort size) {
        bucketSize = size;
        return *this;
    }
};

}
