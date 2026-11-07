# Holo Battalion

**Three holonomic robots that pick, carry and drop crates in a 10 ft by 10 ft arena, guided by an overhead camera.**

e-Yantra Robotics Competition, IIT Bombay | NIT Tiruchirappalli | ROS 2, ESP32, Python

[Back to all projects](../README.md)

## Overview

The competition task was to coordinate three robots working in a shared arena. The robots locate crates, pick them up, carry them to their destinations and drop them, while avoiding each other and the arena boundaries. Early tasks were carried out in simulation, and the later tasks ran on physical robots.

## System

| Layer | Approach |
|---|---|
| Perception | A fixed overhead camera detects ArUco markers to estimate the poses of the robots and crates |
| Path planning | Theta* finds any angle paths across the arena grid |
| Collision avoidance | ORCA lets the robots avoid one another by choosing velocities that stay clear of reciprocal velocity obstacles |
| Control | ROS 2 Humble nodes compute wheel and arm commands and send them over MQTT |
| Robots | Three wheel holonomic bases with an arm for handling crates, each driven by an ESP32 |

## How it works

The overhead camera gives a global view, so every robot and crate has a known position in the arena. A task allocator decides which robot handles which crate. Theta* then plans a path for each robot. Unlike a grid planner that is limited to eight directions, Theta* connects waypoints with straight lines whenever the line of sight is free, which gives shorter and smoother paths. Because three robots share one small arena, a path that is free of obstacles is not enough. ORCA adjusts each robot's velocity in real time, with every robot taking half of the responsibility for avoiding a collision. The resulting velocity is converted into wheel commands for the holonomic base and sent to the ESP32 on the robot.

## My contribution

I worked on path planning and collision avoidance, using Theta* planning and ORCA avoidance, and deployed and tuned them on the physical robots. I also worked on designing the bots, arena setup and sensor calibration
## Tools

ROS 2 Humble, Python, OpenCV with ArUco markers, MQTT, ESP32 firmware, Gazebo, SolidWorks

## Availability

The source code, firmware and task submissions are not published. The competition was run under a non disclosure agreement, and the code stays private in line with it.
