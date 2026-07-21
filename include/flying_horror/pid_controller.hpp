#pragma once

namespace flying_horror {
    class PIDController {
        public:
            PIDController(
                double kp,
                double ki,
                double kd,
                double minimumOutput = -1.0,
                double maximumOutput = 1.0,
                double minimumIntegral = -1.0,
                double maximumIntegral = 1.0
            );

            [[nodiscard]]
            double update(
                double target,
                double measurement,
                double deltaTimeSeconds
            );

            void reset() noexcept;

        private:
            double kp_;
            double ki_;
            double kd_;

            double minimumOutput_;
            double maximumOutput_;

            double minimumIntegral_;
            double maximumIntegral_;

            double accumulatedError_{0.0};
            double previousError_{0.0};
            bool hasPreviousError_{false};
        };
}