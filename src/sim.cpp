#include <flying_horror/sim.hpp>
#include <flying_horror/motor_output.hpp>
#include <flying_horror/roll_dynamics.hpp>
#include <flying_horror/units.hpp>

#include <iostream>

namespace flying_horror {
    const double torque = 0.01; // N*m, example constant torque for simulation
    const double update_freq = 100.0; // Hz, simulation update frequency
    const double sim_duration = 2.0; // seconds, total simulation duration

    void init_sim() {
        std::cout << "Flying Horror simulation initialized.\n";
        run_sim();
    }

    void run_sim() {
        std::cout << "Running Flying Horror simulation...\n";

        double current_angle = 0.0; // degrees
        double angular_velocity = 0.0; // rads/s

        int steps = sim_duration * update_freq;
        for (int i = 0; i < steps; i++) {
            double delta_time = 1.0 / update_freq;
            current_angle = RollDynamics::calculate_new_angle(current_angle, angular_velocity, delta_time, torque);
            std::cout << "Angle: " << radians_to_degrees(current_angle) << " degrees, Angular Velocity: " << angular_velocity << " rads/s\n";
        }
    }
}