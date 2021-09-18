CRGB getColorBetween(CRGB color1, CRGB color2, float percent) {
  EVERY_N_SECONDS(1) {
    int rounded = percent * 100;
    if (rounded == 90) {
      Serial.print(color1.r);
      Serial.print(" ");
      Serial.print(color1.g);
      Serial.print(" ");
      Serial.println(color1.b);
      Serial.print(color2.r);
      Serial.print(" ");
      Serial.print(color2.g);
      Serial.print(" ");
      Serial.println(color2.b);
    }
  }
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
  float percent = (float)(angle % 60) / 60;
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

void setAll() {
  for(int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      if (colorMode == COLOR_MODE_WHEEL) {
        leds[strand][pixel] = getStrandWheelColor(strand, pixel);
      } else if (colorMode == COLOR_MODE_GRADIENT) {
        leds[strand][pixel] = getStrandGradientColor(strand, pixel);
      } else if (colorMode == COLOR_MODE_SOLID) {
        leds[strand][pixel] = activeColor;
      } else { // black
        leds[strand][pixel] = CRGB(0, 0, 0);
      }
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    if (colorMode == COLOR_MODE_WHEEL) {
      dotsInner[pixel] = getInnerWheelColor(pixel);
    } else if (colorMode == COLOR_MODE_GRADIENT) {
      dotsInner[pixel] = getInnerGradientColor(pixel);
    } else if (colorMode == COLOR_MODE_SOLID) {
      dotsInner[pixel] = activeColor;
    } else { // black
      dotsInner[pixel] = CRGB(0, 0, 0);
    }
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    if (colorMode == COLOR_MODE_WHEEL) {
      dotsOuter[pixel] = getOuterWheelColor(pixel);
    } else if (colorMode == COLOR_MODE_GRADIENT) {
      dotsOuter[pixel] = getOuterGradientColor(pixel);
    } else if (colorMode == COLOR_MODE_SOLID) {
      dotsOuter[pixel] = activeColor;
    } else { // black
      dotsOuter[pixel] = CRGB(0, 0, 0);
    }
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
