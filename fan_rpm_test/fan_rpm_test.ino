const byte FAN_PIN = 9;
const byte TACH_PIN = 2;
const unsigned int PWM_TOP = 639;

// Assumed pulses per complete turn; not yet independently verified.
const byte PULSES_PER_REVOLUTION = 2;

volatile unsigned long tachPulses = 0;

// Called automatically on each falling edge of the tach signal.
void countTachPulse() {
  tachPulses++;
}

void setFanPercent(byte percent) {
  if (percent > 100) {
    percent = 100;
  }

  if (percent == 100) {
    digitalWrite(FAN_PIN, LOW);
  } else if (percent == 0) {
    digitalWrite(FAN_PIN, HIGH);
  } else {
    OCR1A = ((unsigned long)(PWM_TOP + 1) *
             (100 - percent)) / 100;

    TCCR1A = _BV(WGM11) | _BV(COM1A1);
  }
}

void runFanStage(byte percent) {
  setFanPercent(percent);

  Serial.println();
  Serial.print("Starting fan command: ");
  Serial.print(percent);
  Serial.println("%");

  // Start a fresh pulse-counting window.
  noInterrupts();
  tachPulses = 0;
  interrupts();

  unsigned long windowStart = millis();

  // Take 15 readings, approximately one second apart.
  for (byte reading = 1; reading <= 15; reading++) {
    delay(1000);

    // Copy and reset the counter without an interrupt changing it.
    noInterrupts();
    unsigned long pulses = tachPulses;
    tachPulses = 0;
    unsigned long windowEnd = millis();
    interrupts();

    unsigned long elapsedMs = windowEnd - windowStart;
    windowStart = windowEnd;

    float rpm = (pulses * 60000.0) /
                (PULSES_PER_REVOLUTION * elapsedMs);

    Serial.print("Command: ");
    Serial.print(percent);
    Serial.print("% | Reading: ");
    Serial.print(reading);
    Serial.print("/15 | Pulses: ");
    Serial.print(pulses);
    Serial.print(" | RPM: ");
    Serial.println(rpm, 0);
  }
}

void setup() {
  Serial.begin(9600);

  digitalWrite(FAN_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);

  // The external 10 kΩ resistor supplies the tach pull-up.
  pinMode(TACH_PIN, INPUT);

  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  ICR1 = PWM_TOP;
  OCR1A = 0;

  TCCR1A = _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);

  attachInterrupt(
    digitalPinToInterrupt(TACH_PIN),
    countTachPulse,
    FALLING
  );

  Serial.println("Fan PWM and RPM test starting");
}

void loop() {
  runFanStage(100);
  runFanStage(60);
  runFanStage(30);
}