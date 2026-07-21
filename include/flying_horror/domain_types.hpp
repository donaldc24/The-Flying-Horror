#pragma once

namespace flying_horror {
    struct Vector3D {
        double x{0.0};
        double y{0.0};
        double z{0.0};
    };

    struct Attitude {
        double rollRadians{0.0};
        double pitchRadians{0.0};
        double yawRadians{0.0};
    };

    struct AngularVelocity {
        double rollRadiansPerSecond{0.0};
        double pitchRadiansPerSecond{0.0};
        double yawRadiansPerSecond{0.0};
    };

    struct ControlCommand {
        double throttle{0.0};
        double desiredRollRadians{0.0};
        double desiredPitchRadians{0.0};
        double desiredYawRateRadiansPerSecond{0.0};
    };

    struct MotorOutputs {
        double frontLeft{0.0};
        double frontRight{0.0};
        double rearLeft{0.0};
        double rearRight{0.0};
    };

    struct SimulationState {
        Attitude attitude{};
        AngularVelocity angularVelocity{};
        MotorOutputs motorOutputs{};
        double elapsedTimeSeconds{0.0};
    };

}