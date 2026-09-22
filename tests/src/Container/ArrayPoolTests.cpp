#include "Core/Container/ArrayPool.h"

#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

namespace ml {

TEST_CASE("Sanitizes config to at least one") {
    // Arrange
    ml::ArrayPoolConfig config = ml::ArrayPoolConfig
    {
        .arrayMinSize = 0,
        .totalBuckets = 0,
        .bucketSize = 0
    };

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>(config);

    // Act
    ml::Array<byte>* rentedBuffer = pool.rent(0);
    ml::Array<byte>* previousBuffer = rentedBuffer;

    bool released = pool.release(rentedBuffer);
    rentedBuffer = pool.rent(0);

    // Assert
    REQUIRE(previousBuffer != nullptr);
    REQUIRE(previousBuffer->size() == 1); // arrayMinSize clamped to 1
    REQUIRE(released); // bucketSize clamped to 1, so the bucket has a slot
    REQUIRE(rentedBuffer == previousBuffer); // totalBuckets clamped to 1, so the bucket exists

    pool.release(rentedBuffer);
}

TEST_CASE("Rents buffer of nearest bucket size") {
    // Arrange
    struct NearestSizeTest {
        size_t requestedSize;
        size_t expectedSize;
    };

    NearestSizeTest testCase = GENERATE(
        NearestSizeTest{ .requestedSize = 5, .expectedSize = 16 }, // below min size
        NearestSizeTest{ .requestedSize = 16, .expectedSize = 16 }, // exactly min size
        NearestSizeTest{ .requestedSize = 17, .expectedSize = 32 },
        NearestSizeTest{ .requestedSize = 35, .expectedSize = 64 },
        NearestSizeTest{ .requestedSize = 64, .expectedSize = 64 }, // exactly a power of two
        NearestSizeTest{ .requestedSize = 210, .expectedSize = 256 }
    );

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>();

    // Act
    ml::Array<byte>* rentedBuffer = pool.rent(testCase.requestedSize);

    // Assert
    REQUIRE(rentedBuffer != nullptr);
    REQUIRE(rentedBuffer->size() == testCase.expectedSize);

    pool.release(rentedBuffer);
}

TEST_CASE("Reuses same buffer after releasing") {
    // Arrange
    size_t minimumSize = GENERATE(10, 28, 55, 210);

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>();

    // Act
    ml::Array<byte>* rentedBuffer = pool.rent(minimumSize);
    ml::Array<byte>* previousBuffer = rentedBuffer;

    pool.release(rentedBuffer);
    rentedBuffer = pool.rent(minimumSize);

    // Assert
    REQUIRE(previousBuffer != nullptr);
    REQUIRE(rentedBuffer == previousBuffer);

    pool.release(rentedBuffer);
}

TEST_CASE("Does not reuse same buffer while rented") {
    // Arrange
    size_t minimumSize = GENERATE(10, 28, 55, 210);

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>();

    // Act
    ml::Array<byte>* firstBuffer = pool.rent(minimumSize);
    ml::Array<byte>* secondBuffer = pool.rent(minimumSize);

    // Assert
    REQUIRE(firstBuffer != nullptr);
    REQUIRE(secondBuffer != nullptr);
    REQUIRE(firstBuffer != secondBuffer);

    pool.release(firstBuffer);
    pool.release(secondBuffer);
}

TEST_CASE("Release fails when bucket is full") {
    // Arrange
    ml::ArrayPoolConfig config = ml::ArrayPoolConfig
    {
        .arrayMinSize = 16,
        .totalBuckets = 4,
        .bucketSize = 1
    };

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>(config);

    ml::Array<byte>* firstBuffer = pool.rent(16);
    ml::Array<byte>* secondBuffer = pool.rent(16);

    // Act
    bool firstReleased = pool.release(firstBuffer);
    bool secondReleased = pool.release(secondBuffer);

    // Assert
    REQUIRE(firstReleased);
    REQUIRE_FALSE(secondReleased);
}

TEST_CASE("Release fails for size outside pool range") {
    // Arrange
    ml::ArrayPoolConfig config = ml::ArrayPoolConfig
    {
        .arrayMinSize = 2,
        .totalBuckets = 3, // holds sizes 2, 4, 8
        .bucketSize = 1
    };

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>(config);

    ml::Array<byte>* rentedBuffer = pool.rent(16);

    // Act
    bool released = pool.release(rentedBuffer);

    // Assert
    REQUIRE_FALSE(released);
}

TEST_CASE("Uses overflown buffer") {
    // Arrange
    struct OverflowTest {
        ml::ArrayPoolConfig config;
        size_t requestedSize;
        byte maxOverflow;
        size_t prefilledSize;
        size_t expectedBufferSize;
    };

    OverflowTest testCase = GENERATE(
        OverflowTest{ .config = { .arrayMinSize = 2, .totalBuckets = 3, .bucketSize = 1 }, .requestedSize = 2, .maxOverflow = 1, .prefilledSize = 4, .expectedBufferSize = 4 },
        OverflowTest{ .config = { .arrayMinSize = 2, .totalBuckets = 3, .bucketSize = 1 }, .requestedSize = 2, .maxOverflow = 2, .prefilledSize = 8, .expectedBufferSize = 8 },
        OverflowTest{ .config = { .arrayMinSize = 2, .totalBuckets = 3, .bucketSize = 1 }, .requestedSize = 2, .maxOverflow = 1, .prefilledSize = 8, .expectedBufferSize = 2 }, // too far to overflow
        OverflowTest{ .config = { .arrayMinSize = 2, .totalBuckets = 3, .bucketSize = 1 }, .requestedSize = 2, .maxOverflow = 0, .prefilledSize = 4, .expectedBufferSize = 2 } // no overflow allowed
    );

    ml::ArrayPool<byte> pool = ml::ArrayPool<byte>(testCase.config);

    ml::Array<byte>* prefilled = pool.rent(testCase.prefilledSize);
    pool.release(prefilled);

    // Act
    ml::Array<byte>* rentedBuffer = pool.rent(testCase.requestedSize, testCase.maxOverflow);

    // Assert
    REQUIRE(rentedBuffer != nullptr);
    REQUIRE(rentedBuffer->size() == testCase.expectedBufferSize);

    pool.release(rentedBuffer);
}

}
