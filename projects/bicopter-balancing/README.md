# 1 DOF Bicopter Balancing System

**Roll axis stabilization of a bicopter using an IMU, a Kalman filter and a tuned PID controller.**

Embedded control project | ESP32, MPU6050, C

[Back to all projects](../README.md)

## Overview

A bicopter has two rotors and, in this setup, is free to rotate about a single axis. Holding it level means controlling the difference in thrust between the two rotors, which turns balancing into a feedback control problem. The project covers the full loop: sensing the angle, estimating it reliably, and correcting it fast enough to keep the vehicle stable.

## System

| Part | Role |
|---|---|
| MPU6050 IMU | Measures angular rate and acceleration |
| Kalman filter | Combines the noisy gyroscope and accelerometer readings into a clean roll angle estimate |
| PID controller | Converts the angle error into a thrust difference between the two rotors |
| ESP32 | Runs the estimator and controller in real time |

## What it demonstrates

- Sensor fusion for attitude estimation on a small microcontroller
- Practical PID tuning on real hardware
- Embedded programming in C for the ESP32

## Tools

C (Arduino framework), ESP32, MPU6050
