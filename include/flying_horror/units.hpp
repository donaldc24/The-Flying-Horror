#pragma once

#include <numbers>

namespace flying_horror {
    [[nodiscard]] 
    constexpr double radians_to_degrees(double radians) noexcept {
        return radians * (180.0 / std::numbers::pi);
    }

    [[nodiscard]] 
    constexpr double degrees_to_radians(double degrees) noexcept {
        return degrees * (std::numbers::pi / 180.0);
    }
}