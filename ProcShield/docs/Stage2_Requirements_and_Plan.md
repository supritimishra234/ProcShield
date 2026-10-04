# Stage 2 – Project Requirements and Development Plan

## Project Requirements

After defining the project idea, the next step was to identify what
ProcShield should actually do.

The main requirement was to build a Linux-based system that can monitor a
service process and take suitable action when the service fails.

## Functional Requirements

ProcShield should be able to:

- Start and monitor a TestService process.
- Maintain a parent-child relationship with the service.
- Detect when the service terminates.
- Identify abnormal termination and signal-based crashes.
- Receive heartbeat information from the service.
- Detect when the service stops responding.
- Check the service memory usage.
- Detect when the configured memory limit is exceeded.
- Restart the service after a recoverable failure.
- Limit repeated restart attempts.
- Use increasing delays between repeated restarts.
- Record important service events.
- Send events to a Linux character device.
- Send events through UDP to a local receiver.
- Stop the monitored service properly when ProcShield is terminated.

## Non-Functional Requirements

- The system should run in a Linux environment and use C/C++ for the main
implementation.
- The project should use Linux system APIs directly where required instead of
depending on large external frameworks.
- The monitoring process should remain stable while the TestService is
running.
- The output should make important events easy to understand during testing.
- The source code should remain simple enough to explain the complete working
of the system during a project demonstration or interview.

## Planned Modules
The initial design was divided into a few modules so that each part had a
clear responsibility.
- **Supervisor** – controls the overall monitoring and recovery process.
- **ServiceManager** – handles starting and stopping the TestService.
- **ProcessMonitor** – observes the service process and heartbeat.
- **ResourceMonitor** – checks the service memory usage.
- **EventLogger** – handles service event reporting.
- **TestService** – acts as the service being monitored.
- **Character Driver** – provides the `/dev/procshield` interface.

These modules were planned to keep the implementation understandable and
avoid putting all monitoring logic into one large source file.

## Planned Features
The main features selected for development were:
- Process supervision
- Crash detection
- Heartbeat-based hang detection
- Memory monitoring
- Automatic service restart
- Restart limit
- Restart backoff
- Event logging
- Character device communication
- UDP event reporting
- Clean service shutdown

The recovery mechanism was planned to restart the service only when recovery
was appropriate and to stop attempting restarts after the configured limit
was reached.

## Development Approach
Development was planned incrementally rather than implementing the complete
system at once.
The first step was to create a basic TestService and establish the
Supervisor and child-process relationship.

The next step was to add process monitoring and determine how the Supervisor
would identify different types of service termination.

After the basic supervision flow was working, crash recovery and restart
control would be added.

Heartbeat monitoring would then be used to identify a service that was still
running but no longer responding.

Memory monitoring would be added using Linux process information.

Finally, event reporting through the character device and UDP would be
integrated with the monitoring system.

Each feature would be tested before moving to the next part of the system.

## Development Environment

The project was planned to be developed in a Linux environment using WSL2.

The main tools and technologies are:

- C and C++
- Linux system programming
- Linux process and signal APIs
- Linux pipes and IPC
- `/proc` filesystem
- Linux character device driver
- UDP sockets
- GNU Make
- Git
- WSL2

## Project Deliverables

The development was planned to produce:

- C/C++ source code
- Linux character driver source
- Makefiles
- Project documentation
- Architecture and design documentation
- Testing documentation
- GitHub repository containing the project

The development plan focused on applying Linux system programming and C/C++ concepts in a practical service monitoring and recovery system.