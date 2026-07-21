#include <flying_horror/roll_dynamics.hpp>

namespace flying_horror {
    const double ROLLING_MOMENT_OF_INERTIA = 0.0019; // kg*m^2
    const double DAMPING_COEFFICIENT = 0.0035;    // D in N*m*s/rad

    double RollDynamics::calculate_new_angle(double current_angle, double &angular_velocity, double delta_time, double torque) {
        double damping_torque = DAMPING_COEFFICIENT * angular_velocity;
        double net_torque = torque - damping_torque;

        double angular_acceleration = net_torque / ROLLING_MOMENT_OF_INERTIA;
        angular_velocity += angular_acceleration * delta_time;

        current_angle += angular_velocity * delta_time;
        return current_angle;
    }
}