# Temperature, fan control and RPM integration test

**Date:** 24 November 2026. **Hardware:** ELEGOO Uno R3, DS18B20 probe with terminal adapter, ARCTIC P12 PWM PST, 2N3904 PWM interface and external 12 V fan supply. [Connections and schematic](../../temperature-fan-wiring.md).

## Procedure and observed results

The combined controller reads temperature on D4, sends fan commands on D9 and counts tach pulses on D2. The initial rule uses 30% at or below 24 C, a linear ramp to 100% at 28 C, and 100% for a detected sensor fault. Measurements below are transcribed from the supplied Serial Monitor screenshots.

| Test | Observation | Evidence |
|---|---|---|
| Room-temperature baseline | 21.06-21.19 C, status OK, 30% command, 24-26 pulses and 720-780 displayed RPM | [Baseline](screenshots/room-temperature-baseline.png) |
| Warming the probe by hand | Visible readings rise from 25.25 to 26.81 C; command rises from 52% to 79%; displayed RPM rises from 900 to 1500 | [Warming](screenshots/warming-response.png) |
| Upper threshold and fan-assisted cooling | 28.12 and 28.00 C command 100%. As readings fall to 27.00 C, command falls to 83% and displayed RPM falls from 1860 to 1650. The final partial line shows 26.87 C / 80%, but its RPM is not visible. | [Cooling](screenshots/cooling-from-full-command.png) |
| Return to minimum | 23.06-23.44 C, status OK, 30% command and 720-750 displayed RPM | [Minimum command restored](screenshots/return-to-minimum-command.png) |
| Sensor data disconnected from D4 | Temperature becomes unavailable, status SENSOR FAULT, command 100%. Displayed RPM rises from 720 to 1830-1860 as the fan accelerates. | [Disconnect fallback](screenshots/sensor-disconnect-fallback.png) |
| DAT reconnected to D4 without resetting | Status returns to OK, 20.00-20.44 C and 30% command. Displayed RPM falls from 1680 to 810-840 in the captured recovery interval. | [Recovery](screenshots/sensor-reconnection-recovery.png) |

The builder placed the probe in fan airflow during cooling. This demonstrates controller response during fan-assisted cooling; it does not quantify an enclosure cooling benefit or isolate fan performance from passive cooling. The displayed RPM may lag a newly applied command because it reflects a counting interval and the rotor takes time to change speed.

## Circuit photographs

- [Integrated circuit overview](photos/integrated-circuit-overview.png)
- [Sensor adapter and control circuit close-up](photos/sensor-and-control-closeup.png)
- [Fan connector and wiring](photos/fan-connector-and-wiring.png)

## Evidence and limits

The six screenshots and three photographs are preserved as supplied. Screenshot crops differ; no readings have been reconstructed or retouched. An AI-generated baseline crop preview was excluded from the evidence. The separate explanatory circuit diagram is labeled as generated artwork.

The disconnect and reconnect observations demonstrate the fallback and automatic recovery for this data-wire interruption. Detection latency was not measured. Other sensor failure modes, startup with a missing probe and stuck but plausible readings were not tested. The current validity checks do not detect all such failures.

RPM uses an unverified assumption of two tach pulses per revolution. Temperature accuracy and absolute RPM have not been checked against reference instruments. The 25 kHz PWM frequency is calculated from Timer1 settings and has not been measured on a scope. No full serial log or timestamps were captured, so the screenshots are selected observations rather than a continuous dataset.

## Code review completed

The builder reviewed the sketch's libraries, pin definitions, sensor objects, local-variable scope, linear temperature-to-command calculation, initialization, blocking conversion setting, validity checks, full-speed fallback, function arguments, pulse counting, timed count/save/reset sequence, RPM calculation and Serial Monitor output. Familiar PWM and tach sections were revisited by comparison with the prior sketch.

Next: confirm pulses per revolution and plan reference checks, then build the enclosure and safely mount the dummy heat load. Review temperature thresholds, sensor placement and minimum fan command using those tests.
