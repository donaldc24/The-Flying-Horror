#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <limits>
#include <stdexcept>

#include <flying_horror/motor_output.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("Motor output below zero clamps to zero", "[motor-output]") {
    const double result =
        flying_horror::clamp_motor_output(-0.25);

    REQUIRE_THAT(result, WithinAbs(0.0, 1e-12));
}

TEST_CASE("Motor output above one clamps to one", "[motor-output]") {
    const double result =
        flying_horror::clamp_motor_output(1.25);

    REQUIRE_THAT(result, WithinAbs(1.0, 1e-12));
}

TEST_CASE("Valid motor output remains unchanged", "[motor-output]") {
    const double result =
        flying_horror::clamp_motor_output(0.65);

    REQUIRE_THAT(result, WithinAbs(0.65, 1e-12));
}

TEST_CASE("Motor output boundary values remain unchanged", "[motor-output]") {
    REQUIRE_THAT(
        flying_horror::clamp_motor_output(0.0),
        WithinAbs(0.0, 1e-12)
    );

    REQUIRE_THAT(
        flying_horror::clamp_motor_output(1.0),
        WithinAbs(1.0, 1e-12)
    );
}

TEST_CASE("NaN motor output is rejected", "[motor-output]") {
    const double notANumber =
        std::numeric_limits<double>::quiet_NaN();

    REQUIRE_THROWS_AS(
        flying_horror::clamp_motor_output(notANumber),
        std::invalid_argument
    );
}

TEST_CASE("Infinite motor output is rejected", "[motor-output]") {
    const double infinity =
        std::numeric_limits<double>::infinity();

    REQUIRE_THROWS_AS(
        flying_horror::clamp_motor_output(infinity),
        std::invalid_argument
    );
}