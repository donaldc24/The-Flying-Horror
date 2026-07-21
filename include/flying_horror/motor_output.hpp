#pragma once

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace flying_horror {

[[nodiscard]]
inline double clamp_motor_output(double output) {
    if (!std::isfinite(output)) {
        throw std::invalid_argument{
            "Motor output must be a finite number"
        };
    }

    return std::clamp(output, 0.0, 1.0);
}

}