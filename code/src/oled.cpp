
#include "treatbot_globals.h"

// OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

uint8_t oled_next_row();

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);
// TODO: create baldengineer logo for OLEDs
void setup_oled() {
  Serial.print(F("Setting up OLED..."));
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_LCD_ADDRESS)) { 
    Serial.println(F("SSD1306 allocation failed!"));
    //while (1); 
  } else {
    Serial.println(F("done"));
  }
}

 void print_message(String message) {
    display.setCursor(0,oled_next_row());
    display.println(message);
    display.display();
    
    Serial.println(message);
 }

 uint8_t oled_next_row() {
  static uint8_t current_row = 0;
  static uint8_t line_height = (SCREEN_HEIGHT/4);
  
  current_row = current_row + line_height;
  if (current_row >= SCREEN_HEIGHT)
    current_row = 0;
  if (current_row == 0)
    display.fillRect(0,current_row, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_BLACK); // void fillRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t color);
  else
    display.fillRect(0,current_row, SCREEN_WIDTH, line_height, SSD1306_BLACK);

  return current_row;
}

//  void oled_clear() {
//     display.clearDisplay();
//     display.setRotation(2);
//     display.setTextSize(2);             // Normal 1:1 pixel scale
//     display.setTextColor(SSD1306_WHITE);        // Draw white text
//     display.setTextWrap(false);  // clip text that is too long
//     display.setCursor(0, 0);            // Start at top-left corner
// }