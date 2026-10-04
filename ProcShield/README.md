# ProcShield

## Linux Service Monitoring and Recovery System
ProcShield is a Linux-based service monitoring and recovery system that monitors a `TestService` process and responds to common service failures.
The system uses a Supervisor to manage the service, monitor its execution, detect failures, and perform controlled recovery.

## Key Capabilities

- Process monitoring and service management
- Crash detection and recovery
- Heartbeat-based hang detection
- Memory monitoring through `/proc`
- Controlled restart and increasing backoff
- Restart limit handling
- Character-device event reporting
- UDP event reporting
- Clean service shutdown

## How It Works

ProcShield starts `TestService` as a child process and continuously monitors it.

`TestService` sends a heartbeat through a pipe every 2 seconds. The Supervisor uses this heartbeat along with process status and memory information to determine the current service condition.

When a failure is detected, the event is recorded and the recovery logic decides whether the service should be restarted.

Repeated failures are controlled using a restart limit and increasing restart delays.

## Failure Handling

| Condition | Detection | Recovery |
|---|---|---|
| **Crash** | Process termination / signal | Restart service |
| **Hang** | Heartbeat timeout | Terminate and restart |
| **Memory limit** | `/proc/<PID>/status` | Record resource event |

## Project Structure

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
│   ├── EventLogger.cpp
│   └── TestService.cpp
├── driver/
├── Makefile
└── README.md
```

## Build & Run

### 1. Build ProcShield
From the project root:

```bash
make
```

This builds the main ProcShield application using the project Makefile.

### 2. Build the Character Driver
Move into the driver directory:

```bash
cd driver
make
```

The driver Makefile uses the running kernel build directory to compile the module for the current Linux environment.

### 3. Load the Driver
Load the generated kernel module:

```bash
sudo insmod procshield_driver.ko
```

Verify that the device is available:

```bash
ls -l /dev/procshield
```

### 4. Run ProcShield
Return to the project root:

```bash
cd ..
./procshield
```

ProcShield starts `TestService` automatically and begins monitoring it.

### 5. Stop the System
ProcShield can be stopped using:

`Ctrl+C`

The Supervisor performs a controlled shutdown and waits for `TestService` to terminate.

## Demonstration

A typical project demonstration follows this sequence:

```text
Start ProcShield
      ↓
TestService Starts
      ↓
Heartbeat Monitoring
      ↓
Trigger Service Failure
      ↓
Failure Detection
      ↓
Event Reporting
      ↓
Recovery / Restart
      ↓
Monitoring Resumes
```
The demonstration shows the complete monitoring and recovery behaviour of ProcShield under both normal and failure conditions.