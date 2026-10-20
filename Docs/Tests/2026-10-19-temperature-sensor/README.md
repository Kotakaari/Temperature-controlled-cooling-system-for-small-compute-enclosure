# Temperature sensor test — October 19, 2026

## Purpose
Check that the Arduino reads the DS18B20 sensor and responds
to temperature changes.

## Connections
- Adapter DAT → Arduino digital pin 2
- Adapter VCC → Arduino 5V
- Adapter GND → Arduino GND
- Adapter contains the 4.7 kΩ pull-up resistor.

## Software
Arduino IDE with OneWire and DallasTemperature libraries.
Serial Monitor set to 9600 baud.

## Results
- Untouched-probe screenshot shows repeated readings of 20.06 °C.
- Hand-warming screenshots show readings reaching 28.50 °C.
- After release, captured readings fall from 28.25 °C to 28.00 °C.
- An error message appeared, followed by resumed readings.
  A controlled disconnection test still needs documenting.

## Conclusion
The sensor produces readings and responds to warming and cooling.
Accuracy has not been checked against a reference thermometer.

## Evidence
Photographs are in the photos folder.
Serial Monitor captures are in the screenshots folder.