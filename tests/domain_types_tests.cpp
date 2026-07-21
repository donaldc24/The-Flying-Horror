#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include <flying_horror/domain_types.hpp>

using Catch::Matchers::WithinAbs;

TEST_CASE("Attitude defaults to level orientation", "[domain-types]") {
    const flying_horror::Attitude attitude{};

    REQUIRE_THAT(
        attitude.rollRadians,
        WithinAbs(0.0, 1e-12)
    );

    REQUIRE_THAT(
        attitude.pitchRadians,
        WithinAbs(0.0, 1e-12)
    );

    REQUIRE_THAT(
        attitude.yawRadians,
        WithinAbs(0.0, 1e-12)
    );
}

TEST_CASE("Motor outputs default to stopped", "[domain-types]") {
    const flying_horror::MotorOutputs outputs{};

    REQUIRE_THAT(outputs.frontLeft, WithinAbs(0.0, 1e-12));
    REQUIRE_THAT(outputs.frontRight, WithinAbs(0.0, 1e-12));
    REQUIRE_THAT(outputs.rearLeft, WithinAbs(0.0, 1e-12));
    REQUIRE_THAT(outputs.rearRight, WithinAbs(0.0, 1e-12));
}

TEST_CASE("Simulation state begins at time zero", "[domain-types]") {
    const flying_horror::SimulationState state{};

    REQUIRE_THAT(
        state.elapsedTimeSeconds,
        WithinAbs(0.0, 1e-12)
    );
}

TEST_CASE("Attitude supports explicit aggregate initialization", "[domain-types]") {
    const flying_horror::Attitude attitude{
        .rollRadians = 0.25,
        .pitchRadians = -0.1,
        .yawRadians = 1.5
    };

    REQUIRE_THAT(
        attitude.rollRadians,
        WithinAbs(0.25, 1e-12)
    );

    REQUIRE_THAT(
        attitude.pitchRadians,
        WithinAbs(-0.1, 1e-12)
    );

    REQUIRE_THAT(
        attitude.yawRadians,
        WithinAbs(1.5, 1e-12)
    );
}