# Compliant Split Hub Gear

**A scissor gear without the spring: passive backlash elimination with monolithic beam flexures.**

REU-CM 2026 research project | M2D2 Lab, Indian Institute of Science, Bengaluru | Supervisor: Prof. Ananthasuresh | Seven weeks, May to July 2026

[Back to all projects](../README.md)

## Overview

Every gear train has backlash, a small angular gap between meshing teeth. Each time torque reverses, the driver crosses this dead zone before the driven gear moves, which limits positioning accuracy and destabilizes feedback control. One of the fixes is a scissor gear, where the gear is split in two and a coil spring pushes the halves apart so both tooth flanks stay in contact. The spring is a separate part that wears, fatigues and traps contamination, which makes it a poor fit for small, sterilizable, maintenance free actuators.

This project removes the spring. Two coaxial hub halves are joined by beam flexures built into the same body. Preloading the beams at assembly stores elastic energy, and that stored energy holds both flanks in contact. The motivating application is a miniature prosthetic finger actuator.

## Approach

The concept was proven first at an 80 mm, module 2 gear, where it is easy to calculate, simulate and 3D print. The same modelling approach can then be scaled down toward the target size of under 15 mm.

| Stage | What was done |
|---|---|
| Analytical model | Castigliano flexibility model and a nonlinear elastic solver for the beam flexures |
| Simulation | ANSYS static structural analysis of stress, deflection and hub rotation |
| Prototype | 3D printed ABS gears to check the preload deformation physically |
| Automation | Gear profile generation with Python, DXF and SolidWorks, and MATLAB optimization for the serpentine family |

Two beam families were developed and compared.

| Beam family | Role | Status |
|---|---|---|
| Straight beam | Baseline design | Analytical model, ANSYS and prototype agree with the design target |
| Serpentine beam | Route to compact and miniature versions | Close agreement with ANSYS, monolithic print demonstrated |

## Outcome

- The straight beam was sized to close the calculated backlash of the 80 mm gear, about 0.11 degrees of rotation. The simulated rotation matched the target, and the printed prototype showed the designed preload deformation.
- The serpentine design fits more compliant length into the same annulus. A monolithic print showed that the folded beams can be produced and preloaded as a single part.
- Both designs are valid but not proven optimal, because many beam geometries meet the same preload target.

## My work

I designed the compliant split hub gear, built the analytical and nonlinear elastic models, validated them against ANSYS, and automated the gear profile generation. I also produced and tested the 3D printed prototypes.

## Tools

ANSYS Mechanical, MATLAB, Python, SolidWorks, FDM 3D printing in ABS

## Availability

The final prototype images, videos and ANSYS result images from the seven weeks of work are in the project repository:

[REU-CM-2026 on GitHub](https://github.com/RAHUL-VIJAY-VELMURUGAN/REU-CM-2026)

The technical paper is still in preparation and full project details cannot be shared publicly at the moment, hence the repo is private for now

## Acknowledgements

Thanks to Prof. Ananthasuresh for the opportunity, to Jeevan for guidance, and to Vishwanath R. for help with manufacturing and prototyping. Support from all members of the M2D2 Laboratory is gratefully acknowledged. Financial support from Toyota Research Institute North America (TRINA), Ann Arbor, MI, USA, is gratefully acknowledged.
