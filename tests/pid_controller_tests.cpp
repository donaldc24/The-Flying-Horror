#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <limits>
#include <stdexcept>

#include <flying_horror/pid_controller.hpp>

TEST_CASE("Zero Error produces zero output", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 1.0, 1.0);

    const double result = pid_controller.update(1.0, 1.0, 10.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-12));
}

TEST_CASE("Positive Error produces positive output", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 0.0, 0.0);

    const double result = pid_controller.update(2.0, 1.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-12));
    REQUIRE(result > 0.0);
}

TEST_CASE("Negative Error produces negative output", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 0.0, 0.0, -100.0, 100.0);

    const double result = pid_controller.update(1.0, 2.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-1.0, 1e-12));
    REQUIRE(result < 0.0);
}

TEST_CASE("Output is clamped to maximum output", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 0.0, 0.0, -1.0, 1.0);

    const double result = pid_controller.update(10.0, 1.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-12));
}

TEST_CASE("Output is clamped to minimum output", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 0.0, 0.0, -1.0, 1.0);

    const double result = pid_controller.update(-10.0, 1.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-1.0, 1e-12));
}

TEST_CASE("Integral accumulates across repeated calls", "[pid-controller]") {
    flying_horror::PIDController pid_controller(0.0, 1.0, 0.0, -100.0, 100.0, -100.0, 100.0);

    const double result1 = pid_controller.update(1.0, 0.0, 1.0);
    const double result2 = pid_controller.update(1.0, 0.0, 1.0);

    REQUIRE_THAT(result1, Catch::Matchers::WithinAbs(1.0, 1e-12));
    REQUIRE_THAT(result2, Catch::Matchers::WithinAbs(2.0, 1e-12));
}

TEST_CASE("Integral is clamped to maximum integral", "[pid-controller]") {
    flying_horror::PIDController pid_controller(0.0, 1.0, 0.0, -100.0, 100.0, -10.0, 10.0);

    const double result = pid_controller.update(20.0, 0.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(10.0, 1e-12));
}

TEST_CASE("Integral is clamped to minimum integral", "[pid-controller]") {
    flying_horror::PIDController pid_controller(0.0, 1.0, 0.0, -100.0, 100.0, -10.0, 10.0);

    const double result = pid_controller.update(-20.0, 0.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(-10.0, 1e-12));
}

TEST_CASE("Reset clears accumulated integral error","[pid-controller]") {
    flying_horror::PIDController pid_controller{0.0, 1.0, 0.0, -100.0, 100.0, -100.0, 100.0
    };

    pid_controller.update(1.0, 0.0, 1.0);
    pid_controller.update(1.0, 0.0, 1.0);

    pid_controller.reset();

    const double result =
        pid_controller.update(1.0, 0.0, 1.0);

    // If reset worked, the integral begins again at 1 rather than 3.
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(1.0, 1e-12));
}

TEST_CASE("Reset clears derivative history", "[pid-controller]") {
    flying_horror::PIDController controller{0.0, 0.0, 1.0, -100.0, 100.0};

    controller.update(1.0, 0.0, 1.0);
    controller.update(3.0, 0.0, 1.0);

    controller.reset();

    const double result = controller.update(3.0, 0.0, 1.0);

    // This is the first sample after reset, so D must be zero.
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-12));
}

TEST_CASE("Derivative responds to changing error", "[pid-controller]") {
    flying_horror::PIDController controller{0.0, 0.0, 1.0, -100.0, 100.0};

    controller.update(1.0, 0.0, 1.0);

    const double result = controller.update(3.0, 0.0, 1.0);

    // Error changed from 1 to 3 over one second.
    // D = 1 × (3 - 1) / 1 = 2.
    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(2.0, 1e-12));
}

TEST_CASE("Zero timestep is rejected", "[pid-controller]") {
    flying_horror::PIDController controller{1.0, 1.0, 1.0};

    REQUIRE_THROWS_AS(controller.update(1.0, 1.0, 0.0), std::invalid_argument);
}

TEST_CASE("Negative timestep is rejected", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 1.0, 1.0);

    REQUIRE_THROWS_AS(pid_controller.update(1.0, 1.0, -1.0), std::invalid_argument);
}

TEST_CASE("NaN timestep is rejected", "[pid-controller]") {
    flying_horror::PIDController pid_controller(1.0, 1.0, 1.0);

    const double nan = std::numeric_limits<double>::quiet_NaN();
    REQUIRE_THROWS_AS(pid_controller.update(1.0, 1.0, nan), std::invalid_argument);
}

TEST_CASE("First update does not produce derivative spike", "[pid-controller]") {
    flying_horror::PIDController pid_controller(0.0, 0.0, 1.0, -100.0, 100.0);

    const double result = pid_controller.update(1.0, 0.0, 1.0);

    REQUIRE_THAT(result, Catch::Matchers::WithinAbs(0.0, 1e-12));
}