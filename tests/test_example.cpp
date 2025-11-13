// tests/test_example.cpp
#include <catch2/catch_test_macros.hpp>
#include <MyProject/example.hpp>

TEST_CASE("Example adds numbers correctly") {
    REQUIRE(add(2, 3) == 5);
}
