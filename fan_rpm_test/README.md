# Standalone PWM and RPM test

Open `fan_rpm_test.ino` in Arduino IDE. Select Arduino Uno and the connected port, upload, then open Serial Monitor at 9600 baud. No external libraries are needed beyond Arduino AVR Boards support. See the [wiring guide](../Docs/fan-rpm-wiring.md) before connecting power.

This sketch preserves the earlier fan-control approach and adds tachometer feedback on D2. It repeats 100%, 60% and 30% commands, collecting 15 readings per stage at approximately one-second intervals. This remains a blocking bench-test program, not the final temperature controller.

## How the measurement works

1. `attachInterrupt(digitalPinToInterrupt(TACH_PIN), countTachPulse, FALLING)` associates D2's falling edges with the counting function.
2. The interrupt handler increments `volatile unsigned long tachPulses`. It can run during `delay()`. `volatile` does not make a multi-byte operation atomic.
3. At the beginning of a stage, the live counter is cleared and a starting timestamp is saved.
4. After each delay, a short interrupt-disabled block copies the count into `pulses`, resets the live count and records the end timestamp.
5. The saved count and elapsed milliseconds give `RPM = pulses * 60000 / (pulsesPerRevolution * elapsedMs)`.
6. The preceding end timestamp becomes the next start timestamp. The saved count remains available after the live counter is cleared.

`PULSES_PER_REVOLUTION = 2` is an explicit assumption, not an experimentally established property of this exact fan. Raw pulse counts are measured; absolute RPM depends on this conversion factor. At two pulses per revolution and a one-second window, one count represents approximately 30 RPM.

## PWM configuration and simplification

Timer1 uses Fast PWM mode 14, ICR1 as TOP, prescaler 1 and D9 / OC1A as output. The nominal calculation is 16 MHz / 640 = 25 kHz. OCR1A selects the switching point; the transistor inverts the output.

The endpoint branches use `digitalWrite()` alone. The Uno's [Arduino AVR implementation](https://github.com/arduino/ArduinoCore-avr/blob/master/cores/arduino/wiring_digital.c) clears COM1A1 on D9 before setting the level, making the earlier explicit TCCR1A assignments redundant in these branches. Intermediate commands re-enable COM1A1. This simplifies endpoint handling without changing the intended power arrangement.

## Provenance and limits

The sketch was copied from the builder's saved Arduino file on 22 November 2026, and its SHA-256 hash was checked against that source. No firmware edits were made during the copy. The builder's supplied Serial Monitor screenshots document the hardware run. A fresh compilation attempt during repository preparation was blocked by a Windows compiler DLL-relocation error; it is not recorded as a successful compile or a new hardware test.

No oscilloscope or independent optical-tachometer verification has been performed. The interrupt-disabled section and timestamp placement introduce small timing boundaries; this is a practical initial pulse-counting test, not calibrated metrology. Zero-command fan behaviour was not tested by the three-stage sequence. The temperature sensor and heat load are not part of this test.

[Test evidence and results](../Docs/Tests/2026-11-21-fan-rpm/README.md)
