#pragma once

namespace flying_horror {
    class RollDynamics {
        public:
            static double calculate_new_angle(
                double current_angle,  // degrees
                double &angular_velocity, // rads/s
                double delta_time, // seconds
                double torque // N*m
            );
    };
}