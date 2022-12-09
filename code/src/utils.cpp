#include "treatbot_globals.h"

extern void process_pixels();

void do_heartbeat_led() {
  static uint32_t previous_millis = 0;
  static bool led_state = true;
  static uint32_t next_interval = HEARTBEAT_LED_ON;

  yield();
  process_pixels();
  uint32_t current_millis = millis();
  if (current_millis - previous_millis >= next_interval) {
    previous_millis = current_millis;
    digitalWrite(KEEPALIVE_LED_Pin, led_state);
    //condition ? value_if_true : value_if_false
    next_interval = led_state ? (HEARTBEAT_LED_ON) : (HEARTBEAT_LED_OFF);
    led_state = !led_state;
  }
}

void long_delay(uint32_t milliseconds) {
    const uint8_t chunks=100;
    const uint16_t iterations = (milliseconds/chunks);
    for (int x=0; x<iterations; x++) {
        yield();
        process_pixels();
        delay(100);
    }
}