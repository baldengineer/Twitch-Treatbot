#include <FastLED.h>

#define LED_PIN     1
#define NUM_LEDS    50
#define BRIGHTNESS  32
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];


bool neo_panel_enable = false;

#define UPDATES_PER_SECOND 500

CRGBPalette16 currentPalette= RainbowColors_p;
TBlendType    currentBlending= LINEARBLEND;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;