# Test plan

This document lists planned tests and their current status.
Detailed results and evidence are stored in dated folders under Tests.

| Test | Expected behaviour | Status |
|---|---|---|
| Temperature readings | Repeated readings appear in Serial Monitor. | Demonstrated |
| Hand warming | Reading rises when the probe is held. | Demonstrated |
| Cooling after release | Reading gradually decreases after release. | Demonstrated |
| Sensor disconnection | Temperature becomes unavailable, SENSOR FAULT is reported and fan command becomes 100%. | Demonstrated with DAT disconnected from D4; see [integration test](Tests/2026-11-24-temperature-fan-integration/README.md) |
| Sensor reconnection | Valid readings and temperature-based commands resume after restoring DAT. | Demonstrated without reset; see integration test |
| Measurement accuracy | Readings are compared with a reference thermometer. | Not tested |
| Direct fan power | Fan spins smoothly from its external 12 V supply. | Observed by builder |
| Standalone fan PWM commands | Fan responds to repeating 100%, 60% and 30% commands, held for 15 seconds each. | Qualitatively demonstrated; see [fan test evidence](Tests/2026-10-26-fan-pwm/README.md) |
| PWM waveform | Oscilloscope confirms frequency and duty cycle at the fan control input. | Not measured; firmware frequency calculated as 25 kHz |
| Fan RPM | Tachometer readings quantify speed at each command. | Pulse counting demonstrated; RPM assumes two pulses per turn. See [results](Tests/2026-11-21-fan-rpm/README.md) |
| Absolute RPM validation | Exact pulses per turn are established and speed is checked independently. | Pending |
| Temperature-based fan control | Fan command changes automatically with measured temperature. | Demonstrated with provisional 24/28 C thresholds, 30-100% ramp and fan-assisted cooling; see integration test |
| Enclosure cooling | Controlled heat-load tests compare temperature with different fan settings. | Planned |
