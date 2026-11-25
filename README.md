# Temperature-Controlled Cooling System

An Arduino-based cooling prototype for a small compute enclosure. The project explores temperature sensing, PWM fan control and automatic cooling in response to temperature.

## Current progress

- DS18B20 temperature readings demonstrated, including response to hand warming and cooling after release.
- Standalone fan test completed with repeating 100%, 60% and 30% PWM commands, held for 15 seconds each. Changes in fan behaviour were observed by the builder and recorded on video.
- Temperature sensing on D4 now controls the fan command on D9 while D2 continues to report tachometer pulses and calculated RPM.
- The integrated bench test demonstrated the temperature ramp, full command at 28 C, return to 30% below 24 C, full-speed fallback on sensor data disconnection and recovery after reconnection without reset.
- Tachometer pulse counting now runs alongside PWM control. The initial test produced mean displayed speeds of approximately 1,894, 1,247 and 743 RPM at 100%, 60% and 30% commands respectively, assuming two pulses per turn.

Absolute RPM, PWM waveform and temperature measurement accuracy have not yet been verified with reference instruments. The tachometer pulses-per-turn assumption remains unconfirmed for this exact fan. PWM command percentages are not measured percentages of maximum RPM.

## Documentation

- [Test plan and status](Docs/test-plan.md)
- [Initial temperature sensor test](Docs/Tests/2026-10-19-temperature-sensor/README.md)
- [Fan PWM test: notes, videos and Serial Monitor capture](Docs/Tests/2026-10-26-fan-pwm/README.md)
- [PWM and RPM test results](Docs/Tests/2026-11-21-fan-rpm/README.md)
- [PWM and tachometer wiring](Docs/fan-rpm-wiring.md)
- [Integrated controller wiring and schematic](Docs/temperature-fan-wiring.md)
- [Temperature control, sensor-failure and recovery tests](Docs/Tests/2026-11-24-temperature-fan-integration/README.md)
- [Project logbook](Docs/Excel%20Files/Tempreture-controlled%20cooling%20system%20Logbook.xlsx)

## Firmware

- [Temperature sensor sketch](temperature_sensor/temperature_sensor.ino)
- [Standalone fan test sketch](fan_test/fan_test.ino)
- [Fan PWM and RPM sketch with explanation](fan_rpm_test/README.md)

Each Arduino sketch is kept in its own matching folder. The fan test targets an Arduino Uno R3 / ATmega328P and configures Timer1 directly; it is not portable to every Arduino board without changes.

## Next steps

Design and build the enclosure, mount the controlled heat source safely, and run cooling tests. Review the provisional 24/28 C control thresholds and 30% minimum command using the enclosure results. Confirm the tachometer pulses-per-turn assumption and compare temperature, speed and PWM timing with reference measurements.
