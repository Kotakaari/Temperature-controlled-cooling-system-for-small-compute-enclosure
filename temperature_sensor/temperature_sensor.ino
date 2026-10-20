#include <OneWire.h>
#include <DallasTemperature.h>

// The adapter's DAT pin connects to Arduino digital pin 2.
const int SENSOR_PIN = 2;

OneWire oneWire(SENSOR_PIN);
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(9600);
  sensors.begin();

  Serial.println("Temperature sensor starting...");
  Serial.print("Sensors detected: ");
  Serial.println(sensors.getDeviceCount());
}

void loop() {
  // Ask the sensor to measure, then read the result.
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);

  if (temperatureC == DEVICE_DISCONNECTED_C) {
    Serial.println("ERROR: Cannot read sensor. Check wiring.");
  } else {
    Serial.print("Temperature: ");
    Serial.print(temperatureC, 2);
    Serial.println(" C");
  }

  delay(1000);
}
