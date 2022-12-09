#include "main.h"

//extern void handle_treats();
extern void setup_stepper_motor();
extern void setup_pixel_panel();
extern void  process_pixels();

void setup() {
  pinMode(WIGGLE_BTN, INPUT_PULLUP);
  pinMode(KEEPALIVE_LED_Pin, OUTPUT);
  digitalWrite(KEEPALIVE_LED_Pin, HIGH);
  Serial.begin(115200);  // wait for serial, but not for too long.
  Serial.println("Waiting for character");
  while(Serial.available() == 0)
    if (millis() > SERIAL_PORT_WAIT_TIMEOUT)
      break;
  digitalWrite(KEEPALIVE_LED_Pin, LOW);

  setup_oled();
  print_message("TreatBot32");
  setup_stepper_motor();
  setup_pixel_panel();
  setup_mp3();
  setup_wifi();
  setup_mqtt();
}

void loop() {
  mqtt_stay_alive();
  handle_buttons();
  do_motor_tasks();
 // handle_treats();
  do_heartbeat_led();
  process_pixels();
}