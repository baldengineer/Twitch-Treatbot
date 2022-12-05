#ifndef treatbot_globals_h
#define treatbot_globals_h

#include <Arduino.h>
#include "treatbot_config.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

// main.cpp
extern unsigned long heartbeat_led_interval;

// utils.cpp
extern void long_delay(uint32_t milliseconds);

// treatbot-wifi.cpp    
extern void setup_wifi();
extern void scan_wifi();
extern WiFiClientSecure espClient;

// treatbot-mqtt.cpp
extern void mqtt_callback(char* topic, byte* payload, unsigned int length);
extern void mqtt_reconnect();
extern String mqtt_status_str(int state);
extern void display_mqtt_state(bool force_update);
extern void setup_mqtt();

// Blinky
#define HEARTBEAT_LED_ON 50
#define HEARTBEAT_LED_OFF 2500

// Pin Assignments
#define WIGGLE_BTN 3
#define KEEPALIVE_LED_Pin 13

//  Stepper Motor
#define EN_PIN           12 // Enable
#define DIR_PIN          14 // Direction
#define STEP_PIN         13 // Step
//#define R_SENSE 0.11f // Match to your driver (not used)

// Nice To Have
#define BALD 0x1
#define PRESSED LOW
#define NOT_PRESSED HIGH

// treatbot_globals.h
#endif