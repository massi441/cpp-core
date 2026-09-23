#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>

#include "Core/Container/Array.h"

TEST_CASE("Creates an array of given size") {
    // Arrange
    uint64_t size = GENERATE(1, 3, 5, 12);

    // Act
    ml::Array<int> array = ml::Array<int>(size);

    // Assert
    REQUIRE(array.size() == size);
}
