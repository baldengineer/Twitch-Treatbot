#include "main.h"

void setup() {
  pinMode(KEEPALIVE_LED_Pin, OUTPUT);
  digitalWrite(KEEPALIVE_LED_Pin, HIGH);
  
  // wait for serial, but not for too long.
  Serial.begin(115200);
  Serial.println("Waiting for character");
  unsigned long before_serial_wait = millis();
  while(Serial.available() == 0)
    if (millis() - before_serial_wait > SERIAL_PORT_WAIT_TIMEOUT)
      break;
  digitalWrite(KEEPALIVE_LED_Pin, LOW);

  setup_oled();
  print_message("TreatBot32");
  setup_wifi();
  setup_mqtt();
  
}

void loop() {
  mqtt_stay_alive();
  handle_buttons();
  do_motor_tasks();
  do_heartbeat_led();
}