#include <flying_horror/pid_controller.hpp>

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace flying_horror {
    PIDController::PIDController(double kp, double ki, double kd, double minimumOutput, double maximumOutput, double minimumIntegral, double maximumIntegral): 
        kp_{kp},
        ki_{ki},
        kd_{kd},
        minimumOutput_{minimumOutput},
        maximumOutput_{maximumOutput},
        minimumIntegral_{minimumIntegral},
        maximumIntegral_{maximumIntegral}
    {
        if (!std::isfinite(kp_) ||
            !std::isfinite(ki_) ||
            !std::isfinite(kd_)) {
            throw std::invalid_argument{
                "PID gains must be finite"
            };
        }

        if (minimumOutput_ > maximumOutput_) {
            throw std::invalid_argument{
                "Minimum output cannot exceed maximum output"
            };
        }

        if (minimumIntegral_ > maximumIntegral_) {
            throw std::invalid_argument{
                "Minimum integral cannot exceed maximum integral"
            };
        }
    }

    double PIDController::update(double target, double measurement, double deltaTimeSeconds) {
        if (!std::isfinite(target) ||
            !std::isfinite(measurement)) {
            throw std::invalid_argument{
                "Target and measurement must be finite"
            };
        }

        if (!std::isfinite(deltaTimeSeconds) ||
            deltaTimeSeconds <= 0.0) {
            throw std::invalid_argument{
                "Delta time must be positive and finite"
            };
        }

        const double error = target - measurement;
        const double proportionalTerm = kp_ * error;
        accumulatedError_ += error * deltaTimeSeconds;

        accumulatedError_ = std::clamp(
            accumulatedError_,
            minimumIntegral_,
            maximumIntegral_
        );

        const double integralTerm = ki_ * accumulatedError_;
        double derivativeTerm{0.0};

        if (hasPreviousError_) {
            const double errorChange = error - previousError_;

            derivativeTerm = kd_ * errorChange / deltaTimeSeconds;
        }

        previousError_ = error;
        hasPreviousError_ = true;

        const double rawOutput = proportionalTerm + integralTerm + derivativeTerm;

        return std::clamp(
            rawOutput,
            minimumOutput_,
            maximumOutput_
        );
    }

    void PIDController::reset() noexcept {
        accumulatedError_ = 0.0;
        previousError_ = 0.0;
        hasPreviousError_ = false;
    }
}