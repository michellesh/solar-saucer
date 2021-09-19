auto spinSpeed = scale(1, 10, 1, 20, true);
int angleThreshold = 10; // how wide the area of LEDs lit up is while spinning

void initPixelAngles() {
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      int x = ledX[strand][pixel];
      int y = ledY[strand][pixel];
      pixelAngles[strand][pixel] = getPixelAngle(x, y);
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    int x = innerX[pixel];
    int y = innerY[pixel];
    pixelAnglesInner[pixel] = getPixelAngle(x, y);
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    int x = outerX[pixel];
    int y = outerY[pixel];
    pixelAnglesOuter[pixel] = getPixelAngle(x, y);
  }
}

int getPixelAngle(int x, int y) {
  int c = xyMax / 2; // both cx and cy (circle origin)
  int d = degrees(atan2(y - c, x - c)) + 90;
  return d < 0 ? 360 + d : d;
}

uint8_t angleDiffToBrightness(int diff) {
  return map(diff, 0, angleThreshold, 255, 0);
}

void setStrandInThreshold(int strand, int pixel) {
  int diff = abs(spinAngle - pixelAngles[strand][pixel]);
  if (diff < angleThreshold) {
    leds[strand][pixel] = getStrandModeColor(strand, pixel).nscale8(
      angleDiffToBrightness(diff)
    );
  }
}

void setInnerInThreshold(int pixel) {
  int diff = abs(spinAngle - pixelAnglesInner[pixel]);
  if (diff < angleThreshold) {
    dotsInner[pixel] = getInnerModeColor(pixel).nscale8(
      angleDiffToBrightness(diff)
    );
  }
}

void setOuterInThreshold(int pixel) {
  int diff = abs(spinAngle - pixelAnglesOuter[pixel]);
  if (diff < angleThreshold) {
    dotsOuter[pixel] = getOuterModeColor(pixel).nscale8(
      angleDiffToBrightness(diff)
    );
  }
}

void vizSpin(float speed) {
  fadeToBlackBy(dotsInner, LEDS_INNER, 20);
  fadeToBlackBy(dotsOuter, LEDS_OUTER, 20);
  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    fadeToBlackBy(leds[strand], STRAND_LENGTH, 20);
  }

  for (int strand = 0; strand < NUM_STRANDS; strand++) {
    for(int pixel = 0; pixel < STRAND_LENGTH; pixel++) {
      setStrandInThreshold(strand, pixel);
    }
  }
  for(int pixel = 0; pixel < LEDS_INNER; pixel++) {
    setInnerInThreshold(pixel);
  }
  for(int pixel = 0; pixel < LEDS_OUTER; pixel++) {
    setOuterInThreshold(pixel);
  }

  spinAngle += spinSpeed(speed);
  if (spinAngle > 360) {
    spinAngle -= 360;
  }
}
