# 🏎️ Smart RC - ADAS Simulator (Software-In-the-Loop)

![C++](https://img.shields.io/badge/C++-20-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.27+-green.svg)
![Raylib](https://img.shields.io/badge/UI-Raylib-red.svg)
![Architecture](https://img.shields.io/badge/Architecture-FSM%20%7C%20MVC-orange.svg)

A professional **Software-In-the-Loop (SIL)** simulation environment for an autonomous RC vehicle. This project demonstrates a **Drive-by-Wire** architecture where user inputs are filtered through an intelligent **Advanced Driver Assistance System (ADAS)** before reaching the actuators.

> **Note:** This project focuses on complex safety algorithms, finite state machines, and real-time telemetry rendering without the need for physical hardware.

[INSERT SCREENSHOT HERE - Drag and drop an image of your dashboard]

## 🌟 Key Features & Safety Protocols

The core of the system is the **Safety Brain**, which utilizes a strict **Finite State Machine (FSM)** to evaluate environmental threats and override user controls if necessary.

*   📡 **Front LiDAR Scanner (Collision Avoidance):** Calculates distance to obstacles based on real-time speed. If a wall is detected within `2.5m`, the system executes an emergency reverse thrust (PWM = -50%). Includes evasion maneuver mechanics (obstacle disappears if the steering angle is > 20°).
*   🌡️ **Thermal Management System:** Simulates motor heat generation based on engine load.
    *   **> 80°C (Warning):** Activates thermal throttling, capping motor power to 40% to allow passive cooling.
    *   **> 120°C (Fatal):** Engine shutdown. PWM drops to 0% until safe temperatures are restored.
*   🔋 **Smart Battery & RTH:** Simulates realistic power drain. If the battery drops below `15%`, the system locks user input and enters an automated **Return-To-Home** cruise mode.
*   🛣️ **Smart Cruise Control:** Maintains target speed automatically. Instantly disengages if the user brakes or if any ADAS protocol (like collision avoidance) is triggered.
*   🎮 **Physics Engine (Lerp):** User inputs are not digital. The system uses Linear Interpolation to simulate vehicle mass (inertia) and servo motor movement delays.

## 🏗️ Architecture

The project is built using modern C++ principles:
*   **MVC Pattern:** Strict separation between the mathematical logic (`SafetyBrain`, `ThermalSystem`) and the graphical representation.
*   **Pure FSM:** The ADAS logic avoids "spaghetti code" by using a clean State Machine (`NORMAL_DRIVE`, `COLLISION_AVOIDANCE`, `THERMAL_WARNING`, `CRITICAL_HALT`).
*   **Raylib GUI:** A highly optimized, 60-FPS custom dashboard rendering real-time telemetry, a dynamic visual radar, and colored safety logs.

## 🕹️ Simulator Controls

To test the ADAS protocols, you act as both the driver and the environment (injecting failures).

| Key | Action | Description |
| :--- | :--- | :--- |
| **W / S** | Drive / Reverse | Accelerate or brake. |
| **A / D** | Steer | Turn the servo mechanism left/right. |
| **C** | Cruise Control | Locks current speed. Disengages on braking/danger. |
| **O** | Spawn Obstacle | Injects a virtual wall 8 meters ahead to test LiDAR. |
| **H** | Heat Motor | Artificially increases motor temp to test thermal shutdowns. |

## 🚀 Build Instructions

This project uses CMake and automatically downloads its dependencies (`raylib`). No manual library installation is required!

1. Clone the repository:
   ```bash
   https://github.com/PopuloNes/SmartRC-ADAS-Simulator
