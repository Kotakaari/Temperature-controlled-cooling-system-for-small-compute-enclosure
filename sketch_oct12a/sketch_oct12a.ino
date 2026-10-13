float temperature = 30.0;  // Practice value, not a sensor reading

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Temperature: ");
  Serial.println(temperature);

  if (temperature >= 35.0) {
    Serial.println("Cooling needed");
  } else {
    Serial.println("Low cooling demand");
  }

  delay(1000);
}