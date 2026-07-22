#include <flying_horror/sim.hpp>
#include <flying_horror/motor_output.hpp>
#include <flying_horror/roll_dynamics.hpp>
#include <flying_horror/units.hpp>
#include <flying_horror/pid_controller.hpp>
#include <flying_horror/utils.hpp>

#include <iostream>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <cstdlib>

namespace flying_horror {
    const double update_freq = 100.0; // Hz, simulation update frequency
    const double sim_duration = 5.0; // seconds, total simulation duration
    const double initial_angle = 15.0;   // Started at 15 degrees
    const double target_angle = 0.0;     // Target is 0 degrees

    const double initial_offset = initial_angle - target_angle;
    const double settling_tolerance = degrees_to_radians(1.0);

    const double kp = 0.2; // Proportional gain
    const double ki = 0.01; // Integral gain
    const double kd = 0.035; // Derivative gain

    void init_sim() {
        std::cout << "Flying Horror simulation initialized.\n";
       std::filesystem::remove("results/output.csv");

        std::ofstream myFile("results/output.csv", std::ios::app);

        if (!myFile.is_open()) {
            std::cerr << "Error: Could not open or create the file." << std::endl;
            return;
        }
        myFile.close();

        run_sim();

        const int result = std::system("python scripts/plot_telemetry.py");

        if (result != 0) {
            std::cerr << "Python plotting script failed.\n";
            return;
        }

        std::cout << "Plots generated successfully.\n";
    }

    void run_sim() {
        std::cout << "Running Flying Horror simulation...\n";

        double current_angle = degrees_to_radians(initial_angle);   // Started at 15 degrees
        double angular_velocity = 0.0;                     // rads/s
        double torque = 0.0; // N*m

        double maximum_overshoot = 0.0;
        double settling_time_candidate = -1.0;
        bool crossed_target = false;

        PIDController pid_controller(kp, ki, kd);

        double delta_time = 1.0 / update_freq;
        int steps = sim_duration * update_freq;
        for (int i = 0; i < steps; i++) {
            writeToCSV("Timestamp: " + std::to_string(i * delta_time), "results/output.csv");
            torque = pid_controller.update(degrees_to_radians(target_angle), current_angle, delta_time);
            current_angle = RollDynamics::calculate_new_angle(current_angle, angular_velocity, delta_time, torque);

            writeToCSV("Angular Velocity : " + std::to_string(angular_velocity), "results/output.csv");
            writeToCSV("Current Angle: " + std::to_string(radians_to_degrees(current_angle)), "results/output.csv");
            writeToCSV("Torque : " + std::to_string(torque), "results/output.csv");
            writeToCSV("-------------------------------------------------------------------", "results/output.csv");

            if (i % 10 == 0) { // Print every 10 steps
                std::cout << "Time: " << (i * delta_time) << " s, ";
                std::cout << "Angle: " << radians_to_degrees(current_angle) << " degrees, ";
                std::cout << "Angular Velocity: " << angular_velocity << " rads/s, ";
                std::cout << "Torque: " << torque << " N*m\n";
                std::cout << "----------------------------------------------------------------------------------------------------------\n";
            }

            const double elapsed_time = (i + 1) * delta_time;
            const double offset_from_target = current_angle - target_angle;

            if (!crossed_target) {
                const bool crossedFromAbove = initial_offset > 0.0 && offset_from_target <= 0.0;
                const bool crossedFromBelow = initial_offset < 0.0 && offset_from_target >= 0.0;
                crossed_target = crossedFromAbove || crossedFromBelow;
            }

            const bool isBeyondTarget = crossed_target && offset_from_target * initial_offset < 0.0;

            if (isBeyondTarget) {
                maximum_overshoot = std::max(
                    maximum_overshoot,
                    std::abs(offset_from_target)
                );
            }

            // Begin recording a possible settling time when entering
            // the tolerance band. Reset it if the response later leaves.
            if (std::abs(offset_from_target) <= settling_tolerance) {
                if (settling_time_candidate < 0.0) {
                    settling_time_candidate = elapsed_time;
                }
            } else {
                settling_time_candidate = -1.0;
            }
        }

        std::cout << "\nSimulation completed:\n";
        std::cout << "------------------------------------------------\n";
        std::cout << "Initial Angle: " << (initial_angle) << " degrees\n";
        std::cout << "Final Angle: " << radians_to_degrees(current_angle) << " degrees\n";
        std::cout << "Maximum Overshoot: " << radians_to_degrees(maximum_overshoot) << " degrees\n";
        std::cout << "Final Angular Velocity: " << angular_velocity << " rads/s\"\n";
        if (settling_time_candidate >= 0.0) {
            std::cout
                << "Settling time (+/- 1 degree): "
                << settling_time_candidate
                << " seconds\n";
        } else {
            std::cout
                << "Settling time: "
                << "Did not settle during simulation\n";
        }
    }
}