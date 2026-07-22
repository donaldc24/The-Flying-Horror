from pathlib import Path

import matplotlib.pyplot as plt


# Change this to the name or path of your simulation output file.
FILE_PATH = Path("results/output.csv")


def read_simulation_data(file_path: Path):
    """Read the simulation log and return seven lists of floating-point values."""

    timestamps = []
    proportional_terms = []
    integral_terms = []
    derivative_terms = []
    angular_velocities = []
    current_angles = []
    torques = []

    with file_path.open("r", encoding="utf-8") as file:
        for line_number, line in enumerate(file, start=1):
            line = line.strip()

            # Ignore blank lines and divider lines.
            if not line or line.startswith("---"):
                continue

            if ":" not in line:
                print(f"Skipping malformed line {line_number}: {line}")
                continue

            label, value_text = line.split(":", maxsplit=1)

            label = label.strip()
            value_text = value_text.strip()

            try:
                value = float(value_text)
            except ValueError:
                print(
                    f"Skipping line {line_number}: "
                    f"could not convert '{value_text}' to a number"
                )
                continue

            if label == "Timestamp":
                timestamps.append(value)

            elif label == "Proportional Term":
                proportional_terms.append(value)

            elif label == "Integral Term":
                integral_terms.append(value)

            elif label == "Derivative Term":
                derivative_terms.append(value)

            elif label == "Angular Velocity":
                angular_velocities.append(value)

            elif label == "Current Angle":
                current_angles.append(value)

            elif label == "Torque":
                torques.append(value)

            else:
                print(f"Skipping unknown label on line {line_number}: {label}")

    return (
        timestamps,
        proportional_terms,
        integral_terms,
        derivative_terms,
        angular_velocities,
        current_angles,
        torques,
    )


def validate_data(
    timestamps,
    proportional_terms,
    integral_terms,
    derivative_terms,
    angular_velocities,
    current_angles,
    torques,
):
    """Verify that every list contains the same number of samples."""

    lengths = {
        "Timestamps": len(timestamps),
        "Proportional terms": len(proportional_terms),
        "Integral terms": len(integral_terms),
        "Derivative terms": len(derivative_terms),
        "Angular velocities": len(angular_velocities),
        "Current angles": len(current_angles),
        "Torques": len(torques),
    }

    print("Samples read:")

    for name, length in lengths.items():
        print(f"  {name}: {length}")

    if len(set(lengths.values())) != 1:
        raise ValueError(
            "The data lists have different lengths. "
            "The input file may contain an incomplete simulation record."
        )

    if len(timestamps) == 0:
        raise ValueError("No simulation data was found in the file.")


def plot_roll_angle(timestamps, current_angles):
    """Plot roll angle over time."""

    plt.figure(figsize=(10, 6))

    plt.plot(
        timestamps,
        current_angles,
        label="Roll angle",
    )

    # Target angle appears to be zero degrees.
    plt.axhline(
        y=0,
        linestyle="--",
        label="Target angle",
    )

    plt.title("Roll Angle Over Time")
    plt.xlabel("Time (seconds)")
    plt.ylabel("Roll Angle (degrees)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    # Optional: save the graph as an image.
    plt.savefig("roll_angle_over_time.png", dpi=300)


def plot_controller_output(timestamps, torques):
    """Plot the PID controller's torque output over time."""

    plt.figure(figsize=(10, 6))

    plt.plot(
        timestamps,
        torques,
        label="Controller output",
    )

    plt.axhline(
        y=0,
        linestyle="--",
    )

    plt.title("Controller Output Over Time")
    plt.xlabel("Time (seconds)")
    plt.ylabel("Torque (N·m)")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig("controller_output_over_time.png", dpi=300)


def plot_pid_components(
    timestamps,
    proportional_terms,
    integral_terms,
    derivative_terms,
):
    """Plot the proportional, integral, and derivative terms together."""

    plt.figure(figsize=(10, 6))

    plt.plot(
        timestamps,
        proportional_terms,
        label="Proportional term",
    )

    plt.plot(
        timestamps,
        integral_terms,
        label="Integral term",
    )

    plt.plot(
        timestamps,
        derivative_terms,
        label="Derivative term",
    )

    plt.axhline(
        y=0,
        linestyle="--",
    )

    plt.title("PID Components Over Time")
    plt.xlabel("Time (seconds)")
    plt.ylabel("PID Contribution")
    plt.grid(True)
    plt.legend()
    plt.tight_layout()

    plt.savefig("pid_components_over_time.png", dpi=300)


def main():
    if not FILE_PATH.exists():
        raise FileNotFoundError(
            f"Could not find '{FILE_PATH}'. "
            "Place the data file next to this Python script or update FILE_PATH."
        )

    (
        timestamps,
        proportional_terms,
        integral_terms,
        derivative_terms,
        angular_velocities,
        current_angles,
        torques,
    ) = read_simulation_data(FILE_PATH)

    validate_data(
        timestamps,
        proportional_terms,
        integral_terms,
        derivative_terms,
        angular_velocities,
        current_angles,
        torques,
    )

    plot_roll_angle(
        timestamps,
        current_angles,
    )

    plot_controller_output(
        timestamps,
        torques,
    )

    plot_pid_components(
        timestamps,
        proportional_terms,
        integral_terms,
        derivative_terms,
    )

    # Displays all three figure windows.
    plt.show()


if __name__ == "__main__":
    main()