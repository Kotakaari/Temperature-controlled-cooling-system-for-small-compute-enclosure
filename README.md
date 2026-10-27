# Temperature-Controlled Cooling System

An Arduino-based cooling prototype for a small compute enclosure. The project explores temperature sensing, PWM fan control and automatic cooling in response to temperature.

## Current progress

- DS18B20 temperature readings demonstrated, including response to hand warming and cooling after release.
- Standalone fan test completed with repeating 100%, 60% and 30% PWM commands, held for 15 seconds each. Changes in fan behaviour were observed by the builder and recorded on video.
- Temperature sensing and fan control are currently separate sketches.

RPM, PWM waveform and temperature measurement accuracy have not yet been verified with reference instruments. PWM command percentages are not measured percentages of maximum RPM.

## Documentation

- [Test plan and status](Docs/test-plan.md)
- [Initial temperature sensor test](Docs/Tests/2026-10-19-temperature-sensor/README.md)
- [Fan PWM test: notes, videos and Serial Monitor capture](Docs/Tests/2026-10-26-fan-pwm/README.md)

## Firmware

- [Temperature sensor sketch](temperature_sensor/temperature_sensor.ino)
- [Standalone fan test sketch](fan_test/fan_test.ino)

Each Arduino sketch is kept in its own matching folder. The fan test targets an Arduino Uno R3 / ATmega328P and configures Timer1 directly; it is not portable to every Arduino board without changes.

## Next steps

Measure fan RPM, combine temperature sensing with fan commands, and design and test the enclosure with a controlled heat source.
