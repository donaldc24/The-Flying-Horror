#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <numbers>
#include <flying_horror/units.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("Zero degrees converts to zero radians", "[units]") {
    const double result =
        flying_horror::degrees_to_radians(0.0);

    REQUIRE_THAT(result, WithinAbs(0.0, 1e-12));
}

TEST_CASE("One hundred eighty degrees converts to pi radians", "[units]") {
    const double result =
        flying_horror::degrees_to_radians(180.0);

    REQUIRE_THAT(
        result,
        WithinAbs(std::numbers::pi, 1e-12)
    );
}

TEST_CASE("Negative degrees convert to negative radians", "[units]") {
    const double result =
        flying_horror::degrees_to_radians(-90.0);

    REQUIRE_THAT(
        result,
        WithinAbs(-std::numbers::pi / 2.0, 1e-12)
    );
}

TEST_CASE("Pi radians converts to one hundred eighty degrees", "[units]") {
    const double result =
        flying_horror::radians_to_degrees(std::numbers::pi);

    REQUIRE_THAT(result, WithinAbs(180.0, 1e-12));
}

TEST_CASE("Negative radians convert to negative degrees", "[units]") {
    const double result =
        flying_horror::radians_to_degrees(
            -std::numbers::pi / 2.0
        );

    REQUIRE_THAT(result, WithinAbs(-90.0, 1e-12));
}

TEST_CASE("Degree and radian conversion round trip preserves value", "[units]") {
    constexpr double originalDegrees{15.0};

    const double radians =
        flying_horror::degrees_to_radians(originalDegrees);

    const double convertedDegrees =
        flying_horror::radians_to_degrees(radians);

    REQUIRE_THAT(
        convertedDegrees,
        WithinAbs(originalDegrees, 1e-12)
    );
}