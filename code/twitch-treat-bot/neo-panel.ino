
void FillLEDsFromPaletteColors( uint8_t colorIndex) {
    uint8_t brightness = 255;

    currentPalette = RainbowColors_p;
    currentBlending = LINEARBLEND;
    
    for( int i = 0; i < NUM_LEDS; i++) {
        leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
        colorIndex += 3;
    }
}

