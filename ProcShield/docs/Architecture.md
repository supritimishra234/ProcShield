# Architecture

ProcShield follows a layered monitoring and recovery flow centered around the
Supervisor. The Supervisor manages the TestService and coordinates the
monitoring and recovery process.

## Components

- **Supervisor** – Controls the overall service lifecycle and coordinates
  monitoring, failure detection and recovery.

- **TestService** – The service being monitored. It sends periodic heartbeat
  information to indicate that it is running normally.

- **Process Monitor** – Monitors the state and termination of the service
  process.

- **Heartbeat Monitor** – Checks the heartbeat from TestService and helps
  identify an unresponsive service.

- **Memory Monitor** – Monitors the service memory usage and checks it against
  the configured limit.

- **Failure Detection** – Combines the monitoring results to identify service
  failures.

- **Recovery** – Performs the required recovery action after a failure is
  detected and controls the restart process.

- **Event Logger** – Records important service events and forwards them through
  the available event reporting paths.

- **Character Driver** – Receives events through `/dev/procshield` and provides
  the kernel-side event interface.

- **UDP Receiver** – Receives service events sent through UDP on port `8080`.

## Flow

The Supervisor starts TestService and continuously monitors its process,
heartbeat and memory usage. The monitoring components provide information to
Failure Detection.

When a failure is identified, the system moves to the Recovery stage, where
the service is restarted according to the configured recovery policy.
Important events are then passed to the Event Logger.

The events can be reported through `/dev/procshield` to the character driver
or through UDP to the receiver, completing the monitoring, recovery and
event-reporting flow.