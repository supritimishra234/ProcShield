# Stage 1 – Project Introduction

## Project Name

**ProcShield – Linux Service Monitoring and Recovery System**

## Introduction

ProcShield is a Linux-based system developed to monitor a running service and handle common service failures automatically.

The project uses a Supervisor to monitor a TestService during execution. When a failure is detected, the Supervisor identifies the problem and performs an appropriate recovery action.

The project was developed as an individual project using C/C++, Linux system programming, and Linux device driver concepts.

## Problem

A service running on Linux can fail in different ways during execution. It may crash because of a software error, stop responding, or consume more memory than expected.

If these failures are not detected properly, the service may remain unavailable and require manual intervention.

Repeated failures can also cause continuous restart attempts. Therefore, the monitoring system needs to detect failures and control the recovery process.

ProcShield is designed to provide a simple solution for monitoring these conditions and recovering the service in a controlled manner.

## Objective

The main objective of ProcShield is to develop a small Linux-based system that can monitor a service and respond to common failure conditions.

The system is designed to:

- Monitor a running service process.
- Detect service crashes.
- Detect an unresponsive service using heartbeat monitoring.
- Monitor the service memory usage.
- Record important service events.
- Restart the service after a failure.
- Limit repeated restart attempts.
- Apply increasing delays between repeated restarts.

## Project Focus

The project focuses on monitoring a single TestService process.

The Supervisor manages the service process and monitors its execution state.

A heartbeat mechanism is used to detect when the service stops responding, while Linux `/proc` is used to monitor its memory usage.

When a failure is detected, ProcShield records the event and performs the required recovery action.

The project also includes a Linux character device at `/dev/procshield` for event communication between the user-space application and the kernel.

UDP communication is used to send service events to a local receiver.

The implementation is intentionally kept simple so that the complete system can be developed, tested and explained as an individual Linux system programming project.

## Expected Outcome

The expected outcome of the project is a working Linux-based service monitoring system that can detect common service failures and perform controlled recovery.

The system should be able to identify crashes, service hangs and memory-limit violations, record the related events, and restart the service when recovery is possible.

The completed project should also demonstrate practical use of Linux process management, signals, pipes, `/proc`, character device drivers and basic socket communication.