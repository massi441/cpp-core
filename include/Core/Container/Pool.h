#pragma once

#include <cstddef>

#include "Core/Util/Types.h"

namespace ml {

/**
 * A pool for renting and reusing objects, allowing to reduce memory allocations.
 * @tparam TPoolType The type of element stored by the pool (an Array, a connection...)
 */
template <typename TPoolType>
class Pool {
public:
    virtual TPoolType rent(size_t requestedSize) = 0;
    virtual TPoolType rent(size_t requestedSize, byte maxOverflowCount) = 0;
    virtual bool release(TPoolType buffer) = 0;
    virtual ~Pool() {}
};

}
