int numColors = 6;  // the number of colors in the color wheel
int angleStep = 360 / numColors;

CRGB getColorBetween(CRGB color1, CRGB color2, float percent) {
  return CRGB(
    color1.r + percent * (color2.r - color1.r),
    color1.g + percent * (color2.g - color1.g),
    color1.b + percent * (color2.b - color1.b)
  );
}

CRGB getStrandGradientColor(int strand, int pixel) {
  float percent = (float)(ledX[strand][pixel] / (float)xyMax);
  return getColorBetween(colorLeft, colorRight, percent);
}

CRGB getInnerGradientColor(int pixel) {
  float percent = (float)innerX[pixel] / (float)xyMax;
  return getColorBetween(colorLeft, colorRight, percent);
}

CRGB getOuterGradientColor(int pixel) {
  float percent = (float)outerX[pixel] / (float)xyMax;
  return getColorBetween(colorLeft, colorRight, percent);
}

CRGB getWheelColor(int angle) {
  float percent = (float)(angle % angleStep) / angleStep;
  if (angle >= 0 && angle < 60) {
    return getColorBetween(CRGB::Orange, CRGB::Yellow, percent);
  } else if (angle >= 60 && angle < 120) {
    return getColorBetween(CRGB::Yellow, CRGB::Green, percent);
  } else if (angle >= 120 && angle < 180) {
    return getColorBetween(CRGB::Green, CRGB::Blue, percent);
  } else if (angle >= 180 && angle < 240) {
    return getColorBetween(CRGB::Blue, CRGB::Purple, percent);
  } else if (angle >= 240 && angle < 300) {
    return getColorBetween(CRGB::Purple, CRGB::Red, percent);
  } else {
    return getColorBetween(CRGB::Red, CRGB::Orange, percent);
  }
}

CRGB getStrandWheelColor(int strand, int pixel) {
  return getWheelColor(pixelAngles[strand][pixel]);
}

CRGB getInnerWheelColor(int pixel) {
  return getWheelColor(pixelAnglesInner[pixel]);
}

CRGB getOuterWheelColor(int pixel) {
  return getWheelColor(pixelAnglesOuter[pixel]);
}

CRGB getStrandModeColor(int strand, int pixel) {
  if (colorMode == COLOR_MODE_WHEEL) {
    return getStrandWheelColor(strand, pixel);
  } else if (colorMode == COLOR_MODE_GRADIENT) {
    return getStrandGradientColor(strand, pixel);
  } else if (colorMode == COLOR_MODE_SOLID) {
    return activeColor;
  } else {
    return CRGB(0, 0, 0);
  }
}

CRGB getInnerModeColor(int pixel) {
  if (colorMode == COLOR_MODE_WHEEL) {
    return getInnerWheelColor(pixel);
  } else if (colorMode == COLOR_MODE_GRADIENT) {
    return getInnerGradientColor(pixel);
  } else if (colorMode == COLOR_MODE_SOLID) {
    return activeColor;
  } else {
    return CRGB(0, 0, 0);
  }
}

CRGB getOuterModeColor(int pixel) {
  if (colorMode == COLOR_MODE_WHEEL) {
    return getOuterWheelColor(pixel);
  } else if (colorMode == COLOR_MODE_GRADIENT) {
    return getOuterGradientColor(pixel);
  } else if (colorMode == COLOR_MODE_SOLID) {
    return activeColor;
  } else {
    return CRGB(0, 0, 0);
  }
}

void setAllColorMode() {
  for(int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      leds[strand][pixel] = getStrandModeColor(strand, pixel);
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel] = getInnerModeColor(pixel);
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel] = getOuterModeColor(pixel);
  }
}

void setAllColor(CRGB color) {
  for(int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      leds[strand][pixel] = color;
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel] = color;
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel] = color;
  }
}

void setAllBrightness(uint8_t b) {
  for(int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      leds[strand][pixel].nscale8(b);
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    dotsInner[pixel].nscale8(b);
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    dotsOuter[pixel].nscale8(b);
  }
}
