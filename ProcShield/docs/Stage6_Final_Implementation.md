# Stage 6 – Final Implementation and Presentation

## Final System

The final ProcShield system is a complete service monitoring and recovery system built around a Supervisor and a controlled `TestService` process.

The Supervisor manages the service lifecycle while monitoring its process state, heartbeat, and memory usage during execution.

## Monitoring and Recovery

The completed implementation can distinguish between normal execution, service crashes, and unresponsive service conditions.

Based on the detected condition, the Supervisor records the event and performs the required recovery action, including controlled service restart.

## Recovery Control

Recovery was designed to prevent uncontrolled restart behaviour.

Restart attempts are tracked by the Supervisor, with increasing delays between repeated failures and a defined restart limit.

## Event Reporting

The final system records important execution events such as service start, crash, hang, restart, and restart-limit conditions.

These events are handled through the `EventLogger` and are available through the implemented application, character-device, and UDP event paths.

## Complete Operation

The final system follows the complete lifecycle of the monitored service:

```text
Service Start
     ↓
Continuous Monitoring
     ↓
Normal Operation / Failure
     ↓
Failure Detection
     ↓
Recovery Decision
     ↓
Service Restart
     ↓
Monitoring Continues
```

## Final Presentation

The project will be presented through a live execution of ProcShield, showing the monitored service under normal operation and during controlled failure scenarios.

The presentation will explain the implementation through the actual system behaviour, demonstrating how monitoring, detection, and recovery work together as one system.