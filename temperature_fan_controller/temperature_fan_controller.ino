#include <OneWire.h>
#include <DallasTemperature.h>
#include <math.h>

const byte FAN_PIN = 9;
const byte TACH_PIN = 2;
const byte SENSOR_PIN = 4;

const unsigned int PWM_TOP = 639;

// Still requires confirmation for this exact fan.
const byte PULSES_PER_REVOLUTION = 2;

// Initial bench-demo settings.
const float LOW_TEMP_C = 24.0;
const float HIGH_TEMP_C = 28.0;
const byte MIN_FAN_PERCENT = 30;

OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);

volatile unsigned long tachPulses = 0;
unsigned long windowStart = 0;

// Interrupt handler: count each tachometer falling edge.
void countTachPulse() {
  tachPulses++;
}

void setFanPercent(byte percent) {
  if (percent > 100) {
    percent = 100;
  }

  if (percent == 100) {
    // Transistor OFF -> fan PWM input HIGH.
    digitalWrite(FAN_PIN, LOW);
  } else if (percent == 0) {
    // Transistor ON -> fan PWM input LOW.
    digitalWrite(FAN_PIN, HIGH);
  } else {
    OCR1A = ((unsigned long)(PWM_TOP + 1) *
             (100 - percent)) / 100;

    TCCR1A = _BV(WGM11) | _BV(COM1A1);
  }
}

byte commandFromTemperature(float temperatureC) {
  if (temperatureC <= LOW_TEMP_C) {
    return MIN_FAN_PERCENT;
  }

  if (temperatureC >= HIGH_TEMP_C) {
    return 100;
  }

  // Position between the lower and upper temperature limits.
  float fraction = (temperatureC - LOW_TEMP_C) /
                   (HIGH_TEMP_C - LOW_TEMP_C);

  float command = MIN_FAN_PERCENT +
                  fraction * (100 - MIN_FAN_PERCENT);

  // Round to the nearest whole percent.
  return (byte)(command + 0.5);
}

void setup() {
  Serial.begin(9600);

  // Begin with transistor OFF: full-speed fan command.
  digitalWrite(FAN_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);

  // External 10 kΩ tachometer pull-up is already fitted.
  pinMode(TACH_PIN, INPUT);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  ICR1 = PWM_TOP;
  OCR1A = 0;

  TCCR1A = _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);

  setFanPercent(100);

  attachInterrupt(
    digitalPinToInterrupt(TACH_PIN),
    countTachPulse,
    FALLING
  );

  sensors.begin();

  // Wait for each temperature conversion before reading it.
  sensors.setWaitForConversion(true);

  noInterrupts();
  tachPulses = 0;
  interrupts();

  windowStart = millis();

  Serial.println("Temperature / fan / RPM controller starting");
  Serial.println("Startup fan command: 100%");
}

void loop() {
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  bool sensorOK =
    isfinite(temperatureC) &&
    temperatureC != DEVICE_DISCONNECTED_C &&
    temperatureC >= -55.0 &&
    temperatureC <= 125.0;

  byte fanPercent;

  if (sensorOK) {
    fanPercent = commandFromTemperature(temperatureC);
  } else {
    fanPercent = 100;
  }

  setFanPercent(fanPercent);

  // Keep each reporting window at least approximately one second.
  while (millis() - windowStart < 1000UL) {
    delay(1);
  }

  noInterrupts();
  unsigned long pulses = tachPulses;
  tachPulses = 0;
  unsigned long windowEnd = millis();
  interrupts();

  unsigned long elapsedMs = windowEnd - windowStart;
  windowStart = windowEnd;

  float rpm = (pulses * 60000.0) /
              (PULSES_PER_REVOLUTION * elapsedMs);

  if (sensorOK) {
    Serial.print("Temperature: ");
    Serial.print(temperatureC, 2);
    Serial.print(" C | Status: OK");
  } else {
    Serial.print("Temperature: unavailable");
    Serial.print(" | Status: SENSOR FAULT");
  }

  Serial.print(" | Fan command: ");
  Serial.print(fanPercent);
  Serial.print("% | Pulses: ");
  Serial.print(pulses);
  Serial.print(" | RPM: ");
  Serial.println(rpm, 0);
}