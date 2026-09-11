#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <stdexcept>
#include "math_functions.h"

TEST_CASE("Square root of positive numbers", "[math]") {
    REQUIRE(compute_square_root(25.0) == 5.0);
    REQUIRE(compute_square_root(0.0) == 0.0);
}

TEST_CASE("Square root of negative numbers", "[math]") {
    REQUIRE_THROWS(compute_square_root(-1.0));  // Error case
    REQUIRE_THROWS_AS(compute_square_root(-4.0), std::domain_error);
}

TEST_CASE("Square root of non-perfect square", "[math]") {
    REQUIRE(compute_square_root(2.0) == Catch::Approx(1.41421356237).epsilon(0.001));
}
