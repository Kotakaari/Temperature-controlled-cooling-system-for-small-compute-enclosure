# Test plan

This document lists planned tests and their current status.
Detailed results and evidence are stored in dated folders under Tests.

| Test | Expected behaviour | Status |
|---|---|---|
| Temperature readings | Repeated readings appear in Serial Monitor. | Demonstrated |
| Hand warming | Reading rises when the probe is held. | Demonstrated |
| Cooling after release | Reading gradually decreases after release. | Demonstrated |
| Sensor disconnection | An error appears while the data connection is absent. | Controlled test pending |
| Sensor reconnection | Readings resume after restoring the connection and restarting. | Controlled test pending |
| Measurement accuracy | Readings are compared with a reference thermometer. | Not tested |
| Direct fan power | Fan spins smoothly from its external 12 V supply. | Observed by builder |
| Standalone fan PWM commands | Fan responds to repeating 100%, 60% and 30% commands, held for 15 seconds each. | Qualitatively demonstrated; see [fan test evidence](Tests/2026-10-26-fan-pwm/README.md) |
| PWM waveform | Oscilloscope confirms frequency and duty cycle at the fan control input. | Not measured; firmware frequency calculated as 25 kHz |
| Fan RPM | Tachometer readings quantify speed at each command. | Not tested |
| Temperature-based fan control | Fan command changes automatically with measured temperature. | Not implemented |
| Enclosure cooling | Controlled heat-load tests compare temperature with different fan settings. | Planned |
