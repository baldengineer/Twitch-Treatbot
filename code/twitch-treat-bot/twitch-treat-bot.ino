/**
   twitch-treat-bot
   james@baldengineer.com

    Based on TMCStepper Library Example
*/

// So we can to the interwebs
#include "config.h"  // edit config.sample and rename to config.h
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "neo-panel.h"

// So that we can see what's going on. hey hey hey
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED Display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

// todo: create baldengineer logo for OLEDs


// Library for TMC2208
#include <TMCStepper.h>


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

void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to '");
  Serial.print(WIFI_SSID); Serial.println("'");

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// MQTT PubSub callback
// dispense-treat-toggle
// treat-counter-text

void start_treats_cycle() {
  previous_indicator_millis = millis();
  arm_indicator_countdown = true;
  neo_panel_enable = true;

  // display.clearDisplay();
  // display.setTextSize(2);             // Normal 1:1 pixel scale
  // display.setTextColor(SSD1306_WHITE);        // Draw white text
  // display.setCursor(0, 0);            // Start at top-left corner
  display.setCursor(0,oled_row_counter());
  display.println(F("Treats for Baldee"));
  display.display(); // kinda important if you want to actually want to see anything... just sayin'
}

void callback(char* topic, byte* payload, unsigned int length) {
  String topic_str = topic;

  Serial.print("Message arrived [");
  Serial.print(topic_str);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if (topic_str == "stream/dispense-treat-toggle") {
    Serial.println("Processing stream/dispense-treat-toggle");
    if ((char)payload[0] == '1') {
      start_treats_cycle();
      arm_indicator_countdown = true;
      previous_indicator_millis = millis();
    }
  } else if (topic_str == "stream/treat-counter-text") {
    Serial.println("Processing stream/treat-counter-text");
    if ((char)payload[0] == '1') {
      //mp3Play(3);
    }
  }
}

// dispense-treat-toggle
// treat-counter-text
void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "notifier-";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect
    if (client.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
      Serial.println("connected");

      // MQTT topics for subscription
      client.subscribe("stream/dispense-treat-toggle");
      client.subscribe("stream/treat-counter-text");

      Serial.println("subscribed!");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

// the wiggle button!
#define WIGGLE_BTN 15
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
bool global_enable = false;

// To make code easier to read with pull-ups
#define PRESSED LOW
#define NOT_PRESSED HIGH

#define EN_PIN           12 // Enable
#define DIR_PIN          14 // Direction
#define STEP_PIN         13 // Step
//#define CS_PIN           42 // Chip select
//#define SW_MOSI          66 // Software Master Out Slave In (MOSI)
//#define SW_MISO          44 // Software Master In Slave Out (MISO)
//#define SW_SCK           64 // Software Slave Clock (SCK)
//#define SW_RX            13 // TMC2208/TMC2224 SoftwareSerial receive pin
//#define SW_TX            15 // TMC2208/TMC2224 SoftwareSerial transmit pin
#define SERIAL_PORT Serial1 // TMC2208/TMC2224 HardwareSerial port
//#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2

#define R_SENSE 0.11f // Match to your driver
// SilentStepStick series use 0.11
// UltiMachine Einsy and Archim2 boards use 0.2
// Panucatt BSD2660 uses 0.1
// Watterott TMC5160 uses 0.075

// Select your stepper driver type
//TMC2130Stepper driver(CS_PIN, R_SENSE);                           // Hardware SPI
//TMC2130Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK); // Software SPI
//TMC2660Stepper driver(CS_PIN, R_SENSE);                           // Hardware SPI
//TMC2660Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);
//TMC5160Stepper driver(CS_PIN, R_SENSE);
//TMC5160Stepper driver(CS_PIN, R_SENSE, SW_MOSI, SW_MISO, SW_SCK);

TMC2208Stepper driver(&SERIAL_PORT, R_SENSE);                     // Hardware Serial
//TMC2208Stepper driver(SW_RX, SW_TX, R_SENSE);                     // Software serial
//TMC2209Stepper driver(&SERIAL_PORT, R_SENSE, DRIVER_ADDRESS);
//TMC2209Stepper driver(SW_RX, SW_TX, R_SENSE, DRIVER_ADDRESS);

String mqtt_status_str(int state) {
  switch (state) {
    case MQTT_CONNECTION_TIMEOUT:
      return "Connect Timeout";
      break;
    case MQTT_CONNECTION_LOST:
      return "Connection Lost";
      break;
    case MQTT_CONNECT_FAILED:
      return "Connect Failed";
      break;
    case MQTT_DISCONNECTED:
      return "Disconnected";
      break;
    case MQTT_CONNECTED:
      return "Connected";
      break;
    case MQTT_CONNECT_BAD_PROTOCOL:
      return "BAD_PROTOCOL";
      break;
    case MQTT_CONNECT_BAD_CLIENT_ID:
      return "BAD_CLIENT_ID";
      break;
    case MQTT_CONNECT_UNAVAILABLE:
      return "UNAVAILABLE";
      break;
    case MQTT_CONNECT_BAD_CREDENTIALS:
      return "Bad Cred";
      break;
    case MQTT_CONNECT_UNAUTHORIZED:
      return "Unauthorized";
      break;
    default:
      return "I dunno!";
      break;

  }
  return "No can haz.";

}

uint8_t oled_row_counter() {
  static uint8_t current_row = 0;
  static uint8_t line_height = (SCREEN_HEIGHT/4);
  current_row = current_row + line_height;
  if (current_row >= SCREEN_HEIGHT)
    current_row = 0;
  // clearing the row here, depsite the name of the function
  // for reference:
  // void drawRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  // void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  if (current_row == 0)
    display.fillRect(0,current_row, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK);
  else
    display.fillRect(0,current_row, SCREEN_WIDTH, line_height, SSD1306_BLACK);

  return current_row;
}

void oled_clear() {
    display.clearDisplay();
    display.setRotation(2);
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setTextWrap(false);  // clip text that is too long
    display.setCursor(0, 0);            // Start at top-left corner
}

void display_mqtt_state(bool force_update) {
  static uint8_t previous_client_state = 0xFF;
  uint8_t current_client_state = client.state();

  if (force_update || (previous_client_state != current_client_state)) {
    previous_client_state = current_client_state;

    display.setCursor(0,oled_row_counter());
    display.println(mqtt_status_str(current_client_state));
    display.display();

    Serial.print("["); Serial.print(millis()); Serial.print("]"); Serial.println(mqtt_status_str(current_client_state));
  }
}

void setup() {
  Serial.begin(115200);
  delay(250);
  Serial.println(F("\n\nHello chat, you look great today!!!!"));

  pinMode(KEEPALIVE_LED_Pin, OUTPUT);
  digitalWrite(KEEPALIVE_LED_Pin, HIGH);

  Serial.print(F("Setting up OLED..."));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }
  Serial.println(F("done"));

//  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
//  FastLED.setBrightness(  BRIGHTNESS );

  
  oled_clear();
  display.println(F("TreatBot 3"));
  display.display(); // kinda important if you want to actually want to see anything... just sayin'

  display.setCursor(0,oled_row_counter());
  display.println(F("WiFi..."));
  display.display();
  Serial.print(F("Setting up WiFi..."));
  // connect to WiFi
  setup_wifi();
  Serial.println(F("done"));

  // wait for a connection
  display.setCursor(0,oled_row_counter());
  display.println(F("MQTT..."));
  display.display();
  Serial.print(F("Attempting MQTT..."));
  // Connect to MQTT Broker
  espClient.setFingerprint(fingerprint); // when you do know it
  client.setServer(MQTT_SERVER, MQTT_SERVERPORT);
  client.setCallback(callback);

  while (client.state() != MQTT_CONNECTED) {
    reconnect();
    display_mqtt_state(false);
    delay(250);
  }
  display_mqtt_state(false);
  Serial.println(F("done!"));

  Serial.print(F("Setting up Stepper Motor..."));
  display.setCursor(0,oled_row_counter());
  display.println(F("Stepper..."));
  display.display();
  pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);      // Enable driver in hardware

  // setup input buttons (there used to be more than one)
  pinMode(WIGGLE_BTN, INPUT_PULLUP);
  previous_wiggle_button_state = digitalRead(WIGGLE_BTN);


  // Enable one according to your setup
  //SPI.begin();                    // SPI drivers
  SERIAL_PORT.begin(115200);      // HW UART drivers
  //driver.beginSerial(115200);     // SW UART drivers
  driver.begin();                 //  SPI: Init CS pins and possible SW SPI pins
  // UART: Init SW UART (if selected) with default 115200 baudrate
  driver.toff(5);                 // Enables driver in software
  driver.rms_current(600);        // Set motor RMS current
  driver.microsteps(16);          // Set microsteps to 1/16th
  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  //driver.en_spreadCycle(false);   // Toggle spreadCycle on TMC2208/2209/2224
  driver.pwm_autoscale(true);     // Needed for stealthChop
  Serial.println(F("done!"));

  display_mqtt_state(true);
}

bool shaft = false;

void ledKeepAlive() {
  if (global_enable == false) {
    if (millis() - previous_millis_blink >= blink_interval) {
      previous_millis_blink = millis();
      led_blink_state = !led_blink_state;
      digitalWrite(KEEPALIVE_LED_Pin, led_blink_state);
    }
  } else {
    //  digitalWrite(13, global_enable);
  }
}

void processButtons() {
  ledKeepAlive();
  // get button states
  bool current_wiggle_button_state = digitalRead(WIGGLE_BTN);

  if (previous_wiggle_button_state != current_wiggle_button_state) {
    previous_wiggle_button_state = current_wiggle_button_state;
    if (current_wiggle_button_state == PRESSED) {
      display.setCursor(0,oled_row_counter());
      display.println(F("Wiggle..."));
      display.display();
      Serial.println(F("Going to Wiggle Once"));
      wiggle_once = true;
    }
  }
}

// default should be amt=350 and speed=400
void shake_em_mms(int amt, int speed, int shakes) {
  for (int x = 0; x < (shakes - 1); x++) {
    digitalWrite(DIR_PIN, shaft);
    driver.shaft(shaft);
    for (uint16_t i = amt; i > 0; i--) {
      processButtons();
      process_neo();
      digitalWrite(STEP_PIN, HIGH);
      delayMicroseconds(speed);
      digitalWrite(STEP_PIN, LOW);
      delayMicroseconds(speed);
    }
    shaft = !shaft;
  }
}

void spin_for_treats(bool direction) {
  digitalWrite(DIR_PIN, direction);
  driver.shaft(direction);
  //digitalWrite(13, direction);
  int amt = 1000;
  int speed = 1000;

  for (uint16_t i = amt; i > 0; i--) {
    processButtons();
    process_neo();
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(speed);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(speed);
  }
}

void dispense_cycle() {
  shake_em_mms(400, 250, 10);
  spin_for_treats(false);
}

unsigned long neo_previous_millis = 0;
unsigned long neo_interval = (250); // isn't that 2? math is hard

CRGB color_cycle[] = {CRGB::White, CRGB::Red, CRGB::Blue, CRGB::Green};
byte excite_color_index = 0;


void process_neo() {
  if (neo_panel_enable) {
    if (millis() - neo_previous_millis >= neo_interval) {
      neo_previous_millis = millis();
      // static uint8_t startIndex = 0;
      // startIndex = startIndex + 32; /* motion speed */

      // FillLEDsFromPaletteColors( startIndex);
      // FastLED.show();
      for (int x = 0; x < NUM_LEDS; x++) {
        leds[x] = color_cycle[excite_color_index];
        // leds[x] = CRGB::White;
      }
      FastLED.show();

      excite_color_index++;
      if (excite_color_index >= 4)
        excite_color_index = 0;

    }
  } else {
    FastLED.clear();
    FastLED.show();
  }
}

void loop() {
  // Stay Connected
  if (!client.connected())
    reconnect();
  // Needed to keep MQTT alive
  client.loop();
  display_mqtt_state(false);

  processButtons();
  process_neo();

  if ((arm_indicator_countdown) && (millis() - previous_indicator_millis >= indicator_interval)) {
    // we received a 1 and decided to do something for a while
    // but that time is over

    // display.clearDisplay();
    // display.setTextSize(2);             // Normal 1:1 pixel scale
    // display.setTextColor(SSD1306_WHITE);        // Draw white text
    // display.setCursor(0, 0);            // Start at top-left corner
      display.setCursor(0,oled_row_counter());
    display.println(F("No moar treats"));
    display.display(); // kinda important if you want to actually want to see anything... just sayin'

    neo_panel_enable = false;
    arm_indicator_countdown = false; // stop checking if it is time to clear
    //treat_dispense->save(0); // turn off the LEDs through MQTT
    client.publish("dispense-treat-toggle", "0");

  }

  if (cycle_once) {
    cycle_once = false;
    // Serial.println(F("Cycling Once"));
    dispense_cycle();
  }

  if (run_once) {
    run_once = false;
    // Serial.println(F("Running Once"));
    spin_for_treats(false);
  }

  if (wiggle_once) {
    wiggle_once = false;
    // Serial.println(F("Wiggling Once"));
    shake_em_mms(400, 250, 10);
    display_mqtt_state(true);
  }
  if (global_enable && (millis() - previous_millis_wait >= wait_interval)) {
    dispense_cycle();
    previous_millis_wait = millis();
  }
}

/*
   Original based on this library
   Author Teemu Mäntykallio
   Initializes the library and runs the stepper
   motor in alternating directions.
*/
