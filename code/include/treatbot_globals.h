#ifndef treatbot_globals_h
#define treatbot_globals_h

#include <Arduino.h>
#include "treatbot_config.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// Prototypes
void setup_wifi();

// Keep Alive Things
unsigned long previous_indicator_millis = millis();
unsigned long indicator_interval = 1000;
bool arm_indicator_countdown = false;

// ESP8266 WiFi
WiFiClientSecure espClient;    // oooo secure!

// PubSubClient
PubSubClient client(espClient);
unsigned long lastMsg = 0;
const byte MSG_BUFFER_SIZE = 50;
char msg[MSG_BUFFER_SIZE];
int value = 0;
int previous_mqtt_status = 0;

// the wiggle button!
#define WIGGLE_BTN 3
bool previous_wiggle_button_state = false;

// automatic cycle timer
unsigned long previous_millis_wait = 2500;
unsigned long wait_interval = 2500;

// status LED timer
bool led_blink_state = false;
unsigned long previous_millis_blink;
unsigned long blink_interval = 500;
#define KEEPALIVE_LED_Pin 0

bool wiggle_once = false;
bool run_once = false;
bool cycle_once = false;
bool global_enable = true;

// To make code easier to read with pull-ups
#define PRESSED LOW
#define NOT_PRESSED HIGH

// ws2812 Related
unsigned long neo_previous_millis = 0;
unsigned long neo_interval = (250); // isn't that 2? math is hard

// CRGB color_cycle[] = {CRGB::White, CRGB::Red, CRGB::Blue, CRGB::Green};
// byte excite_color_index = 0;


// Stepper related
bool shaft = false;
#define EN_PIN           12 // Enable
#define DIR_PIN          14 // Direction
#define STEP_PIN         13 // Step
#define R_SENSE 0.11f // Match to your driver


// treabot_globals.h
#endif