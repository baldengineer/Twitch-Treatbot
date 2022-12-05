#include <Arduino.h>

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("Hello ESP32");
}

void loop() {
  static uint32_t previous_millis = 0;
  
  if(millis() - previous_millis >= 1000) {
    previous_millis = millis();
    Serial.println(millis());
  }
}