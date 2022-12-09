
#include "treatbot_globals.h"
#include "pixel-panel.h"

void setup_pixel_panel() {
  print_message("Pixels");
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
}

void FillLEDsFromPaletteColors( uint8_t colorIndex) {
    uint8_t brightness = 255;

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}


void process_pixels() {
  if (pixel_panel_enable) {
    if (millis() - pixel_previous_millis >= neo_interval) {
      pixel_previous_millis = millis();
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

