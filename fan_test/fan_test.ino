const byte FAN_PIN = 9;
const unsigned int PWM_TOP = 639;

// Command the fan from 0 to 100%.
// The transistor inverts the Arduino's output.
void setFanPercent(byte percent) {
  if (percent > 100) {
    percent = 100;
  }

  if (percent == 100) {
    // Arduino LOW -> transistor off -> fan PWM HIGH.
    TCCR1A = _BV(WGM11);
    digitalWrite(FAN_PIN, LOW);
  } else if (percent == 0) {
    // Arduino HIGH -> transistor on -> fan PWM LOW.
    TCCR1A = _BV(WGM11);
    digitalWrite(FAN_PIN, HIGH);
  } else {
    // Reverse the Arduino duty cycle to account for the transistor.
    OCR1A = ((unsigned long)(PWM_TOP + 1) *
             (100 - percent)) / 100;

    TCCR1A = _BV(WGM11) | _BV(COM1A1);
  }

  Serial.print("Fan command: ");
  Serial.print(percent);
  Serial.println("%");
}

void setup() {
  Serial.begin(9600);

  digitalWrite(FAN_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);

  // Configure Timer1 for 25 kHz PWM on digital pin 9.
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  ICR1 = PWM_TOP;
  OCR1A = 0;

  TCCR1A = _BV(WGM11);
  TCCR1B = _BV(WGM13) | _BV(WGM12) | _BV(CS10);

  Serial.println("Fan speed test starting");
}

void loop() {
  setFanPercent(100);
  delay(15000);

  setFanPercent(60);
  delay(15000);

  setFanPercent(30);
  delay(15000);
}