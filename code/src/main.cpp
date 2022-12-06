#include "main.h"

void setup() {
  delay(500);
  Serial.begin(115200);
  Serial.println("Hello ESP32");
  setup_wifi();
  setup_mqtt();
  pinMode(KEEPALIVE_LED_Pin, OUTPUT);
}

void loop() {
  mqtt_stay_alive();
  do_heartbeat_led();
  delay(100);
}