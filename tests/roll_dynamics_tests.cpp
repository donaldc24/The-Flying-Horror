#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <limits>
#include <stdexcept>

#include <flying_horror/roll_dynamics.hpp>

TEST_CASE("Positive torque increases angle and angular velocity", "[roll-dynamics]") {
    double initial_angle = 0.0;
    double angular_velocity = 0.0;
    double delta_time = 1.0;
    double torque = 0.01;

    double result = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity,
        delta_time,
        torque
    );

    REQUIRE(result > 0.0);
    REQUIRE(angular_velocity > 0.0);
}

TEST_CASE("Negative torque decreases angle and angular velocity", "[roll-dynamics]") {
    double initial_angle = 0.0;
    double angular_velocity = 0.0;
    double delta_time = 1.0;
    double torque = -0.01;

    double result = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity,
        delta_time,
        torque
    );

    REQUIRE(result < 0.0);
    REQUIRE(angular_velocity < 0.0);
}

TEST_CASE("Zero torque with zero starting velocity leaves the state unchanged", "[roll-dynamics]") {
    double initial_angle = 0.0;
    double angular_velocity = 0.0;
    double delta_time = 1.0;
    double torque = 0.0;

    double result = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity,
        delta_time,
        torque
    );

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-12));
    REQUIRE_THAT(angular_velocity, Catch::Matchers::WithinAbs(0.0, 1e-12));
}

TEST_CASE("Zero torque with nonzero starting velocity allows damping to reduce the magnitude of that velocity", "[roll-dynamics]") {
    double initial_angle = 0.0;
    double angular_velocity = 0.01;
    double delta_time = 1.0;
    double torque = -0.0;

    double initial_velocity = angular_velocity;

    double result = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity,
        delta_time,
        torque
    );

    REQUIRE(angular_velocity > -initial_velocity); 
}

TEST_CASE("The same inputs always produce the same outputs", "[roll-dynamics]") {
    double initial_angle = 0.03;
    double angular_velocity1 = 0.01;
    double angular_velocity2 = 0.01;
    double delta_time = 1.0;
    double torque = 0.01;

    double result1 = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity1,
        delta_time,
        torque
    );

    double result2 = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity2,
        delta_time,
        torque
    );

    REQUIRE(result1 == result2);
    REQUIRE(angular_velocity1 == angular_velocity2);
}

TEST_CASE("One timestep produces the mathematically expected angle and velocity", "[roll-dynamics]") {
    double initial_angle = 0.0;
    double angular_velocity = 0.0;
    double delta_time = 1.0;
    double torque = 0.01;

    double expected_angular_acceleration = torque / 0.0019; // Using the moment of inertia from roll_dynamics.cpp
    double expected_angular_velocity = expected_angular_acceleration * delta_time;
    double expected_angle = initial_angle + expected_angular_velocity * delta_time;

    double result = flying_horror::RollDynamics::calculate_new_angle(
        initial_angle,
        angular_velocity,
        delta_time,
        torque
    );

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(expected_angle, 1e-12));
    REQUIRE_THAT(angular_velocity, Catch::Matchers::WithinAbs(expected_angular_velocity, 1e-12));
}