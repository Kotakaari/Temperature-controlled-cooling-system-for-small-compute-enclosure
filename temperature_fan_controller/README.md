# Temperature-controlled fan firmware

[Sketch](temperature_fan_controller.ino) for an Arduino Uno R3 / ATmega328P. This combines the earlier temperature, PWM and tachometer tests. The uploaded source was supplied by the builder after the 24 November bench test and is preserved byte-for-byte; only the repository filename/folder were normalized to `temperature_fan_controller` for Arduino's matching-name convention.

## Setup and running

1. Install the Arduino AVR Boards package and select Arduino Uno in Arduino IDE.
2. Install the OneWire and DallasTemperature libraries through Library Manager. `math.h` comes with the toolchain and does not need a separate library installation.
3. Follow the [integrated wiring guide](../Docs/temperature-fan-wiring.md): sensor DAT on D4, tach on D2 with its external pull-up, and D9 through the NPN interface to the fan PWM input. Share ground between the Uno and the external 12 V fan supply.
4. Open this sketch, choose the Uno's connected port, verify and upload. Open Serial Monitor at 9600 baud.
5. The startup command is 100%. Valid temperature readings then determine the command. The report includes temperature/status, command, raw pulse count and calculated RPM roughly once per second.

The firmware owns Timer1 and D9. Other code or libraries that require Timer1 must be reviewed before combining them. Do not use the wiring or direct register settings unchanged on a different microcontroller.

## Control rule

| Sensor input | Fan command |
|---|---|
| Valid temperature at or below 24 C | 30% |
| Valid temperature between 24 and 28 C | Linear ramp from 30% to 100%, rounded to a whole percent |
| Valid temperature at or above 28 C | 100% |
| Non-finite value, library disconnect error or value outside -55 to 125 C | 100% |

For 26 C, `fraction = (26 - 24) / (28 - 24) = 0.5`; `command = 30 + 0.5 * (100 - 30) = 65%`. The 24/28 C thresholds and 30% minimum are provisional bench settings. Tune them after enclosure testing, considering ambient conditions, sensor placement and intended operating temperatures. No hysteresis is implemented.

## How the sketch works

### Sensor communication

`OneWire oneWire(SENSOR_PIN)` creates the bus object on D4. `DallasTemperature sensors(&oneWire)` creates a sensor interface using that same bus. These declarations do not take a reading. `sensors.begin()` initializes sensor discovery; `setWaitForConversion(true)` makes measurement requests wait for conversion.

Each loop calls `requestTemperatures()`, then `getTempCByIndex(0)` to retrieve the first sensor's result. `sensorOK` combines all validity checks with logical AND. A valid-looking result is not proof of measurement accuracy or freshness.

### Applying the command

`commandFromTemperature()` returns a command percentage. `setFanPercent(fanPercent)` passes that number to the existing hardware-control function. The two functions have different jobs: calculate a command, then apply it.

Timer1 uses Fast PWM with ICR1 as TOP, set to 639, and no clock prescaler. At a nominal 16 MHz clock, the calculated frequency is `16,000,000 / (639 + 1) = 25,000 Hz`. This has not been verified with a scope.

The NPN transistor inverts D9. D9 LOW turns the transistor off and lets the fan's internal pull-up make its PWM input HIGH, commanding 100%. D9 HIGH pulls the fan input LOW for the 0% endpoint. Intermediate settings use `100 - percent` to compensate for inversion. On the Uno AVR core, `digitalWrite()` disconnects hardware PWM on the relevant pin; intermediate settings re-enable it with COM1A1. The fan motor retains its separate 12 V supply throughout.

### Counting and timing

The D2 falling-edge interrupt calls `countTachPulse()`, which only increments `tachPulses`. `volatile` tells the compiler this counter can change through an interrupt. Briefly disabling interrupts protects copying/resetting the multi-byte counter on the 8-bit Uno; hardware PWM continues.

The first reset discards counts collected during initialization. Each reporting interval then follows count, save, reset. `windowStart` and `windowEnd` are timestamps, not counters that increment themselves. The loop waits until at least 1000 ms have elapsed, saves the raw count, and uses the actual elapsed time:

```text
RPM = saved pulses * 60000 / (pulses per revolution * elapsed milliseconds)
```

With 60 pulses, two pulses per turn and 1000 ms, that gives 1800 RPM. Two pulses per revolution remains unconfirmed for this exact fan. PWM percentage is a command, not a percentage of measured full-speed RPM. The displayed command is newly applied while RPM describes the recent counting interval, so transient readings need not match steady-state behavior.

### Reporting

When `sensorOK` is true, the program prints Celsius to two decimal places and status OK. Otherwise it prints temperature unavailable and SENSOR FAULT. Both paths report command, raw pulses and rounded RPM. `println()` ends the line; `print()` continues on the same line. The startup text appears once during setup.

## Verification and remaining limits

The [hardware test record](../Docs/Tests/2026-11-24-temperature-fan-integration/README.md) documents warming, fan-assisted cooling, minimum/full-command behavior, DAT-disconnect fallback and reconnection without reset. A fresh command-line build of the supplied file was attempted during repository preparation, but Windows prevented `avr-g++` from launching with an Illegal System DLL Relocation error. This is an environment limitation, not a successful fresh compile; the reported hardware run is the functional evidence.

Exact dependency versions used by the builder were not captured. Absolute temperature accuracy, absolute RPM, PWM waveform and fault latency remain unverified. Blocking sensor conversion can delay the main loop. OneWire signaling briefly masks interrupts, so missed tach edges are a possible measurement limitation to assess against an independent reference. The checks do not detect a sensor stuck at a plausible temperature, and startup with a missing sensor was not tested. There is no independent fan-stall alarm or heater control in this sketch.
