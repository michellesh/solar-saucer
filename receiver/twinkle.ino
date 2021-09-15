// Overall twinkle speed.
// 0 (VERY slow) to 8 (VERY fast).  
// 4, 5, and 6 are recommended, default is 4.
#define TWINKLE_SPEED 4

// Overall twinkle density.
// 0 (NONE lit) to 8 (ALL lit at once).  
// Default is 5.
#define TWINKLE_DENSITY 5

// How often to change color palettes.
#define SECONDS_PER_PALETTE  30
// Also: toward the bottom of the file is an array 
// called "ActivePaletteList" which controls which color
// palettes are used; you can add or remove color palettes
// from there freely.

// Background color for 'unlit' pixels
// Can be set to CRGB::Black if desired.
CRGB gBackgroundColor = CRGB::Black; 
// Example of dim incandescent fairy light background color
// CRGB gBackgroundColor = CRGB(CRGB::FairyLight).nscale8_video(16);

// If AUTO_SELECT_BACKGROUND_COLOR is set to 1,
// then for any palette where the first two entries 
// are the same, a dimmed version of that color will
// automatically be used as the background color.
#define AUTO_SELECT_BACKGROUND_COLOR 0

// If COOL_LIKE_INCANDESCENT is set to 1, colors will 
// fade out slighted 'reddened', similar to how
// incandescent bulbs change color as they get dim down.
#define COOL_LIKE_INCANDESCENT 1

//  This function loops over each pixel, calculates the 
//  adjusted 'clock' that this pixel should use, and calls 
//  "CalculateOneTwinkle" on each pixel.  It then displays
//  either the twinkle color of the background color, 
//  whichever is brighter.
void vizTwinkle(uint8_t speed) {

  // "PRNG16" is the pseudorandom number generator
  // It MUST be reset to the same starting value each time
  // this function is called, so that the sequence of 'random'
  // numbers that it generates is (paradoxically) stable.
  uint16_t PRNG16 = 11337;
  
  uint32_t clock32 = millis();

  // Set up the background color, "bg".
  // if AUTO_SELECT_BACKGROUND_COLOR == 1, and the first two colors of
  // the current palette are identical, then a deeply faded version of
  // that color is used for the background color
  CRGB bg;
  if ((AUTO_SELECT_BACKGROUND_COLOR == 1) &&
      (gCurrentPalette[0] == gCurrentPalette[1])) {
    bg = gCurrentPalette[0];
    uint8_t bglight = bg.getAverageLight();
    if (bglight > 64) {
      bg.nscale8_video(16); // very bright, so scale to 1/16th
    } else if (bglight > 16) {
      bg.nscale8_video(64); // not that bright, so scale to 1/4th
    } else {
      bg.nscale8_video(86); // dim, scale to 1/3rd.
    }
  } else {
    bg = gBackgroundColor; // just use the explicitly defined background color
  }

  uint8_t backgroundBrightness = bg.getAverageLight();
  
  for(uint8_t strand = 0; strand < 18; strand++) {
    for(uint8_t pixel = 0; pixel < getNumPixels(strand); pixel++) {
      leds[strand][pixel] = getTwinkleColor(PRNG16, clock32, bg, backgroundBrightness);
    }
  }
  for(uint8_t pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel] = getTwinkleColor(PRNG16, clock32, bg, backgroundBrightness);
  }
  for(uint8_t pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel] = getTwinkleColor(PRNG16, clock32, bg, backgroundBrightness);
  }
}

CRGB getTwinkleColor(uint16_t &PRNG16, uint32_t clock32, CRGB bg, uint8_t backgroundBrightness) {
  PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
  uint16_t myclockoffset16 = PRNG16; // use that number as clock offset
  PRNG16 = (uint16_t)(PRNG16 * 2053) + 1384; // next 'random' number
  // use that number as clock speed adjustment factor (in 8ths, from 8/8ths to 23/8ths)
  uint8_t myspeedmultiplierQ5_3 = ((((PRNG16 & 0xFF)>>4) + (PRNG16 & 0x0F)) & 0x0F) + 0x08;
  uint32_t myclock30 = (uint32_t)((clock32 * myspeedmultiplierQ5_3) >> 3) + myclockoffset16;
  uint8_t  myunique8 = PRNG16 >> 8; // get 'salt' value for this pixel

  // We now have the adjusted 'clock' for this pixel, now we call
  // the function that computes what color the pixel should be based
  // on the "brightness = f( time )" idea.
  CRGB c = computeOneTwinkle(myclock30, myunique8, speed);

  uint8_t cbright = c.getAverageLight();
  int16_t deltabright = cbright - backgroundBrightness;
  if (deltabright >= 32 || (!bg)) {
    // If the new pixel is significantly brighter than the background color, 
    // use the new color.
    return c;
  } else if (deltabright > 0 ) {
    // If the new pixel is just slightly brighter than the background color,
    // mix a blend of the new color and the background color
    return blend(bg, c, deltabright * 8);
  } else { 
    // if the new pixel is not at all brighter than the background color,
    // just use the background color.
    return bg;
  }
}


//  This function takes a time in pseudo-milliseconds,
//  figures out brightness = f( time ), and also hue = f( time )
//  The 'low digits' of the millisecond time are used as 
//  input to the brightness wave function.  
//  The 'high digits' are used to select a color, so that the color
//  does not change over the course of the fade-in, fade-out
//  of one cycle of the brightness wave function.
//  The 'high digits' are also used to determine whether this pixel
//  should light at all during this cycle, based on the TWINKLE_DENSITY.
CRGB computeOneTwinkle( uint32_t ms, uint8_t salt, uint8_t speed) {
  uint16_t ticks = ms >> (8-speed);
  uint8_t fastcycle8 = ticks;
  uint16_t slowcycle16 = (ticks >> 8) + salt;
  slowcycle16 += sin8( slowcycle16);
  slowcycle16 =  (slowcycle16 * 2053) + 1384;
  uint8_t slowcycle8 = (slowcycle16 & 0xFF) + (slowcycle16 >> 8);
  
  uint8_t bright = 0;
  if( ((slowcycle8 & 0x0E)/2) < TWINKLE_DENSITY) {
    bright = attackDecayWave8( fastcycle8);
  }

  uint8_t hue = slowcycle8 - salt;
  CRGB c;
  if( bright > 0) {
    c = ColorFromPalette( gCurrentPalette, hue, bright, NOBLEND);
    if( COOL_LIKE_INCANDESCENT == 1 ) {
      coolLikeIncandescent( c, fastcycle8);
    }
  } else {
    c = CRGB::Black;
  }
  return c;
}


// This function is like 'triwave8', which produces a 
// symmetrical up-and-down triangle sawtooth waveform, except that this
// function produces a triangle wave with a faster attack and a slower decay:

uint8_t attackDecayWave8( uint8_t i)
{
  if( i < 86) {
    return i * 3;
  } else {
    i -= 86;
    return 255 - (i + (i/2));
  }
}

// This function takes a pixel, and if its in the 'fading down'
// part of the cycle, it adjusts the color a little bit like the 
// way that incandescent bulbs fade toward 'red' as they dim.
void coolLikeIncandescent( CRGB& c, uint8_t phase)
{
  if( phase < 128) return;

  uint8_t cooling = (phase - 128) >> 4;
  c.g = qsub8( c.g, cooling);
  c.b = qsub8( c.b, cooling * 2);
}
