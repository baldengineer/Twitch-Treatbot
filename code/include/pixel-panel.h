#include <FastLED.h>

#define LED_PIN     17 // was 1 on 8266
#define NUM_LEDS    50
#define BRIGHTNESS  128
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


bool pixel_panel_enable = false;
uint32_t pixel_previous_millis = 0;
uint32_t neo_interval = 250;

CRGB color_cycle[] = {CRGB::White, CRGB::Red, CRGB::Blue, CRGB::Green};
uint8_t excite_color_index = 0;

#define UPDATES_PER_SECOND 500

CRGBPalette16 currentPalette= RainbowColors_p;
TBlendType    currentBlending= LINEARBLEND;

// extern CRGBPalette16 myRedWhiteBluePalette;
// extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;