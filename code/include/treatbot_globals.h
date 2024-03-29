#ifndef treatbot_globals_h
#define treatbot_globals_h

#include <Arduino.h>
#include "treatbot_config.h"
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <SparkFun_Qwiic_MP3_Trigger_Arduino_Library.h>

// main.cpp
extern unsigned long heartbeat_led_interval;
#define SERIAL_PORT_WAIT_TIMEOUT 10000

// mp3 player
void mp3_status();
void mp3_play(int track);

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

// Display
void print_message(String message);
void oled_clear();
void candy_test();
#define OLED_LCD_ADDRESS 0x3C

// Blinky
#define HEARTBEAT_LED_ON 50
#define HEARTBEAT_LED_OFF 2500

// Pin Assignments
#define WIGGLE_BTN 16  // 3 was for ESP8266
#define KEEPALIVE_LED_Pin 13

//  Stepper Motor
#define CW 0x1
#define CCW 0x0 
#define SPIN_AMOUNT 1000
#define SPIN_SPEED  1000
#define STEP_EN_PIN           19 // Enable (12)
#define STEP_PIN         18 //  (13)
#define DIR_PIN          5 // Direction (14)

//#define R_SENSE 0.11f // Match to your driver (not used)

// Nice To Have
#define BALD 0x1
#define PRESSED LOW
#define NOT_PRESSED HIGH

// treatbot_globals.h
#endif