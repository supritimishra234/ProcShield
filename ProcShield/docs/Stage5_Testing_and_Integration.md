# Stage 5 – Testing, Integration and Improvement

## Module Integration

All ProcShield modules were integrated into a single monitoring and recovery workflow.

The Supervisor coordinated service management, monitoring, recovery, and event reporting.

## Normal Operation Testing

`TestService` was started normally and monitored by the Supervisor.

Heartbeat messages were received and memory usage was monitored successfully.

## Crash Recovery Testing

A `SIGSEGV` was used to simulate a service crash.

The system detected the crash, generated `SERVICE_CRASHED`, and restarted `TestService` successfully.

## Restart and Backoff Testing

Repeated failures were used to verify controlled restart behaviour.

The restart delays increased to 1, 2, 4, and 8 seconds before the restart limit was reached.

```text
Failure → Detection → Reporting → Recovery → Restart / Stop
```

## Hang and Resource Testing

`SIGSTOP` was used to simulate a service hang. The heartbeat timeout detected the condition and triggered recovery.

Memory was checked using `/proc/<PID>/status` and compared with the 100 MB configured limit.

## Driver and UDP Testing

The `/dev/procshield` character device was rebuilt, loaded, and tested for user-space communication.

UDP events were also verified through the local receiver on port 8080.

## Debugging and Improvements

Driver permissions and the kernel build configuration were corrected during integration.

The `TestService` output was also simplified to keep execution and test results clear.

## Final Integration

The complete workflow was verified from service startup through monitoring, failure detection, event reporting, and recovery.