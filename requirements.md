# Software Requirements Specification

## 1. Project Overview

### 1.1 Project Name

The Flying Horror: Quadcopter Flight-Control Reconstruction

### 1.2 Purpose

This project is aimed to reconstruct the flight-control software for a previously built quadcopter project from my college dats. The original source code was lost, so this will be recreated using modern C++ and simulation-based testing.

### 1.3 Scope

**Included:**

* Single-axis roll simulation
* PID-based stabilization
* Telemetry output
* Automated tests

**Not included in the initial version:**

* Physical hardware deployment
* Full three-dimensional flight
* ROS 2 integration
* PX4 or Gazebo integration
* Autonomous navigation

---

## 2. Background

### 2.1 Original Hardware

* Arduino Uno
* GY-521 MPU-6050 IMU
* Four brushless motors
* Four electronic speed controllers
* Custom frame designed in solidworks (see solidwork designs for refrence)
* Battery and power-distribution hardware

### 2.2 Reconstruction Notice

> The original experimental software was not preserved. The software in this repository is a later reconstruction based on the original hardware design and intended behavior. Simulation results are not results from the original physical quadcopter.

---

## 3. System Goals

The system should:

1. Simulate the roll motion of a quadcopter.
2. Accept a desired roll angle.
3. Calculate corrective output using a PID controller.
4. Return the simulated vehicle toward the desired angle.
5. Record telemetry for analysis.
6. Handle invalid inputs safely.
7. Produce repeatable results.

---

## 4. Functional Requirements

### FR-1: Simulation Initialization

The system shall allow the simulation to start with a configurable initial roll angle.

**Example:**

```text
Initial roll angle: 15 degrees
Initial angular velocity: 0 degrees per second
```

### FR-2: Target Angle

The system shall allow the user to configure a target roll angle.

The default target shall be zero degrees.

### FR-3: PID Controller

The system shall calculate corrective output using configurable proportional, integral, and derivative gains.

```text
Kp
Ki
Kd
```

### FR-4: Physics Update

The system shall update:

* Roll angle
* Angular velocity
* Angular acceleration

at each simulation timestep.

### FR-5: Output Limits

The controller output shall remain within configurable minimum and maximum limits.

### FR-6: Telemetry

The system shall record the following values for each simulation step:

* Timestamp
* Target roll angle
* Actual roll angle
* Angular velocity
* Controller output
* Proportional term
* Integral term
* Derivative term

### FR-7: Simulation Completion

The system shall stop after a configurable simulation duration.

### FR-8: Result Export

The system shall export telemetry in CSV format.

### FR-9: Automated Testing

The system shall include automated tests for:

* PID calculations
* Output clamping
* Controller reset behavior
* Physics updates
* Stabilization behavior

---

## 5. Non-Functional Requirements

### NFR-1: Programming Language

The flight-control core shall be implemented using C++20 or newer.

### NFR-2: Build System

The project shall use CMake.

### NFR-3: Portability

The core software should compile on:

* Windows
* Linux

### NFR-4: Determinism

Given the same configuration and starting conditions, the simulator shall produce the same result.

### NFR-5: Testability

The PID controller and physics simulator shall be testable independently.

### NFR-6: Separation of Concerns

The flight-control core shall not depend directly on ROS 2, Gazebo, or PX4.

Future integrations shall use adapter components.

### NFR-7: Documentation

Public classes, configuration values, coordinate conventions, and measurement units shall be documented.

### NFR-8: Safety

The system shall detect and safely handle:

* Invalid timestamps
* NaN values
* Infinite values
* Invalid controller gains
* Invalid simulation parameters

---

## 6. System Inputs

| Input               | Type    | Default | Description            |
| ------------------- | ------- | ------: | ---------------------- |
| Initial roll angle  | Degrees |      15 | Starting vehicle angle |
| Target roll angle   | Degrees |       0 | Desired vehicle angle  |
| Simulation duration | Seconds |       5 | Total simulated time   |
| Timestep            | Seconds |    0.01 | Time between updates   |
| Kp                  | Number  |     TBD | Proportional gain      |
| Ki                  | Number  |     TBD | Integral gain          |
| Kd                  | Number  |     TBD | Derivative gain        |
| Maximum output      | Number  |     1.0 | Upper controller limit |
| Minimum output      | Number  |    -1.0 | Lower controller limit |

---

## 7. System Outputs

The program shall produce:

1. Console status output
2. CSV telemetry data
3. Final simulation summary
4. Success or failure status

Example summary:

```text
Simulation completed.

Initial angle: 15.00 degrees
Final angle: 0.42 degrees
Maximum overshoot: 1.30 degrees
Settling time: 2.15 seconds
Result: PASS
```

---

## 8. Success Criteria

The initial release shall be considered successful when:

* The project builds using CMake.
* All automated tests pass.
* The simulation starts at 15 degrees roll.
* The target angle is zero degrees.
* The controller returns the vehicle to within 1 degree of the target.
* The vehicle remains within 1 degree for the final second.
* The controller output remains within configured limits.
* Telemetry is exported successfully.
* The result can be reproduced from a clean checkout.

---

## 9. Constraints

* The original quadcopter hardware is no longer available.
* The original source code is no longer available.
* Initial results will be simulation-only.
* The first release will model only roll-axis movement.
* The physics model will intentionally simplify real aerodynamic behavior.

---

## 10. Assumptions

* The quadcopter is treated as a rigid body.
* Roll movement can initially be modeled independently.
* Motor response can initially be represented as direct torque.
* The simulation uses a fixed timestep.
* Environmental forces are ignored in the first release.
* Sensor values are perfect until sensor simulation is introduced later.

---

## 11. Future Requirements

Possible later additions include:

* Pitch and yaw control
* Quad-X motor mixing
* Simulated accelerometer and gyroscope
* Sensor noise and bias
* Complementary-filter attitude estimation
* Motor response delay
* Wind disturbances
* Battery-voltage simulation
* ROS 2 integration
* Gazebo integration
* PX4 integration
* Deployment to physical hardware

---

## 12. Open Questions

Track unresolved decisions here.

* What units should the internal angle representation use?
* Which C++ test framework should be used?
* How should PID gains be configured?
* What physics constants should be used initially?
* How should simulation success be measured?
* Should configuration come from command-line arguments or a file?

---

## 13. Revision History

| Version | Date       | Author         | Description                      |
| ------- | ---------- | -------------- | ----------------------------     |
| 0.0     | 2025-09-25 | Donald Calhoun | Original Physical Quadcopter     |
| 0.1     | 2026-07-20 | Donald Calhoun | Initial requirements             |
| 0.1     | 2026-07-22 | Donald Calhoun | Single-Axis Stabilization        |
