#include <catch2/catch_test_macros.hpp>

#include "Core/Memory/SpanWriter.h"
#include "Core/String/StringUtil.h"

TEST_CASE("Writes 3 Integers") {
    // Arrange
    int nums[3] = {};

    int num1 = 1;
    int num2 = 5;
    int num3 = 10;

    ml::SpanWriter writer = ml::SpanWriter(nums);

    // Act
    writer.write(num1);
    writer.write(num2);
    writer.write(num3);

    // Assert
    REQUIRE(nums[0] == num1);
    REQUIRE(nums[1] == num2);
    REQUIRE(nums[2] == num3);
}

TEST_CASE("Writes values into a struct") {
    // Arrange
    struct Person { int age; float balance; char name[11]; };

    int age = 10;
    float balance = 1000.0f;
    const char* name = "Alessandro"; // 10 letters, + 1 for the null terminator in the buffer

    Person person = Person();
    ml::SpanWriter writer = ml::SpanWriter(&person);

    // Act
    writer.write(age);
    writer.write(balance);
    writer.write(name, std::strlen(name) + 1);

    // Assert
    REQUIRE(person.age == age);
    REQUIRE(person.balance == balance);
    REQUIRE(ml::streql(person.name, name));
}
