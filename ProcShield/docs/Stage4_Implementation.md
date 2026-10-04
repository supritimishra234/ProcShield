# Stage 4 – Initial Implementation and Prototype

## Initial Prototype

The initial implementation started with a Supervisor and a separate `TestService` process.

The Supervisor creates and controls `TestService` and monitors its execution.

## Process Management

Linux process APIs such as `fork()`, `exec()`, and `waitpid()` were used to create the parent-child relationship.

This provided the basic foundation for process monitoring and recovery.

## Core Monitoring

Process monitoring was implemented to detect service termination and abnormal exits.

Heartbeat monitoring was then added through a Linux pipe to detect an unresponsive `TestService`.

## Resource Monitoring

Memory monitoring was added using the Linux `/proc` filesystem.

The Supervisor reads `VmRSS` information and compares it with the configured memory limit.

## Recovery

The recovery logic was added after the basic monitoring flow was working.

It handles service restart, restart counting, and increasing delays between repeated restart attempts.

## Event Reporting

`EventLogger` was introduced to keep event reporting separate from the monitoring logic.

Events are reported through application logging, `/dev/procshield`, and UDP.

## Character Driver

The Linux character device was integrated as the kernel-side event channel.

The user-space application communicates with the driver through `/dev/procshield`.

## Repository Structure

The implementation was organized into separate headers and source files based on component responsibilities.

```text
ProcShield/
├── include/
│   ├── Supervisor.h
│   ├── ServiceManager.h
│   ├── ProcessMonitor.h
│   ├── ResourceMonitor.h
│   └── EventLogger.h
├── src/
│   ├── main.cpp
│   ├── Supervisor.cpp
│   ├── ServiceManager.cpp
│   ├── ProcessMonitor.cpp
│   ├── ResourceMonitor.cpp
│   └── EventLogger.cpp
├── driver/
├── Makefile
└── TestService.cpp
```

This structure kept process management, monitoring, resource checking, and event reporting separated during development.

## Progressive Integration

The components were integrated step by step, starting with process supervision and then adding monitoring, recovery, and event reporting.

The resulting prototype formed the complete ProcShield monitoring workflow.

## Implementation Issues

During development, the original monitoring logic became concentrated in a large source file.

It was reorganized into separate components to make the implementation easier to maintain and explain.